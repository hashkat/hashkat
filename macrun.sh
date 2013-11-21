set -e # Good practice -- exit completely on any bad exit code

args="$@"

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
    cmake .
    make -j4
fi

# Wrap the gdb around the program with -g:
if handle_flag "-g" ; then
    echo "Wrapping in GDB:"
    gdb -silent -ex=r --args socialnodes/main $@
else
    # Normal execution
    socialnodes/main $@
fi
