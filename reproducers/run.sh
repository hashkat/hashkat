#!/bin/bash
set -e
export PYTHONDONTWRITEBYTECODE=1 # Turn off .pyc files, more annoying than useful
BUILD_FOLDER=debug_build ../build.sh --make-lib
BUILD_FOLDER=release_build ../build.sh -O --make-lib
PYTHONPATH='.' python2 cricket/unittest/__main__.py
