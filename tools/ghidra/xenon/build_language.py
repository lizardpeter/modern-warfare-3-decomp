#!/usr/bin/env python3
"""Install the Xbox 360 (Xenon) PowerPC language into a Ghidra installation and compile it.

    python3 tools/ghidra/xenon/build_language.py <ghidra-install-dir>

Ghidra's PowerPC languages don't decode VMX128, and its closest language (PowerPC:BE:64:A2ALT-32addr, Power ISA
3.0 with AltiVec) has later instructions on opcode 4 that overlap VMX128's encodings (POWER9 maddhd/maddhdu/maddld,
SPE brinc, AltiVec added after ISA 2.03 such as vabsdub), so adding VMX128 to it would decode Xbox 360 code wrongly.

This builds PowerPC:BE:64:Xenon-32addr from the installed release's own files: ppc_common.sinc is copied with the
VMX128 fields (vmx128_fields.sinc) added to its instruction token, ppc_isa.sinc and altivec.sinc are copied with
every opcode-4 constructor removed whose mnemonic Xenon doesn't have (the list below, Xenon's AltiVec
set as Xenia decodes it) and every opcode-31 one on the Cell vector load/store XOPs (Book E's stvepx/stvepxl), and
ppc_embedded.sinc without dcbzl (the same encoding is Xenon's dcbz128), and ppc_64_32.cspec with a call fixup for
the register save/restore helpers (compiler_spec below), then tools/ghidra/xenon/vmx128.sinc adds VMX128 and the Cell/Xenon load/store additions.
Nothing of Ghidra's is stored in this repo; a Ghidra upgrade regenerates everything and the tests
(tools/ghidra/xenon/TestXenon.java, .github/workflows/xenon-language.yml) check it again.
"""
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
LANG_ID = "PowerPC:BE:64:Xenon-32addr"

# Xenon's opcode-4 AltiVec instructions (VMX, ISA 2.03), from Xenia's ppc-instructions.xml.
XENON_ALTIVEC = set("""
mfvscr mtvscr vaddcuw vaddfp vaddsbs vaddshs vaddsws vaddubm vaddubs vadduhm vadduhs vadduwm vadduws vand vandc
vavgsb vavgsh vavgsw vavgub vavguh vavguw vcfsx vcfux vcmpbfp vcmpeqfp vcmpequb vcmpequh vcmpequw vcmpgefp vcmpgtfp
vcmpgtsb vcmpgtsh vcmpgtsw vcmpgtub vcmpgtuh vcmpgtuw vctsxs vctuxs vexptefp vlogefp vmaddfp vmaxfp vmaxsb vmaxsh
vmaxsw vmaxub vmaxuh vmaxuw vmhaddshs vmhraddshs vminfp vminsb vminsh vminsw vminub vminuh vminuw vmladduhm vmrghb
vmrghh vmrghw vmrglb vmrglh vmrglw vmsummbm vmsumshm vmsumshs vmsumubm vmsumuhm vmsumuhs vmulesb vmulesh vmuleub
vmuleuh vmulosb vmulosh vmuloub vmulouh vnmsubfp vnor vor vperm vpkpx vpkshss vpkshus vpkswss vpkswus vpkuhum
vpkuhus vpkuwum vpkuwus vrefp vrfim vrfin vrfip vrfiz vrlb vrlh vrlw vrsqrtefp vsel vsl vslb vsldoi vslh vslo vslw
vspltb vsplth vspltisb vspltish vspltisw vspltw vsr vsrab vsrah vsraw vsrb vsrh vsro vsrw vsubcuw vsubfp vsubsbs
vsubshs vsubsws vsububm vsububs vsubuhm vsubuhs vsubuwm vsubuws vsum2sws vsum4sbs vsum4shs vsum4ubs vsumsws vupkhpx
vupkhsb vupkhsh vupklpx vupklsb vupklsh vxor
""".split())

# Opcode-31 extended opcodes Xenon uses for the Cell load/store vector left/right instructions (vmx128.sinc);
# Book E's stvepx/stvepxl sit on two of them
XENON_XOP31 = {519, 551, 647, 679, 775, 807, 903, 935}

CONSTRUCTOR = re.compile(r"^:([A-Za-z0-9_.]+)")
XOP31 = re.compile(r"\bOP=31\b.*\bXOP_1_10=(\d+)\b", re.S)
OPCODE_4 = re.compile(r"\bOP=4\b")


