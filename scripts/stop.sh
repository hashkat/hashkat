set -e

# NOTE: Despite the use of 'kill', pkill and kill merely send signals to other programs.
pkill hashkat --signal 10
