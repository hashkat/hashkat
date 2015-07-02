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
  $rootdir/run.sh
  ./verify.py
  # clean up after the tests
  rm DATA_vs_TIME INFILE.yaml-generated output/*.dat output/*.gexf 
  cd $rootdir
done
