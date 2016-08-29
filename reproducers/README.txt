
Here are integration tests for hashkat, using small Python scripts to drive the tests. 

The basic idea is that using common themes found in base_infiles/*.yaml, we create a new synthetic INFILE, and analysis can be done after as well as during execution.
The functions from src/capi.h are automatically exposed to hashkat_util.py, which provides a convenient interface to them. If arbitrary C++ code needs to be run during tests,
it can be exposed in this manner.

The basic structure of an integration test is as follows:
    - on_start_all():
        Called before any test runs are made. Can be used to initialize the test object.
    - on_start(): 
        Called before the start of a run for the current test. If n_runs = 1 (typical case), this is only called once.
        Modify the base infile with slightly different attributes, depending on the requirements of the test, 
        as well as the self.runs variable, which determines the current run. 
    - on_end():
        Called after a test run, while the AnalysisState object in hashkat is still available. Can be used to query the C++ code.
    - on_end_all():
        Called after output is created for all the test runs (note, by default only the last run's output/ folder will be available). 
        Can 

There are also events (on_tweet, on_retweet, on_add). For aid using these, find tests that use them and print out their arguments.

To run tests, use ./run.sh. You can graphically choose what tests to run. You do not need to re-open the runner if you modify tests (but you may need to for completely new tests).
If dependencies are missing, run ./install.sh.
