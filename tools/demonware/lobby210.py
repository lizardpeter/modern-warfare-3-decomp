"""MW3 Store lobby v210 handshake records and transcript derivation."""

from dataclasses import dataclass
import hashlib
import hmac
import struct

PROTOCOL = 210
TITLE = 0x906
TICKET_SIZE = 128
_HELLO = struct.Struct("<IIIII8s")
_TRANSCRIPT_HELLO = struct.Struct("<III8s")
_CHALLENGE = struct.Struct("<IQ8s")
_SIZE = struct.Struct("<I")
_ACK_TAG = b"\xab\x81"
_CLIENT_AUTH_TAG = b"\xab\x82"
_AUTH_DONE_TAG = b"\xab\x83"
_AUTH_HEADER_BITS = 83
_AUTH_BODY_SIZE = (_AUTH_HEADER_BITS + TICKET_SIZE * 8 + 7) // 8


@dataclass(frozen=True)
class ClientHello:
    frame_limit: int
    nonce: bytes

    @classmethod
    def decode(cls, wire: bytes):
        if len(wire) != _HELLO.size:
            raise ValueError("invalid MW3 client hello length")
        legacy_min, legacy_max, minimum, maximum, frame_limit, nonce = _HELLO.unpack(wire)
        if (legacy_min, legacy_max, minimum, maximum) != (200, 200, PROTOCOL, PROTOCOL):
            raise ValueError("unsupported MW3 lobby protocol")
        if not 2 + _AUTH_BODY_SIZE + 8 <= frame_limit <= 65535:
            raise ValueError("unsupported MW3 frame limit")
        return cls(frame_limit, nonce)

    def transcript_bytes(self) -> bytes:
        return _TRANSCRIPT_HELLO.pack(PROTOCOL, PROTOCOL, self.frame_limit, self.nonce)


def _frame(tag: bytes, body: bytes) -> bytes:
    return _SIZE.pack(len(tag) + len(body)) + tag + body


def server_challenge(connection_id: int, nonce: bytes) -> bytes:
    if not connection_id or len(nonce) != 8:
        raise ValueError("invalid lobby challenge")
    return _frame(_ACK_TAG, _CHALLENGE.pack(PROTOCOL, connection_id, nonce))


@dataclass(frozen=True)
class ClientAuth:
    title: int
    iv_seed: int
    ticket: bytes
    proof: bytes
    transcript_bytes: bytes

    @classmethod
    def decode(cls, wire: bytes):
        if len(wire) != 4 + 2 + _AUTH_BODY_SIZE + 8:
            raise ValueError("invalid MW3 client-auth length")
        if _SIZE.unpack_from(wire)[0] != len(wire) - 4 or wire[4:6] != _CLIENT_AUTH_TAG:
            raise ValueError("invalid MW3 client-auth frame")
        body = int.from_bytes(wire[6:-8], "little")
        if body & 0x1ff != 0x107 or (body >> 9) & 31 != 8 or (body >> 46) & 31 != 8:
            raise ValueError("invalid MW3 client-auth bit header")
        title = (body >> 14) & 0xffffffff
        iv_seed = (body >> 51) & 0xffffffff
        ticket = ((body >> _AUTH_HEADER_BITS) & ((1 << (8 * TICKET_SIZE)) - 1)).to_bytes(TICKET_SIZE, "little")
        if title != TITLE:
            raise ValueError("unexpected MW3 title")
        return cls(title, iv_seed, ticket, wire[-8:], wire[:-8])


def _expand(secret: bytes, label: bytes, length: int) -> bytes:
    output = bytearray()
    previous = b""
    for counter in range(1, (length + 19) // 20 + 1):
        previous = hmac.digest(secret, previous + label + bytes((counter,)), "sha1")
        output.extend(previous)
    return bytes(output[:length])


def verify_client_auth(hello: ClientHello, challenge: bytes, auth: ClientAuth, session_key: bytes):
    """Return the server-auth reply and raw BDDATA material for a service-issued ticket."""
    if len(session_key) != 24 or len(challenge) != 26:
        raise ValueError("invalid MW3 handshake material")
    digest = hashlib.sha1(hello.transcript_bytes() + challenge + auth.transcript_bytes).digest()
    secret = hmac.digest(digest, session_key, "sha1")
    proof = _expand(secret, b"CLIENTCHAL", 16)
    if not hmac.compare_digest(auth.proof, proof[:8]):
        raise ValueError("MW3 client proof does not match the issued ticket")
    return _frame(_AUTH_DONE_TAG, proof[8:]), _expand(secret, b"BDDATA", 72)
