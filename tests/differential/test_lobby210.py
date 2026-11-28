"""Check the MW3 v210 key expansion against the pinned Store client."""

import hashlib
import hmac
import json
import struct
from base64 import b64decode

from unicorn import x86_const as X

from tools.demonware.auth import encode_auth_reply
from tools.demonware.lobby210 import ClientAuth, ClientHello, _expand, server_challenge, verify_client_auth


def check_lobby_v210(expand):
    hello = ClientHello.decode(bytes.fromhex("c8000000c8000000d2000000d2000000ffff0000") + b"client01")
    challenge = server_challenge(7, b"server01")
    assert challenge == bytes.fromhex("16000000ab81d20000000700000000000000") + b"server01"
    ticket = bytes(range(128))
    seed = 0x10203040
    body = (0x107 | (8 << 9) | (0x906 << 14) | (8 << 46) | (seed << 51) |
            (int.from_bytes(ticket, "little") << 83)).to_bytes(139, "little")
    prefix = struct.pack("<I2s", 149, b"\xab\x82") + body
    auth = ClientAuth.decode(prefix + bytes.fromhex("b8340c7f58adea29"))
    assert (auth.title, auth.iv_seed, auth.ticket) == (0x906, seed, ticket)
    reply, keys = verify_client_auth(hello, challenge, auth, bytes(range(24)))
    assert reply.hex() == "0a000000ab83c1d2a12c0712365c"
    assert keys.hex() == (
        "9bc1adce4bcb4a55397dc81c57aa306555a4cc559221876270c004efbd95357043a"
        "b36441dfcb20d0556f14df64957ca333728e49aa618ac4c19b080098988473f69cfb2c00932ad")
    assert expand(bytes(range(20)), b"CLIENTCHAL", 16) == _expand(bytes(range(20)), b"CLIENTCHAL", 16)
    assert expand(bytes(range(20)), b"BDDATA", 72) == _expand(bytes(range(20)), b"BDDATA", 72)
    try:
        verify_client_auth(hello, challenge, auth, bytes(24))
    except ValueError:
        pass
    else:
        raise AssertionError("an unrelated session key authenticated the client proof")


def native_expander():
    from harness import Image, Machine, original_exe

    machine = Machine(Image(original_exe("iw5mp_ship")))
    ctx = {}

    def initialize(mu):
        key = mu.mem_read(mu.reg_read(X.UC_X86_REG_RDX), mu.reg_read(X.UC_X86_REG_R8))
        ctx["hash"] = hmac.new(bytes(key), digestmod=hashlib.sha1)
        return mu.reg_read(X.UC_X86_REG_RCX)

    def update(mu):
        data = mu.mem_read(mu.reg_read(X.UC_X86_REG_RDX), mu.reg_read(X.UC_X86_REG_R8))
        ctx["hash"].update(data)
        return 1

    def finish(mu):
        mu.mem_write(mu.reg_read(X.UC_X86_REG_RDX), ctx["hash"].digest())
        return 1

    fakes = {0x140448510: initialize, 0x140448860: update, 0x140448740: finish, 0x140448600: lambda mu: 0}

    def expand(secret, label, length):
        status, buffers, _, _ = machine.call(0x140406B40, [secret, len(secret), label, len(label), bytes(length),
                                                           length], "bool", fakes=fakes)
        assert status == 1, status
        return buffers[2]

    return expand


def check_auth_ticket_decoder():
    from harness import Image, Machine, original_exe

    machine = Machine(Image(original_exe("iw5mp_ship")))
    account = 0x1234567890ABCDEF
    server_ticket = bytes(reversed(range(128)))
    session_key = bytes(range(24))
    response = json.loads(encode_auth_reply(77, account, "Test Player", session_key, server_ticket))
    assert (response["auth_task"], response["code"], response["iv_seed"]) == (45, 700, 77)
    assert b64decode(response["server_ticket"]) == server_ticket
    assert set(json.loads(response["extra_data"])) == {"sbx", "dpi"}
    _, buffers, _, _ = machine.call(0x1404049D0, [bytes(0x88), b64decode(response["client_ticket"])], "void")
    ticket = buffers[0]
    assert struct.unpack_from("<I", ticket)[0] == 0xEFBDADDE
    assert struct.unpack_from("<I", ticket, 8)[0] == 0x906
    assert struct.unpack_from("<Q", ticket, 32)[0] == account
    assert ticket[40:104].split(b"\0")[0] == b"Test Player"
    assert ticket[104:128] == session_key


if __name__ == "__main__":
    check_lobby_v210(native_expander())
    check_auth_ticket_decoder()
    print("MW3 lobby v210 handshake vectors, native key expansion and native ticket decoding passed")
