set -e # Good practice -- exit completely on any bad exit code

args="$@"

# Bash function to apply a color to a piece of text.
function colorify {
    local words;
    words=$(cat)
    echo -e "\e[$1m$words\e[0m"
}

# Bash function to check for a flag in 'args' and remove it.
# Treats 'args' as one long string.
# Returns true if flag was removed.
function handle_flag() {
    flag=$1
    if [[ "$args" == *"$flag"* ]] ; then
        args="${args/flag/}"
        return 0 # True!
    fi
    return 1 # False!
}

# Pass the -f flag to avoid building:
if ! handle_flag "-f" ; then
    cmake . | colorify '1;33'
    cores=$(grep -c ^processor /proc/cpuinfo)
    make -j$((cores+1)) | colorify '1;32'
fi

# Wrap the gdb around the program with -g:
if handle_flag "-g" ; then
    echo "Wrapping in GDB:" | colorify '1;35'
    gdb -silent -ex=r --args socialnodes/main $@
else
    # Normal execution
    socialnodes/main $@
fi
