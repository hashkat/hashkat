#!/usr/bin/bash

echo "#KAT tests are running..."

cd ..

###############################################################################
# Test setup and helpers.
###############################################################################

rm -rf tests/logs
mkdir -p tests/logs

RUNS=0
FAILS=0

function run_infile_test() {
    ((RUNS++))
    LOG=tests/logs/$1.log
    if ! ./run.sh --input tests/$1/INFILE.yaml &> $LOG; then
        ((FAILS++))
        less $LOG
    fi
}
###############################################################################
# Run tests.
###############################################################################

run_infile_test 01_Small_Network

###############################################################################
# Report output.
###############################################################################

echo "#KAT tests completed."
if ((FAILS > 0)) ; then
    echo "FAILURE. Not all tests have passed! This should not happen."
    echo "There was $FAILS failures out of $RUNS tests."
    exit 1
else
    echo "Success! All $RUNS tests have passed."
    exit 0
fi
