set -e # Good practice -- exit completely on any bad exit code

args="$@"

function is_mac() {
    if [ "$(uname)" == "Darwin" ]; then
        return 0 # True!
    else
        return 1 # False!
    fi
}

# Bash function to apply a color to a piece of text.
function colorify() {
    if is_mac ; then
        cat
    else
        local words;
        words=$(cat)
        echo -e "\e[$1m$words\e[0m"
    fi 
}

# Bash function to check for a flag in 'args' and remove it.
# Treats 'args' as one long string.
# Returns true if flag was removed.
function handle_flag() {
    flag=$1
    if [[ "$args" == *"$flag"* ]] ; then
        args="${args/$flag/}"
        return 0 # True!
    fi
    return 1 # False!
}

if [[ -e /proc/cpuinfo ]] ; then
    cores=$(grep -c ^processor /proc/cpuinfo)
else
    cores=4 # Guess -- may want to manually edit if above fails.
fi

# Create eclipse-project-files
if handle_flag "-e" ; then
    src=$(pwd)
    rm -f CMakeCache.txt
    mkdir ../KMCEclipse -p
    cd ../KMCEclipse
    # Eclipse project creation
    cmake -G"Eclipse CDT4 - Unix Makefiles" $src
    exit
fi

# Pass the -f flag to avoid building:
if ! handle_flag "-f" ; then
    cmake . | colorify '1;33'
    if handle_flag "--clean" ; then
        make clean
    fi
    make -j$((cores+1))
fi

if handle_flag "-g" ; then
    # Wrap the gdb around the program with -g:
    echo "Wrapping in GDB:" | colorify '1;35'
    gdb -silent -ex=r --args src/main $args
elif handle_flag "-l" ; then
    # Wrap lldb around the program with -g:
    echo "Wrapping in LLDB:" | colorify '1;35'
    lldb src/main $args
elif handle_flag "-p" ; then
    # Profile the program (requires oprofile) with -p:
    echo "Profiling with oprofile:" | colorify '1;35'
    operf --callgraph src/main $args
    opreport --callgraph --exclude-dependent --demangle=smart --symbols src/main | more
else
    # Normal execution
    src/main $args --handle-ctrlc
fi
