set -e
export SKIP_KNOWN_FAILURES=1 # Disable known-to-fail tests
export PYTHONDONTWRITEBYTECODE=1 # Turn off .pyc files, more annoying than useful
# Create the debug and release libraries necessary for running tests:
BUILD_FOLDER=debug_build ../build.sh --make-lib
BUILD_FOLDER=release_build ../build.sh -O --make-lib
# Run tests by discovering all the test classes in test_* files in this folder:
python -m unittest discover
