from collections import defaultdict
import unittest
from hashkat_util import HashkatTestCase, hashkat_dump_state, hashkat_dump_summary, hashkat_dump_stats, hashkat_dump_agents, KnownFailingTest
from memory_util import memory

class Memory_usage_over_time(unittest.TestCase):
    '''INFO ONLY: How much memory does a particular configuration consume?
        This assumes the size of Python objects created is insignificant.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    use_full_checks = False
    n_runs = 1
    def __init__(self):
        self.steps = 0
        print 'Starting memory usage:', (memory() / 1024 ** 2), 'MB'
    # Configure the base configuration
    def infile_setup(self, yaml):
        #print 'Base memory usage:', (memory() / 1024 ** 2), 'MB'
        self.base_memory = memory()
        self.analysis = analysis = yaml["analysis"]
        analysis["initial_agents"] = analysis["max_agents"] = 1000 
        analysis["max_time"] = 100000 * (self.runs + 1)
    def on_exit(self):
        print 'Increased memory usage:', (memory() - self.base_memory) / 1024 ** 2, 'MB'
        print 'Absolute:', (self.base_memory) / 1024 ** 2, 'MB'
        print 'Time:', self.analysis["max_time"]
        print 'Agents:', self.analysis["initial_agents"]

