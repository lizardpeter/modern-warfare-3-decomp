#!/usr/bin/env python3
"""Fail closed unless original engine logs show client init and its active action.
A successful check establishes client activation, not rendering or input quality.
The active-action convention is from the Xbox reference and must be confirmed on Store.
"""
import re
import sys
from pathlib import Path

def active(text):
    init=re.search(r"(?m)^.*CL_InitCGame:\s*[0-9.]+\s+seconds\s*$",text)
    if not init:
        return False
    return bool(re.search(r"(?m)^\s*IW5_CI_ACTIVE\s*$",text[init.end():]))

def main():
    root=Path(sys.argv[1])
    files=sorted(root.glob("engine-console-*.log"))
    for f in files:
        if active(f.read_text(encoding="utf-8",errors="replace")):
            print(f"CLIENT ACTIVE: {f.name}; review screenshot/input before claiming playable")
            return 0
    print("MAP ENTRY NOT VERIFIED: no ordered client initialization and active-action evidence")
    print("Engine logs found:",len(files))
    return 1

if __name__=="__main__":
    sys.exit(main())
