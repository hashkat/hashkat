import unittest
from hashkat_util import HashkatTestCase, hashkat_dump_state, hashkat_dump_summary, hashkat_dump_stats, hashkat_dump_agents, KnownFailingTest

class Stagnant_network_exits_correctly(HashkatTestCase, unittest.TestCase):
    '''Test a stagnant network. No non-zero rates. Should exit immediately.'''

    base_infile = "base_infiles/simple-base.yaml"
    n_runs = 1
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = 2
        yaml["analysis"]["initial_agents"] = 0
        # Nullify all actions: 
        yaml["rates"]["add"] = {"function": "constant", "value": 0.0}
        for agent_type in yaml["agents"]: 
            for rate in agent_type["rates"]:
                agent_type["rates"][rate] = {"function": "constant", "value": 0}
        print "Note: A WARNING message is expected."
    def on_exit(self):
        # Test that a stagnant network does not result in creation events:
        self.assertTrue(hashkat_dump_stats(self.state)["n_agents"] == 0)
        # Test that a stagnant network does not result in step events at all:
        self.assertTrue(hashkat_dump_stats(self.state)["n_steps"] == 0)

class Pure_growth_network_grows_correctly(HashkatTestCase, unittest.TestCase):
    '''Test a pure-growth network. All users do is join. Should have no tweets, and grow one agent per step.'''

    base_infile = "base_infiles/simple-base.yaml"
    n_runs = 1
    AGENTS_TO_GROW = 1000
    # Configure the base configuration
    def on_start(self, yaml):
        yaml["analysis"]["max_analysis_steps"] = self.AGENTS_TO_GROW
        yaml["analysis"]["max_agents"] = self.AGENTS_TO_GROW
        yaml["analysis"]["initial_agents"] = 0
        yaml["rates"]["add"] = {"function": "constant", "value": 1.0}
        # Make user addition (joining) the only action.
        # Turn off tweeting.
        for agent_type in yaml["agents"]: 
            for rate in agent_type["rates"]:
                agent_type["rates"][rate] = {"function": "constant", "value": 0}
    def on_exit(self):
        stats = hashkat_dump_stats(self.state)
        print hashkat_dump_stats(self.state)
        # Test that a network with only addition rates grows 1 user per step: 
        self.assertTrue(int(stats["n_agents"]) == self.AGENTS_TO_GROW,
                "Should have the maximum amount of agents!")
        self.assertTrue(int(stats["global_stats"]["n_tweets"]) == 0,
                "Should not have any tweets!")
