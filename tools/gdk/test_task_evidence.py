import unittest
from task_evidence import collect_edges, scheduling_catalogue
from summarize_rejections import summarize


class EvidenceTests(unittest.TestCase):
    def test_graph_is_deduplicated(self):
        graph = ["caller\tcallee\n", "140000010\t140000020\n",
                 "140000010\t140000020\n", "140000030\t140000020\n",
                 "140000040\t140000050\n", "bad\n"]
        self.assertEqual(collect_edges(graph, {0x140000020, 0x140000060}),
                         {0x140000020: {0x140000010, 0x140000030}, 0x140000060: set()})

    def test_scheduling_only(self):
        rows = [{"va": "10", "name": "XTaskQueueDispatch"},
                {"va": "20", "name": "XAsyncBegin"},
                {"va": "30", "name": "XUserAddAsync"},
                {"va": "40", "name": "XStoreQueryGameLicenseAsync"}]
        self.assertEqual(scheduling_catalogue(rows),
                         {16: "XTaskQueueDispatch", 32: "XAsyncBegin"})

    def test_trace_raw_counts_not_unique_calls(self):
        marker = "iw5_gdk_rejected iid_le=" + "00" * 16 + " class_le=" + "00" * 16
        report = summarize([marker, marker], {})
        self.assertIn("Raw rejection marker occurrences: 2", report)
        self.assertIn("Distinct interface/class pairs: 1", report)
        self.assertIn("not unique calls", report)


if __name__ == "__main__":
    unittest.main()
