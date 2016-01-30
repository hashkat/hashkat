#!/bin/bash

# these tests test...?

#set -e
#./run.sh --tests $@

# where are we?

rootdir=`pwd`

# loop over all the reference tests and check whether they match expected output

for dir in tests/referencefiles/test??
do
  cd $dir
  echo -n "Running "
  echo $dir
  echo "This will take approximately 30 seconds"
  $rootdir/run.sh >& log
  cd output
  $rootdir/verify.py
  cd ..
  # clean up after the tests
  rm log INFILE.yaml-generated output/*.dat output/*.gexf output/DATA_vs_TIME
  cd $rootdir
done
