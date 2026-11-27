#!/usr/bin/env python3
"""Xbox 360 XEX2 -> PE image, for the TU1 reference builds (docs/xbox360.md).

    python tools/unpack/xex.py <in.xex> <out.pe>            unpack, print the result's sha256
    python tools/unpack/xex.py --info <in.xex>               print the headers only

The output is the image as the loader maps it (sections at their virtual addresses, image base 0x82000000), which
is also a valid PE file: an XEX's PE has file alignment equal to section alignment. Deterministic, so target.toml
pins its sha256 (`unpacked_sha256`) and tools/fetch_originals.py checks it; the unpacked image is never committed.

Supports what the TU1 drop uses: AES-128 encryption (retail key, then the development key) with basic compression
(data blocks followed by zero runs), plus unencrypted files and the uncompressed form. Normal (LZX) compression is
refused with a clear error rather than guessed at.
"""
import argparse
import hashlib
import struct
import sys

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

# Public Xbox 360 XEX2 master keys: retail, then development kits (all zero).
KEYS = {
    "retail": bytes.fromhex("20B185A59D28FDC340583FBB0896BF91"),
    "devkit": bytes(16),
}

H_FILE_FORMAT = 0x000003FF
H_ENTRY_POINT = 0x00010100
H_IMAGE_BASE = 0x00010201
H_ORIGINAL_PE_NAME = 0x000183FF
H_EXECUTION_INFO = 0x00040006


def aes(key, data, mode):
    c = Cipher(algorithms.AES(key), mode).decryptor()
    return c.update(data) + c.finalize()


class Xex:
    def __init__(self, data):
        self.data = data
        magic, self.module_flags, self.pe_offset, _, self.sec_offset, count = struct.unpack_from(">4s5I", data, 0)
        if magic != b"XEX2":
            raise ValueError(f"not an XEX2 file (magic {magic!r})")
        self.headers = {}
        for i in range(count):
            key, value = struct.unpack_from(">II", data, 24 + 8 * i)
            self.headers[key] = value
        ff = self.headers[H_FILE_FORMAT]
        info_size, self.encryption, self.compression = struct.unpack_from(">IHH", data, ff)
        self.blocks = []
        if self.compression == 1:
            for off in range(ff + 8, ff + info_size, 8):
                self.blocks.append(struct.unpack_from(">II", data, off))
        s = self.sec_offset
        self.image_size = struct.unpack_from(">I", data, s + 4)[0]
        self.image_flags, self.load_address = struct.unpack_from(">II", data, s + 0x10C)
        self.encrypted_key = data[s + 0x150:s + 0x160]

    def header_value(self, key):
        """Small headers (low byte 0 or 1) are stored inline; others point at their data."""
        return self.headers.get(key)

    def original_name(self):
        off = self.headers.get(H_ORIGINAL_PE_NAME)
        if off is None:
            return None
        size = struct.unpack_from(">I", self.data, off)[0]
        return self.data[off + 4:off + size].split(b"\0")[0].decode("ascii", "replace")

    def info(self):
        return {
            "module_flags": f"{self.module_flags:#x}",
            "encryption": {0: "none", 1: "aes"}.get(self.encryption, self.encryption),
            "compression": {0: "none", 1: "basic", 2: "normal (lzx)", 3: "delta"}.get(self.compression, self.compression),
            "image_size": f"{self.image_size:#x}",
            "load_address": f"{self.load_address:08X}",
            "entry_point": f"{self.headers.get(H_ENTRY_POINT, 0):08X}",
            "image_base": f"{self.headers.get(H_IMAGE_BASE, 0):08X}",
            "original_name": self.original_name(),
            "blocks": len(self.blocks),
        }

    def payload(self):
        """The PE data after decryption; tries each known master key and keeps the one that yields 'MZ'."""
        raw = self.data[self.pe_offset:]
        if self.encryption == 0:
            return raw, None
        raw = raw[:len(raw) - len(raw) % 16]
        for name, master in KEYS.items():
            session = aes(master, self.encrypted_key, modes.ECB())
            head = aes(session, raw[:16], modes.CBC(bytes(16)))
            if head[:2] == b"MZ":
                return aes(session, raw, modes.CBC(bytes(16))), name
        raise ValueError("no known XEX2 key decrypts this file to a PE")

    def image(self):
        data, key = self.payload()
        if self.compression == 0:
            out = bytearray(data[:self.image_size])
        elif self.compression == 1:
            out = bytearray()
            pos = 0
            for data_size, zero_size in self.blocks:
                out += data[pos:pos + data_size]
                out += bytes(zero_size)
                pos += data_size
        else:
            raise ValueError(f"compression type {self.compression} is not supported (only none and basic)")
        if len(out) < self.image_size:
            out += bytes(self.image_size - len(out))
        if out[:2] != b"MZ":
            raise ValueError("unpacked image does not start with MZ")
        return to_file_layout(bytes(out[:self.image_size])), key


def to_file_layout(image):
    """The loader's mapped image -> the PE file the linker wrote (before imagexex): headers, then each section's raw
    data copied from its virtual address back to its PointerToRawData. Section headers keep the original file
    offsets, so the result loads in any PE reader (Ghidra, pefile) with the original layout."""
    pe = struct.unpack_from("<I", image, 0x3C)[0]
    if image[pe:pe + 4] != b"PE\0\0":
        raise ValueError("no PE header in the unpacked image")
    nsec = struct.unpack_from("<H", image, pe + 6)[0]
    opt_size = struct.unpack_from("<H", image, pe + 20)[0]
    sec = pe + 24 + opt_size
    size_of_headers = struct.unpack_from("<I", image, pe + 24 + 60)[0]
    sections = []
    end = size_of_headers
    for i in range(nsec):
        vsize, va, raw_size, raw_ptr = struct.unpack_from("<IIII", image, sec + 40 * i + 8)
        if raw_size:
            sections.append((va, raw_size, raw_ptr))
            end = max(end, raw_ptr + raw_size)
    out = bytearray(end)
    out[:size_of_headers] = image[:size_of_headers]
    for va, raw_size, raw_ptr in sections:
        chunk = image[va:va + raw_size]
        out[raw_ptr:raw_ptr + len(chunk)] = chunk
    return bytes(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--info", action="store_true", help="print the headers and exit")
    ap.add_argument("xex")
    ap.add_argument("out", nargs="?")
    a = ap.parse_args()
    x = Xex(open(a.xex, "rb").read())
    if a.info or not a.out:
        for k, v in x.info().items():
            print(f"{k:14} {v}")
        return
    img, key = x.image()
    with open(a.out, "wb") as f:
        f.write(img)
    print(f"{a.out}: {len(img)} bytes, key {key or 'none'}, sha256 {hashlib.sha256(img).hexdigest()}")


if __name__ == "__main__":
    sys.exit(main())
