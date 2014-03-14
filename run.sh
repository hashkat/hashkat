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
function handle_flag(){
    flag=$1
    local new_args
    local got
    got=1 # False!
    for arg in $args ; do
        if [ $arg = $flag ] ; then
            args="${args/$flag/}"
            got=0 # True!
        else
            new_args="$new_args $arg"
        fi
    done
    args="$new_args"
    return $got # False!
}

if [[ -e /proc/cpuinfo ]] ; then
    cores=$(grep -c ^processor /proc/cpuinfo)
else
    cores=4 # Guess -- may want to manually edit if above fails.
fi

# Handle environment-variable setting convenience flags
# These are used to communicate with CMake
# Each flag has an optional shortform, use whichever is preferred.

if handle_flag "--optimize" || handle_flag "-O" ; then
    export BUILD_OPTIMIZE=1
fi
if handle_flag "--sanitize" || handle_flag "-S" ; then
    export BUILD_SANITIZE=1
fi
if handle_flag "--profile-gen" || handle_flag "--pgen" ; then
    export BUILD_OPTIMIZE=1
    export BUILD_PROF_GEN=1
fi
# Use --pgen, and then this flag, with, for optimal performance
if handle_flag "--profile-use" || handle_flag "--puse" ; then
    export BUILD_OPTIMIZE=1
    export BUILD_PROF_USE=1
fi
# Pick whether to use debug std data-structures for eg std::vector
if handle_flag "--debug-std" ; then
    export BUILD_FLAGS="$BUILD_FLAGS -D_GLIBCXX_DEBUG"
fi


# Create eclipse-project-files
if handle_flag "--eclipse" || handle_flag "-e" ; then
    src=$(pwd)
    rm -f CMakeCache.txt
    mkdir ../KMCEclipse -p
    cd ../KMCEclipse
    # Eclipse project creation
    cmake -G"Eclipse CDT4 - Unix Makefiles" $src
    exit
fi

# Pass the -f flag to avoid building:
if ! handle_flag "-f" && ! handle_flag "--force" ; then
    cmake . | colorify '1;33'
    if handle_flag "--clean" ; then
        make clean
    fi
    make -j$((cores+1))
fi

# Clear all existing generated YAML files:
rm -f *yaml-generated

# We must generate INFILE-generated.yaml from INFILE.yaml:
# NOTE: Requires python and the pyyaml package
python './INFILE.py' "$@"

# If the flag is NOT present
if ! handle_flag "--no-ctrlc" ; then
    args="$args --handle-ctrlc"
fi

if handle_flag "--gdb" || handle_flag "-g" ; then
    # Wrap the gdb around the program with -g:
    echo "Wrapping in GDB:" | colorify '1;35'
    gdb -silent  \
        -ex='set confirm off' \
        -ex="handle 31 nostop noprint pass" -ex=r --args src/socialsim $args 
#       -ex="handle SIGINT nostop noprint pass" \
elif handle_flag "--lldb" || handle_flag "-l" ; then
    # Wrap lldb around the program with -l:
    echo "Wrapping in LLDB:" | colorify '1;35'
    lldb src/socialsim $args
elif handle_flag "--oprofile" || handle_flag "--oprof" ; then
    # Requires installation of oprofile!
    # Profile the program (requires oprofile) with -p:
    echo "Profiling with oprofile:" | colorify '1;35'
    operf src/socialsim $args
    opreport --exclude-dependent --demangle=smart --symbols src/socialsim | less
elif handle_flag "--valgrind" || handle_flag "--vprof"; then
    # Requires installation of valgrind and kCacheGrind!
    echo "Profiling with valgrind:" | colorify '1;35'

    function showOutput() {
        kcachegrind callgrind.out.*
        rm callgrind.out.*        
    }

    trap showOutput INT

    valgrind --tool=callgrind src/socialsim $args

    showOutput
else
    # Normal execution
    src/socialsim $args
fi
