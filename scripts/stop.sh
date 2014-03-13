set -e

# NOTE: Despite the use of 'kill', pkill and kill merely send signals to other programs.
pkill socialsim --signal 10
