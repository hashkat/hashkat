#!/bin/bash
set -e
../build.sh --make-lib
../build.sh -O --make-lib
PYTHONPATH='.' python cricket/unittest/__main__.py
