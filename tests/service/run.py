#!/usr/bin/env python3
"""The local DemonWare service end to end, as the game's client sees it (docs/online.md).

    python3 tests/service/run.py        # needs clang, lld-link, llvm-dlltool and Wine

Builds enhanced/DW's service into tests/service/service_host.cpp's console program, runs it under Wine and checks,
with the reference protocol code in tools/demonware/ playing the game's part:

- the auth reply has the Store shape (auth_task 45, code 700, the request's IV seed) and its client ticket decodes
  to title 906, the local account and name, and a session key;
- the lobby accepts a protocol 210 hello, sends a challenge, accepts a client auth frame whose proof is made with
  that session key and answers with the server's proof; a proof made with any other key is refused;
- records in both directions decrypt and verify with the BDDATA keys: the service reads what the client sends (it
  traces it) and a corrupted record ends the session.
"""
import base64
import hashlib
import hmac
import json
import os
import socket
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.demonware.lobby210 import _expand  # noqa: E402
from tools.demonware.records210 import ServerRecords  # noqa: E402

TRIPLE = "--target=x86_64-pc-windows-msvc"
DEFS = {
    "kernel32.dll": ["GetStdHandle", "WriteFile", "Sleep", "ExitProcess", "CreateFileA", "GetModuleHandleExA",
                     "GetModuleFileNameA", "InitializeCriticalSection", "EnterCriticalSection",
                     "LeaveCriticalSection", "LocalAlloc", "LocalFree", "CreateThread", "CloseHandle",
                     "GetSystemTimeAsFileTime"],
    "ws2_32.dll": ["WSAStartup", "socket", "bind", "listen", "accept", "recv", "send", "closesocket", "getsockname",
                   "htons", "ntohs", "WSAGetLastError"],
    "bcrypt.dll": ["BCryptOpenAlgorithmProvider", "BCryptCloseAlgorithmProvider", "BCryptCreateHash",
                   "BCryptHashData", "BCryptFinishHash", "BCryptDestroyHash", "BCryptGenerateSymmetricKey",
                   "BCryptSetProperty", "BCryptEncrypt", "BCryptDecrypt", "BCryptDestroyKey", "BCryptGenRandom"],
}


def build(work):
    objs = []
    for src in ("tests/service/service_host.cpp", "enhanced/DW/local_demonware.cpp", "enhanced/DW/local_crypto.cpp"):
        obj = work / (Path(src).stem + ".obj")
        subprocess.run(["clang", TRIPLE, "-O1", "-ffreestanding", "-fno-builtin", "-fno-exceptions", "-fno-rtti",
                        "-fno-stack-protector", "-mno-stack-arg-probe", "-std=c++20", "-I", ROOT / "enhanced/include",
                        "-I", ROOT / "tools/recon/include", "-c", ROOT / src, "-o", obj], check=True)
        objs.append(obj)
    libs = []
    for dll, names in DEFS.items():
        d = work / (dll + ".def")
        d.write_text(f"LIBRARY {dll}\nEXPORTS\n" + "\n".join(names) + "\n")
        lib = work / (dll + ".lib")
        subprocess.run(["llvm-dlltool", "-m", "i386:x86-64", "-d", d, "-l", lib], check=True)
        libs.append(lib)
    exe = work / "service_host.exe"
    subprocess.run(["lld-link", "/subsystem:console", "/nodefaultlib", "/entry:mainCRTStartup", f"/out:{exe}",
                    *objs, *libs], check=True)
    return exe


def recv_exact(s, n):
    data = b""
    while len(data) < n:
        chunk = s.recv(n - len(data))
        if not chunk:
            raise AssertionError(f"connection closed after {len(data)} of {n} bytes")
        data += chunk
    return data


def handshake(port, server_ticket, session_key, seed=0x10203040):
    s = socket.create_connection(("127.0.0.1", port), timeout=10)
    nonce = os.urandom(8)
    hello = struct.pack("<IIIII", 200, 200, 210, 210, 0xFFFF) + nonce
    s.sendall(hello)
    challenge = recv_exact(s, 26)
    assert challenge[:6] == bytes.fromhex("16000000ab81") and struct.unpack_from("<I", challenge, 6)[0] == 210
    body = (0x107 | (8 << 9) | (0x906 << 14) | (8 << 46) | (seed << 51) |
            (int.from_bytes(server_ticket, "little") << 83)).to_bytes(139, "little")
    prefix = struct.pack("<I2s", 149, b"\xab\x82") + body
    transcript = struct.pack("<III8s", 210, 210, 0xFFFF, nonce) + challenge + prefix
    secret = hmac.digest(hashlib.sha1(transcript).digest(), session_key, "sha1")
    proof = _expand(secret, b"CLIENTCHAL", 16)
    s.sendall(prefix + proof[:8])
    return s, secret, proof


