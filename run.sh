set -e
cmake .
make -j2
time ./main
