#!/usr/bin/env python3
"""Sample the pinned Store client's match state during the isolated Wine smoke run."""
import os
import struct
import sys
from pathlib import Path

# iw5mp_ship 2.0.3.0, confirmed by CL_InitCGame, CL_FirstSnapshot and CL_SetCGameTime.
IMAGE_BASE = 0x140000000
CONNECTION_STATE = 0x140E1EBEC
SNAPSHOT_SERVER_TIME = 0x140E2208C
MAP_NAME = 0x140E22144


def main():
    process = Path("/proc") / str(int(sys.argv[1]))
    mappings = (process / "maps").read_text().splitlines()
    if not any(line.startswith(f"{IMAGE_BASE:x}-") and line.endswith("/iw5mp_ship.exe")
               for line in mappings):
        raise SystemExit("Wine has no Store executable mapping at the pinned image base")
    with (process / "mem").open("rb", buffering=0) as memory:
        state = struct.unpack("<I", os.pread(memory.fileno(), 4, CONNECTION_STATE))[0]
        server_time = struct.unpack("<i", os.pread(memory.fileno(), 4, SNAPSHOT_SERVER_TIME))[0]
        map_name = os.pread(memory.fileno(), 64, MAP_NAME).partition(b"\0")[0].decode("ascii", "replace")
    print(f"state={state} snapshot={server_time} map={map_name}")


if __name__ == "__main__":
    main()