def constructors(text):
    """Split SLEIGH source into (is_constructor, mnemonic, chunk) pieces; a constructor runs from a line starting
    with ':' to the brace that closes its semantic section."""
    out, i, n = [], 0, len(text)
    while i < n:
        m = re.compile(r"^:", re.M).search(text, i)
        if not m:
            out.append((False, None, text[i:]))
            break
        if m.start() > i:
            out.append((False, None, text[i:m.start()]))
        open_ = text.index("{", m.start())
        unimpl = re.compile(r"\bunimpl\b").search(text, m.start(), open_)
        if unimpl:  # ':name ... unimpl' has no semantic section
            j = unimpl.end() - 1
            depth = None
        else:
            depth, j = 0, open_
        while depth is not None:
            c = text[j]
            if c == "{":
                depth += 1
            elif c == "}":
                depth -= 1
                if depth == 0:
                    break
            j += 1
        end = text.find("\n", j)
        end = n if end < 0 else end + 1
        chunk = text[m.start():end]
        out.append((True, CONSTRUCTOR.match(chunk).group(1).split("^")[0], chunk))
        i = end
    return out


def not_xenon(head, mnem, keep_op4, drop=()):
    """True for a constructor Xenon doesn't have: on opcode 4 and not kept, on one of the Cell XOPs, or named."""
    if mnem in drop:
        return True
    if OPCODE_4.search(head) and not keep_op4(mnem):
        return True
    m = XOP31.search(head)
    return bool(m and int(m.group(1)) in XENON_XOP31)


def filtered(src: Path, keep, drop=()) -> tuple[str, list[str]]:
    dropped, parts = [], []
    for is_ctor, mnem, chunk in constructors(src.read_text()):
        if not is_ctor:
            parts.append(chunk)
            continue
        head = chunk[:chunk.index("{")] if "{" in chunk else chunk
        if not_xenon(head, mnem, keep, drop):
            dropped.append(mnem)
            parts.append("# (Xenon) removed: " + head.strip().splitlines()[0] + "\n")
        else:
            parts.append(chunk)
    return "".join(parts), dropped


def save_restore_helpers():
    """The Xenon compiler's register save/restore helpers: __savegprlr_14.._31, __restgprlr_*, __savefpr_*,
    __restfpr_* (14-31), __savevmx_*, __restvmx_* (14-31 and 64-127), and each family's un-numbered entry."""
    names = []
    for family, regs in (("gprlr", range(14, 32)), ("fpr", range(14, 32)),
                         ("vmx", list(range(14, 32)) + list(range(64, 128)))):
        for op in ("save", "rest"):
            names.append(f"__{op}{family}")
            names += [f"__{op}{family}_{n}" for n in regs]
    return names


def compiler_spec(base):
    """Ghidra's ppc_64_32.cspec plus a call fixup for the register save/restore helpers. Each is a chain of entries
    that spill or reload callee-saved registers (and LR) to the caller's frame, called from nearly every prologue
    (mflr r12; bl __savegprlr_NN) and epilogue: as ordinary calls the decompiler takes them for functions that
    return a value (`iVar1 = __savegprlr_28(...)`) and loses the real data flow. As a no-op fixup they vanish from
    the pseudocode; the registers they touch are the callee-saved ones the function preserves anyway.
    __restgprlr_NN is reached by a tail branch and returns for its caller, which a tail call already models."""
    targets = "\n".join(f'    <target name="{n}"/>' for n in save_restore_helpers())
    fixup = f"""  <callfixup name="xenon_save_restore">
{targets}
    <pcode>
      <body><![CDATA[
      r12 = r12;
      ]]></body>
    </pcode>
  </callfixup>

"""
    end = "</compiler_spec>"
    if base.count(end) != 1:
        sys.exit("ppc_64_32.cspec: expected one </compiler_spec>")
    note = "<!-- Generated by tools/ghidra/xenon/build_language.py (modern-warfare-3-decomp) from ppc_64_32.cspec -->\n"
    decl = re.match(r"<\?xml[^>]*\?>\s*", base)  # the XML declaration must stay first
    head = decl.group(0) if decl else ""
    return head + note + base[len(head):].replace(end, fixup + end)


