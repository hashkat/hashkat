from hashkat_util import TestBase, hashkat_test, hashkat_dump_state, hashkat_dump_summary,  hashkat_dump_stats

###############################################################################
## Hash tag retweet test
###############################################################################

class Max_analysis_steps_is_honoured(TestBase):
    base_infile = "base_infiles/simple-base.yaml"
    def __init__(self, EXPECTED_STEPS):
        self.EXPECTED_STEPS = EXPECTED_STEPS
        self.steps = 0
    # Configure the base configuration
    def infile_setup(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = self.EXPECTED_STEPS
    def on_step_analysis(self):
        self.steps += 1
    def on_exit(self):
        stats = hashkat_dump_state(self.state)["stats"]
        # Test that we correctly exit at EXPECTED_STEPS, as specified by max_analysis_steps:
        assert stats["n_steps"] == self.steps
        assert stats["n_steps"] == self.EXPECTED_STEPS 

hashkat_test(Max_analysis_steps_is_honoured(EXPECTED_STEPS = 10))

