import unittest
import os
from hashkat_util import KnownFailingTest

class Unit_tests_should_pass(KnownFailingTest):
    def test_run(self):
        self.assertTrue(os.system('../run.sh --tests') == 0, "Not all cpp unit tests have passed!")
