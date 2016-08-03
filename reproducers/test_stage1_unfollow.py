import unittest
from collections import defaultdict
from hashkat_util import HashkatTestCase, hashkat_dump_state, hashkat_dump_summary,  hashkat_dump_stats

###############################################################################
## Does stage 1 unfollow cause problems with chatty agents and/or output?
###############################################################################

class Stage1_unfollow_should_not_create_issues(HashkatTestCase, unittest.TestCase):
    base_infile = "base_infiles/three-languages-stage1-unfollow.yaml"
    n_runs = 1
    EXPECTED_STEPS = 10
    def on_start_all(self):
        self.steps = 0
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = self.EXPECTED_STEPS
    def on_step_analysis(self):
        self.steps += 1
    def on_exit(self):
        stats = hashkat_dump_state(self.state)["stats"]
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
        self.assertEqual(stats["n_steps"], self.steps)
        self.assertEqual(stats["n_steps"], self.EXPECTED_STEPS )