def main():
    ghidra = Path(sys.argv[1])
    lang = ghidra / "Ghidra" / "Processors" / "PowerPC" / "data" / "languages"
    isa, dropped_isa = filtered(lang / "ppc_isa.sinc", lambda m: False)  # no ISA opcode-4 instruction is Xenon's
    alt, dropped_alt = filtered(lang / "altivec.sinc", lambda m: m.rstrip(".") in XENON_ALTIVEC)
    # dcbz with bit 21 set: the PowerPC 970's dcbzl, Xenon's dcbz128 (vmx128.sinc)
    embedded, dropped_emb = filtered(lang / "ppc_embedded.sinc", lambda m: True, drop={"dcbzl"})
    (lang / "ppc_xenon_embedded.sinc").write_text(embedded)
    common = (lang / "ppc_common.sinc").read_text()
    if common.count('@include "ppc_embedded.sinc"') != 1:
        sys.exit("ppc_common.sinc: expected one include of ppc_embedded.sinc")
    common = common.replace('@include "ppc_embedded.sinc"', '@include "ppc_xenon_embedded.sinc"')
    token = "define token instr(32)\n"
    if common.count(token) != 1:
        sys.exit("ppc_common.sinc: expected one 'define token instr(32)'")
    fields = (HERE / "vmx128_fields.sinc").read_text()
    (lang / "ppc_xenon_common.sinc").write_text(common.replace(token, token + fields + "\n"))
    (lang / "ppc_xenon_isa.sinc").write_text(isa)
    (lang / "altivec_xenon.sinc").write_text(alt)
    (lang / "vmx128.sinc").write_text((HERE / "vmx128.sinc").read_text())
    (lang / "ppc_xenon_be.slaspec").write_text("""# Generated by tools/ghidra/xenon/build_language.py (modern-warfare-3-decomp): Xbox 360 Xenon
@define ENDIAN "big"
@define IS_ISA "1"
@define NoLegacyIntegerMultiplyAccumulate "1"
@define REGISTER_SIZE "8"
@define BIT_64 "64"
@define EATRUNC "ea"
@define CTR_OFFSET "32"

@include "ppc_xenon_common.sinc"
@include "ppc_xenon_isa.sinc"
@include "ppc_a2.sinc"
@include "quicciii.sinc"
@include "FPRC.sinc"
@include "altivec_xenon.sinc"
@include "vmx128.sinc"
""")
    (lang / "ppc_xenon.cspec").write_text(compiler_spec((lang / "ppc_64_32.cspec").read_text()))
    (lang / "xenon.ldefs").write_text(f"""<?xml version="1.0" encoding="UTF-8"?>
<!-- Generated by tools/ghidra/xenon/build_language.py (modern-warfare-3-decomp) -->
<language_definitions>
  <language processor="PowerPC"
            endian="big"
            size="64"
            variant="Xenon-32addr"
            version="1.0"
            slafile="ppc_xenon_be.sla"
            processorspec="ppc_64.pspec"
            manualindexfile="../manuals/PowerISA.idx"
            id="{LANG_ID}">
    <description>Xbox 360 Xenon: PowerPC 64-bit, 32-bit addressing, AltiVec (VMX) and VMX128</description>
    <truncate_space space="ram" size="4"/>
    <compiler name="default" spec="ppc_xenon.cspec" id="default"/>
    <external_name tool="DWARF.register.mapping.file" name="ppc.dwarf"/>
  </language>
</language_definitions>
""")
    print(f"ppc_isa.sinc: removed {len(dropped_isa)} constructors: {' '.join(sorted(set(dropped_isa)))}")
    print(f"ppc_embedded.sinc: removed {len(dropped_emb)} constructors: {' '.join(sorted(set(dropped_emb)))}")
    print(f"altivec.sinc: removed {len(dropped_alt)} opcode-4 constructors not on Xenon: "
          f"{' '.join(sorted(set(dropped_alt)))}")
    kept = {m for c, m, _ in constructors(alt) if c}
    missing = sorted(XENON_ALTIVEC - {m.rstrip(".") for m in kept})
    if missing:
        print(f"note: Xenon AltiVec mnemonics with no constructor in this Ghidra: {' '.join(missing)}")
    subprocess.run([str(ghidra / "support" / "sleigh"), "ppc_xenon_be.slaspec"], cwd=lang,
                   check=True)
    print(f"{LANG_ID}: {lang / 'ppc_xenon_be.sla'}")


if __name__ == "__main__":
    main()
