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

if handle_flag "--help" || handle_flag "help" ; then
        echo "#KAT is a program for simulation of large social networks such as those found in Twitter.

To use, alter INFILE.yaml, consulting DEFAULT.yaml for options.

Command line options for run.sh below:

Configuration options:
    --input <FILE, default ./INFILE.yaml>, load network configuration options from the given input file (options for this file not discussed here).

Build options:
    --eclipse (or -e), create Eclipse project files in sibling directory ‘../KMCEclipse’
    --force (or -f), do not rebuild (can be used to run last without compilation error)

Optimization options:
    --optimize (or -O), build with optimization. Prevents debugging.
The following imply --optimize (ie, specifying --optimization will have no further effect):
    --profile-gen (or --pgen), run and generate data for profile-guided optimization (PGO)
    --profile-use (or --puse), run using PGO (provides best known performance)

Profiling (ie, performance analysis, not PGO as above) options:    
    --valgrind (or --vprof), requires valgrind and kCacheGrind. Visualizes per-function performance results in a GUI. Valgrind has substantial overhead.
    --oprofile (or --oprof), requires oprofile. Displays per-function performance results in text format. Oprofile has unnoticable overhead.

Debug options:
    --lldb (or -l), debug the program in ‘lldb’
    --gdb (or -g), debug the program in ‘gdb’
    --sanitize(or -S), build with Address Sanitizer
    --debug-std, use (slower) error-checking std data-structures for std classes like std::vector

Testing options:
    --tests, run unit tests from ./src/tests/*.cpp

Misc options:
    Running ./scripts/colorify.sh will semantically colour the output of run.sh.
    Running ./scripts/stop.sh will send a SIGUSR1 (user-defined signal) to the binary ‘socialsim’, in effect resulting in graceful termination of the program. This can never result in immediate termination.
    --no-ctrlc, do not handle Ctrl+C (note, results in immediate termination with Ctrl+C)"
    exit
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

# Install lua dependencies (lua-repl and lua-linenoise):
rm -rf .libs
mkdir -p ".libs"
cp -r "./src/dependencies/lua-repl/repl/" "./.libs/repl/"
cp "./src/dependencies/lua-repl/repl.lua" "./.libs/"
cp "./src/dependencies/lua-misc/json.lua" "./.libs/"
cp "./src/interactive_mode.lua" "./.libs/"
cp "./src/gexf.lua" "./.libs/"

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
    mkdir -p build
    cd build
    cmake .. | colorify '1;33'
    if handle_flag "--clean" ; then
        make clean
    fi
    make -j$((cores+1))
    cd ..
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

# Ensure folder exists for output
mkdir -p output/

if handle_flag "--gdb" || handle_flag "-g" ; then
    # Wrap the gdb around the program with -g:
    echo "Wrapping in GDB:" | colorify '1;35'
    gdb -silent  \
        -ex='set confirm off' \
        -ex="handle 31 nostop noprint pass" -ex=r --args build/src/socialsim $args 
#        -ex="handle SIGINT nostop noprint pass" \
elif handle_flag "--lldb" || handle_flag "-l" ; then
    # Wrap lldb around the program with -l:
    echo "Wrapping in LLDB:" | colorify '1;35'
    lldb build/src/socialsim 
elif handle_flag "--oprofile" || handle_flag "--oprof" ; then
    # Requires installation of oprofile!
    # Profile the program (requires oprofile) with -p:
    echo "Profiling with oprofile:" | colorify '1;35'
    operf build/src/socialsim $args
    opreport --exclude-dependent --demangle=smart --symbols build/src/socialsim | less
elif handle_flag "--valgrind" || handle_flag "--vprof"; then
    # Requires installation of valgrind and kCacheGrind!
    echo "Profiling with valgrind:" | colorify '1;35'

    function showOutput() {
        kcachegrind callgrind.out.*
        rm callgrind.out.*        
    }

    trap showOutput INT

    valgrind --tool=callgrind build/src/socialsim $args

    showOutput
else
    # Normal execution
    build/src/socialsim $args
fi
