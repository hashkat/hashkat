import unittest
import os

class Unit_tests_should_pass(unittest.TestCase):
    def test_run(self):
        self.assertTrue(os.system('../run.sh --tests') == 0, "Not all cpp unit tests have passed!")
