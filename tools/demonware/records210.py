"""Authenticated lobby records for the server side of an MW3 v210 session."""

import hmac
import secrets
import struct

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

_HEADER = struct.Struct("<I2sI16s")
_SIZE = struct.Struct("<I")
_TAG = b"\xab\x85"
_MAC_SIZE = 8
_BLOCK = 16


class ServerRecords:
    def __init__(self, material: bytes, frame_limit: int):
        if len(material) != 72 or not 46 <= frame_limit <= 65535:
            raise ValueError("invalid lobby record parameters")
        self._receive_mac = material[:20]
        self._send_mac = material[20:40]
        self._receive_key = material[40:56]
        self._send_key = material[56:72]
        self._frame_limit = frame_limit
        self._receive_counter = 1
        self._send_counter = 1

    def decode(self, frame: bytes) -> tuple[int, bytes]:
        if not _HEADER.size + _BLOCK + _MAC_SIZE <= len(frame) <= self._frame_limit + 4:
            raise ValueError("invalid lobby record length")
        size, tag, counter, iv = _HEADER.unpack_from(frame)
        if size != len(frame) - 4 or tag != _TAG or counter != self._receive_counter:
            raise ValueError("invalid lobby record header or sequence")
        ciphertext = frame[_HEADER.size:-_MAC_SIZE]
        if len(ciphertext) % _BLOCK:
            raise ValueError("invalid lobby ciphertext block count")
        mac = hmac.digest(self._receive_mac, frame[:-_MAC_SIZE], "sha1")[:_MAC_SIZE]
        if not hmac.compare_digest(mac, frame[-_MAC_SIZE:]):
            raise ValueError("invalid lobby record MAC")
        aes = Cipher(algorithms.AES(self._receive_key), modes.CBC(iv)).decryptor()
        plain = aes.update(ciphertext) + aes.finalize()
        length = _SIZE.unpack_from(plain)[0]
        if length > len(plain) - 5:
            raise ValueError("invalid lobby message length")
        self._receive_counter = (counter + 1) & 0xffffffff
        return plain[4], plain[5:5 + length]

    def encode(self, message_type: int, payload: bytes) -> bytes:
        if not 0 <= message_type <= 255:
            raise ValueError("invalid lobby message type")
        size = (len(payload) + 5 + _BLOCK - 1) // _BLOCK * _BLOCK
        wire_length = _HEADER.size - 4 + size + _MAC_SIZE
        if wire_length > self._frame_limit:
            raise ValueError("lobby message exceeds frame limit")
        plain = (_SIZE.pack(len(payload)) + bytes((message_type,)) + payload).ljust(size, b"\0")
        iv = secrets.token_bytes(16)
        header = _HEADER.pack(wire_length, _TAG, self._send_counter, iv)
        aes = Cipher(algorithms.AES(self._send_key), modes.CBC(iv)).encryptor()
        frame = header + aes.update(plain) + aes.finalize()
        mac = hmac.digest(self._send_mac, frame, "sha1")[:_MAC_SIZE]
        self._send_counter = (self._send_counter + 1) & 0xffffffff
        return frame + mac
