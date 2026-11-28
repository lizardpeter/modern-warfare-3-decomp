"""Exercise the DLL's protocol 210 proof verifier using modeled Windows CNG."""

import hashlib
import hmac
import struct

from unicorn import x86_const as X


def check_native_lobby_proof(ctx):
    state = {}
    regs = (X.UC_X86_REG_RCX, X.UC_X86_REG_RDX, X.UC_X86_REG_R8, X.UC_X86_REG_R9)

    def result(mu, status=0):
        mu.reg_write(X.UC_X86_REG_RAX, status & 0xffffffff)

    def open_algorithm(_, mu):
        out = mu.reg_read(regs[0])
        mu.mem_write(out, struct.pack("<Q", 2 if mu.reg_read(regs[3]) == 8 else 1))
        result(mu)

    def create_hash(_, mu):
        provider = mu.reg_read(regs[0])
        out = mu.reg_read(regs[1])
        secret, length = struct.unpack("<QI", mu.mem_read(mu.reg_read(X.UC_X86_REG_RSP) + 0x28, 12))
        if provider == 2:
            state["hash"] = hmac.new(bytes(mu.mem_read(secret, length)), digestmod="sha1")
        else:
            state["hash"] = hashlib.sha1()
        mu.mem_write(out, struct.pack("<Q", 3))
        result(mu)

    def hash_data(_, mu):
        state["hash"].update(bytes(mu.mem_read(mu.reg_read(regs[1]), mu.reg_read(regs[2]))))
        result(mu)

    def finish_hash(_, mu):
        mu.mem_write(mu.reg_read(regs[1]), state["hash"].digest())
        result(mu)

    ctx.imports({f"bcrypt.dll!{name}": (fn, 0) for name, fn in {
        "BCryptOpenAlgorithmProvider": open_algorithm,
        "BCryptCreateHash": create_hash,
        "BCryptHashData": hash_data,
        "BCryptFinishHash": finish_hash,
        "BCryptDestroyHash": lambda _, mu: result(mu),
        "BCryptCloseAlgorithmProvider": lambda _, mu: result(mu),
    }.items()})
    hello = struct.pack("<IIIII8s", 200, 200, 210, 210, 65535, b"client01")
    challenge = struct.pack("<I2sIQ8s", 22, b"\xab\x81", 210, 7, b"server01")
    ticket = int.from_bytes(bytes(range(128)), "little")
    body = (0x107 | (8 << 9) | (0x906 << 14) | (8 << 46) | (0x10203040 << 51) |
            (ticket << 83)).to_bytes(139, "little")
    auth = struct.pack("<I2s", 149, b"\xab\x82") + body + bytes.fromhex("b8340c7f58adea29")
    verify = ctx.enhanced("IW5_DemonWare210Verify")

    def call(frame, key):
        return ctx.m.call(verify, [hello, len(hello), challenge, len(challenge),
                                  frame, len(frame), key, bytes(14), bytes(72)], "bool")

    status, buffers, _, _ = call(auth, bytes(range(24)))
    assert status == 1, status
    assert buffers[-2].hex() == "0a000000ab83c1d2a12c0712365c"
    assert buffers[-1].hex() == (
        "9bc1adce4bcb4a55397dc81c57aa306555a4cc559221876270c004efbd95357043a"
        "b36441dfcb20d0556f14df64957ca333728e49aa618ac4c19b080098988473f69cfb2c00932ad")

    for frame, key in ((auth, bytes(24)), (auth[:-1], bytes(range(24)))):
        status, buffers, _, _ = call(frame, key)
        assert status == 0, status
        assert buffers[-2:] == [bytes(14), bytes(72)]


CHECKS = [check_native_lobby_proof]
