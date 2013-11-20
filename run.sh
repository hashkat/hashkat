set -e
cmake .
cores=$(grep -c ^processor /proc/cpuinfo)
make -j$((cores+1))
./socialnodes/main $@
