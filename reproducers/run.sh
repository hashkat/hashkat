set -e
../build.sh
../build.sh -O
PYTHONPATH='.' python cricket/unittest/__main__.py
