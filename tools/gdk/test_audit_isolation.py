import unittest
from types import SimpleNamespace as N
from audit_isolation import check_pe, is_gdk_module, wine_modules, wine_report


class IsolationTests(unittest.TestCase):
    def test_names(self):
        for name in (b'XGameRuntime.DLL', 'C:\\Windows\\System32\\GamingServices.dll',
                     'Microsoft.Xbox.Services.141.GDK.C.dll', 'xboxlive.dll'):
            self.assertTrue(is_gdk_module(name), name)
        for name in ('kernel32.dll', 'iw5_enhanced.dll', 'x3daudio1_7.dll', 'xgameruntime.lib'):
            self.assertFalse(is_gdk_module(name), name)

    def test_imports(self):
        pe = N(DIRECTORY_ENTRY_IMPORT=[N(dll=b'KERNEL32.dll')])
        self.assertEqual(check_pe(pe, 'test'), ['kernel32.dll'])
        for directory in ('DIRECTORY_ENTRY_IMPORT', 'DIRECTORY_ENTRY_DELAY_IMPORT'):
            pe = N(**{directory: [N(dll=b'XGameRuntime.dll')]})
            with self.assertRaisesRegex(AssertionError, 'xgameruntime.dll'):
                check_pe(pe, 'test')

    def test_wine(self):
        log = ['0024:trace:loaddll:build_module Loaded L"C:\\\\windows\\\\system32\\\\KERNEL32.dll" at 00000000: builtin',
               '0024:trace:loaddll:build_module Loaded L"C:\\\\game\\\\XGameRuntime.dll" at 00000000: native',
               '0024:trace:loaddll:build_module Loaded L"C:\\\\game\\\\XGameRuntime.dll" at 00000000: native',
               'error while looking for gamingservices.dll']
        self.assertEqual(wine_modules(log), {'kernel32.dll', 'xgameruntime.dll'})
        self.assertEqual(wine_report(log)[1], ['xgameruntime.dll'])
        self.assertIn('unverified', wine_report([])[0])


if __name__ == '__main__':
    unittest.main()
