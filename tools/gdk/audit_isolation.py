"""Check direct/delay imports and Wine load observations for known GDK runtime DLLs.

This is a focused module audit, not proof of arbitrary dynamic loads, entitlement
behavior, or process-wide network isolation.
"""
import re
import sys

GDK_PREFIXES = (
    'xgameruntime', 'gamingservices', 'xboxservices', 'xboxlive',
    'microsoft.xbox.services',
)
WINE_LOAD = re.compile(r'\b(?:trace:)?loaddll:[^\s]*\s+Loaded\s+L?"([^"\r\n]+)"', re.I)


def module_name(value):
    if isinstance(value, bytes):
        value = value.decode('ascii', errors='replace')
    return value.replace('\\', '/').rsplit('/', 1)[-1].lower()


def is_gdk_module(value):
    name = module_name(value)
    return name.endswith('.dll') and name.startswith(GDK_PREFIXES)


def pe_modules(pe):
    return {module_name(entry.dll)
            for directory in ('DIRECTORY_ENTRY_IMPORT', 'DIRECTORY_ENTRY_DELAY_IMPORT')
            for entry in getattr(pe, directory, ())}


def check_pe(pe, label):
    modules = pe_modules(pe)
    blocked = sorted(name for name in modules if is_gdk_module(name))
    if blocked:
        raise AssertionError(f'{label}: unexpected gaming runtime import: {", ".join(blocked)}')
    return sorted(modules)


def wine_modules(lines):
    return {module_name(m.group(1)) for line in lines for m in WINE_LOAD.finditer(line)}


def wine_report(lines):
    modules = wine_modules(lines)
    blocked = sorted(name for name in modules if is_gdk_module(name))
    report = [f'Distinct DLL/module loads observed: {len(modules)}',
              'Known GDK runtime DLLs observed: ' + (', '.join(blocked) if blocked else 'none')]
    if not modules:
        report.append('No Wine module load records recognized; module isolation is unverified.')
    report.append('This observation is limited to the captured launch interval and known module names.')
    return '\n'.join(report), blocked


if __name__ == '__main__':
    with open(sys.argv[1], encoding='utf-8', errors='replace') as log:
        report, blocked = wine_report(log)
    print(report)
    sys.exit(bool(blocked))