def main():
    with tempfile.TemporaryDirectory() as tmp:
        work = Path(tmp)
        exe = build(work)
        env = dict(os.environ, WINEDEBUG="-all")
        env.pop("TMPDIR", None)  # Wine 9.0 frees TMPDIR's own string at startup
        host = subprocess.Popen(["wine", str(exe)], cwd=work, stdout=subprocess.PIPE, env=env)
        try:
            port_line = host.stdout.readline().decode().split()
            auth_line = host.stdout.readline().decode().strip()
            assert port_line[0] == "port", port_line
            port = int(port_line[1])
            assert auth_line.startswith("auth "), auth_line
            reply = json.loads(auth_line[5:])
            assert (reply["auth_task"], reply["code"], reply["iv_seed"]) == (45, 700, 305419896), reply
            client = base64.b64decode(reply["client_ticket"])
            server_ticket = base64.b64decode(reply["server_ticket"])
            magic, _, title, _, _, _, account, name, key = struct.unpack("<IBIIIQQ64s24s7x", client)
            assert (magic, title, account) == (0xEFBDADDE, 0x906, 0x0009000000000001)
            assert name.split(b"\0")[0] == b"Unknown Soldier" and len(server_ticket) == 128
            assert json.loads(reply["extra_data"]) == {"sbx": "RETAIL", "dpi": "MW3"}
            print("ok   auth reply and client ticket")

            s, secret, proof = handshake(port, server_ticket, key)
            done = recv_exact(s, 14)
            assert done == bytes.fromhex("0a000000ab83") + proof[8:16], done.hex()
            print("ok   protocol 210 handshake with the issued ticket")
            material = _expand(secret, b"BDDATA", 72)
            # the client's view: its send keys are the server's receive keys
            records = ServerRecords(material[20:40] + material[:20] + material[56:72] + material[40:56], 0xFFFF)
            def read_record():
                size = struct.unpack("<I", recv_exact(s, 4))[0]
                return records.decode(struct.pack("<I", size) + recv_exact(s, size))
            kind, body = read_record()
            assert kind == 4 and body[0] == 10 and struct.unpack_from("<Q", body, 1)[0] >= 1, (kind, body.hex())
            print("ok   connection ID header")
            # bdTitleUtilities::getServerTime: one typed uint32
            s.sendall(records.encode(0x86, b"\x0c\x03\x06"))
            kind, body = read_record()
            assert kind == 1 and body[0] == 10 and body[9] == 8 and struct.unpack_from("<I", body, 10)[0] == 0, body.hex()
            assert body[14:16] == b"\x03\x06" and body[16] == 8 and body[21] == 8 and body[26] == 8, body.hex()
            assert abs(struct.unpack_from("<I", body, 27)[0] - time.time()) < 60, body.hex()
            print("ok   server time task")
            # a service the game hasn't been seen to need: BD_SERVICE_NOT_AVAILABLE
            s.sendall(records.encode(0x86, b"\x01\x02\x03 task"))
            s.sendall(records.encode(0x86, b"\x0a\x03\x07args"))
            kind, body = read_record()
            assert kind == 1 and len(body) == 14 and struct.unpack_from("<I", body, 10)[0] == 108, body.hex()
            assert struct.unpack_from("<Q", body, 1)[0] == 2, body.hex()
            s.sendall(records.encode(0x0C, b""))
            time.sleep(0.5)
            corrupt = bytearray(records.encode(0x86, b"x"))
            corrupt[-1] ^= 1
            s.sendall(bytes(corrupt))
            s.settimeout(10)
            assert s.recv(1) == b"", "the service kept a session with a corrupt record"
            print("ok   encrypted records accepted, a corrupt one ends the session")

            bad, _, _ = handshake(port, server_ticket, bytes(24))
            bad.settimeout(10)
            assert bad.recv(1) == b"", "a proof from another key was accepted"
            unknown, _, _ = handshake(port, os.urandom(128), key)
            unknown.settimeout(10)
            assert unknown.recv(1) == b"", "a ticket the service didn't issue was accepted"
            print("ok   wrong session key and unknown ticket refused")
            time.sleep(0.5)
            log = (work / "iw5_enhanced.log").read_text()
            assert "message type 86 (8 bytes): 010203207461736b" in log and "message type 0c (0 bytes)" in log, log
            assert "service a task 7: status 108" in log and "service c task 6: status 0" in log, log
            print("ok   records traced")
        finally:
            host.kill()
            subprocess.run(["wineserver", "-k"], env=env, check=False)
    return 0


if __name__ == "__main__":
    sys.exit(main())
