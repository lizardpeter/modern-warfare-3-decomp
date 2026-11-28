import unittest
from map_entry_gate import active
class GateTests(unittest.TestCase):
    def test_menu(self):
        self.assertFalse(active("enhanced build active\nGame still running after PLAY observation\n"))
    def test_partial(self):
        self.assertFalse(active("CL_InitCGame: 1.23 seconds\n"))
        self.assertFalse(active("IW5_CI_ACTIVE\nCL_InitCGame: 1.23 seconds\n"))
        self.assertFalse(active("CL_InitCGame: 1.23 seconds\nset cl_activeAction echo IW5_CI_ACTIVE\n"))
    def test_ordered(self):
        self.assertTrue(active("CL_InitCGame: 1.23 seconds\nIW5_CI_ACTIVE\n"))
if __name__=="__main__":
    unittest.main()
