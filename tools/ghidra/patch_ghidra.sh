#!/usr/bin/env bash
# Apply tools/ghidra/patches/*.patch to an installed Ghidra release: fetch each patched source file at the release
# tag, apply the patch, compile it against the release's own jars and replace the classes in the module's jar.
#
#     tools/ghidra/patch_ghidra.sh <ghidra-install-dir> <release-tag, e.g. Ghidra_12.1.4_build>
#
# Each patch is a unified diff against Ghidra's source tree (paths a/Ghidra/<Area>/<Module>/src/main/java/...) and
# says in docs/xbox360.md what it fixes. Fails if a patch doesn't apply, so a Ghidra upgrade can't silently drop one.
set -euo pipefail
ghidra=$1 tag=$2
here=$(cd "$(dirname "$0")" && pwd)
work=$(mktemp -d)
cp_jars=$(find "$ghidra/Ghidra" -name '*.jar' | tr '\n' ':')
shopt -s nullglob
for patch in "$here"/patches/*.patch; do
  echo "== $(basename "$patch")"
  src="$work/src"; rm -rf "$src"; mkdir -p "$src"
  for f in $(grep -E '^\+\+\+ b/' "$patch" | sed 's|^+++ b/||'); do
    mkdir -p "$src/$(dirname "$f")"
    curl -fsSL "https://raw.githubusercontent.com/NationalSecurityAgency/ghidra/$tag/$f" -o "$src/$f"
  done
  (cd "$src" && patch -p1 --forward --fuzz=0 < "$patch")
  for f in $(grep -E '^\+\+\+ b/' "$patch" | sed 's|^+++ b/||'); do
    module_dir=$(echo "$f" | sed -E 's|^(Ghidra/[^/]+/[^/]+)/src/main/java/.*|\1|')
    module=$(basename "$module_dir")
    jar="$ghidra/$module_dir/lib/$module.jar"
    test -f "$jar" || { echo "no module jar $jar"; exit 1; }
    out="$work/classes-$module"; mkdir -p "$out"
    javac --release 21 -nowarn -cp "$cp_jars" -d "$out" "$src/$f"
    (cd "$out" && jar uf "$jar" .)
    echo "patched $jar: $(cd "$out" && find . -name '*.class' | wc -l) classes from $(basename "$f")"
  done
done
rm -rf "$work"
