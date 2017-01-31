import unittest
from collections import defaultdict 
from hashkat_util import HashkatTestCase, hashkat_dump_summary, hashkat_dump_stats, hashkat_dump_agents, KnownFailingTest
from stat_util import stdev

class Bilingual_agents_should_follow_monolingual_agents(HashkatTestCase, unittest.TestCase):
    ''' Reproducer for #176. Bilingual agents should be able to follow monolingual agents.'''

    base_infile = "base_infiles/two-regions-english-french-overlapping.yaml"
    n_runs = 1
    def on_start_all(self):
        self.follows = []
    def on_follow(self, id1, id2):
        self.follows.append( (id1,id2) )
    # Configure the base configuration
    def on_start(self, yaml):
        self.args = ["--seed", str(self.runs + 25)]
        yaml["analysis"]["max_time"] = 60000
    def on_exit(self):
        #print hashkat_dump_stats(self.state)["global_stats"]
        agents = hashkat_dump_agents(self.state)
        language_counts = {
            "LANG_ENGLISH": {
                "LANG_ENGLISH": 0,
                "LANG_FRENCH_AND_ENGLISH": 0
            }, 
            "LANG_FRENCH_AND_ENGLISH": {
                "LANG_ENGLISH": 0,
                "LANG_FRENCH_AND_ENGLISH": 0
            } 
        }
        for id1, id2 in self.follows:
            lang1 = agents[id1]["language"]
            lang2 = agents[id2]["language"]
            language_counts[lang1][lang2] += 1
        print "Language count statistics:"
        print language_counts
        self.assertTrue(language_counts["LANG_ENGLISH"]["LANG_ENGLISH"] > 0,
            "Monolingual English agents should be able to follow other monolingual English agents (most likely fundamental issue with the test!).")
        self.assertTrue(language_counts["LANG_FRENCH_AND_ENGLISH"]["LANG_ENGLISH"] > 0,
            "Bilingual agents should be able to follow monolingual English language agents.")
        self.assertTrue(language_counts["LANG_ENGLISH"]["LANG_FRENCH_AND_ENGLISH"] > 0,
            "Monolingual English language agents should be able to follow bilingual agents.")

if __name__ == "__main__":
    unittest.main()
