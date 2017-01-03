#!/bin/bash

# Default HASHKAT to script folder, check if unset (using a BASHism):
if [ x"$HASHKAT" = x ] ; then
    export HASHKAT=$(dirname "${BASH_SOURCE[0]}")
fi
# If current folder is unknown, default to current folder.
if [ x"$HASHKAT" = x ] ; then
    export HASHKAT='.'
fi

function realpath() {
    python -c 'import os, sys; print os.path.realpath(sys.argv[1])' $1
}
ABS_HASHKAT=$(realpath "$HASHKAT")

# Good practice -- exit completely on any bad exit code:
set -e 

###############################################################################
# Helper functions for conditionally coloring text.
###############################################################################

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

###############################################################################
# Bash function to check for a flag in 'args' and remove it.
# Treats 'args' as one long string. 
# Returns true if flag was removed.
###############################################################################

args="$@" # Create a mutable copy of the program arguments
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

###############################################################################
# Show help for run.sh options available.
#  --help/help: Show options available.
###############################################################################

if handle_flag "--help" || handle_flag "help" ; then
        echo "#KAT is a program for simulation of large social networks such as those found in Twitter.

To use, alter INFILE.yaml, consulting DEFAULT.yaml for options.
Alter INTERACT.lua to configure callbacks into the program, and interactive-mode behaviour.

Command line options for run.sh below:

Configuration options:
    --input <FILE, default ./INFILE.yaml>, load network configuration options from the given input file (options for this file not discussed here).

Build options:
    --eclipse (or -e), create Eclipse project files in sibling directory ‘../KMCEclipse’
    --run (or -R), run after building
 
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
    Running ./scripts/stop.sh will send a SIGUSR1 (user-defined signal) to the binary ‘hashkat’, in effect resulting in graceful termination of the program. This can never result in immediate termination.
    --no-ctrlc, do not handle Ctrl+C (note, results in immediate termination with Ctrl+C)"
    exit
fi

###############################################################################
# Generate INFILE.yaml from DEFAULT.yaml if it's missing.
# INFILE.yaml is under .gitignore to reflect its per-user usage.
###############################################################################

if [ ! -f "$HASHKAT/INFILE.yaml" ] ; then
    echo "WARNING: You have no INFILE.yaml, creating one from DEFAULT.yaml"
    cp "$HASHKAT/DEFAULT.yaml" "$HASHKAT/INFILE.yaml"
fi

###############################################################################
# Eclipse options
#   --eclipse/-e: Create eclipse project files
###############################################################################

# Create eclipse-project-files
if handle_flag "--eclipse" || handle_flag "-e" ; then
    src=$(pwd)
    rm -f CMakeCache.txt
    mkdir ../hashkat_project -p
    cd ../hashkat_project
    # Eclipse project creation
    cmake -G"Eclipse CDT4 - Unix Makefiles" $src
    exit
fi

###############################################################################
# Compiling and setting up runtime directory structure
###############################################################################

# Handle environment-variable setting convenience flags
# These are used to communicate with CMake
# Each flag has an optional shortform, use whichever is preferred.

if [ x"$BUILD_FOLDER" == x ] ; then
    build_folder="build"
else
    build_folder="$BUILD_FOLDER"
fi

if handle_flag "--optimize" || handle_flag "-O" ; then
    export BUILD_OPTIMIZE=1
elif ! handle_flag "--faster-debug" ; then
    # Extra debug build flags that hinder debug performance significantly but are good to have by default.
    # These can be disabled with --faster-debug.
    export BUILD_FLAGS="$BUILD_FLAGS -D_GLIBCXX_DEBUG"
fi
# Run address sanitizer. Substantial overhead. May require library support on some systems.
if handle_flag "--sanitize" || handle_flag "-S" ; then
    export BUILD_SANITIZE=1
fi
if handle_flag "--profile-gen" || handle_flag "--pgen" ; then
    export BUILD_OPTIMIZE=1
    export BUILD_PROF_GEN=1
fi
# Use --pgen, and then this flag, for optimal performance.
# Compared to the gain from debug => release, the gain is minor.
if handle_flag "--profile-use" || handle_flag "--puse" ; then
    export BUILD_OPTIMIZE=1
    export BUILD_PROF_USE=1
fi

# Switch to HASHKAT directory for build
cd "$HASHKAT"
# Configure version string
export HASHKAT_VERSION="`git describe --abbrev=0 2>/dev/null`b`git rev-list HEAD --count`"

# Configure amount of cores used
if [[ -e /proc/cpuinfo ]] ; then
    cores=$(grep -c ^processor /proc/cpuinfo)
else
    cores=4 # Guess -- may want to manually edit if above fails.
fi

# Install lua dependencies (lua-repl and lua-linenoise):
rm -rf .libs
mkdir -p ".libs"
cp -r "./src/dependencies/lua-repl/repl/" "./.libs/repl/"
cp "./src/dependencies/lua-repl/repl.lua" "./.libs/"
cp "./src/dependencies/lua-misc/json.lua" "./.libs/"
cp "./src/interactive_mode.lua" "./.libs/"
cp "./src/gexf.lua" "./.libs/"

# Ensure folder exists for output
mkdir -p output/

echo "Compiling hashkat version $HASHKAT_VERSION in \"$build_folder\""
mkdir -p "$build_folder"
pushd "$build_folder" > /dev/null
cmake "$ABS_HASHKAT" | colorify '1;33'
if handle_flag "--clean" ; then
    make clean
fi
if handle_flag "--make-lib" ; then
    make -j$((cores+1)) hashkat-lib
else
    make -j$((cores+1)) hashkat
fi
popd > /dev/null

if ! handle_flag "--run" && ! handle_flag "-R" ; then
    exit
fi

###############################################################################
# Generating YAML files by expansion of Python
#   --input <FILE>, the input file to use. Defaults to INFILE.yaml.
###############################################################################

# If Python dependencies are hard to acquire on the target system,
# the alternative is to copy a *yaml-generated file from a system that
# can generate it, and run with --no-generate.

if ! handle_flag "--no-generate" ; then
    # Clear all existing generated YAML files:
    rm -f *yaml-generated

    # We must generate INFILE-generated.yaml from INFILE.yaml
    if env python --version 2>&1 | grep 'Python 2\.' > /dev/null ; then
        env python "$HASHKAT/hashkat_pre.py" $args
    else
        echo "#KAT requires Python2.x to run."
        exit 1
    fi
fi

###############################################################################
# Running the program. Options exist for running within a gdb, lldb, oprofile, 
# or valgrind context.  
#   --run/-R: Run after building.
#   Use '--help' or 'help' for details.
###############################################################################

# If the flag is NOT present, handle ctrl-c
if ! handle_flag "--no-ctrlc" ; then
    args="$args --handle-ctrlc"
fi

if handle_flag "--stacktrace" ; then
    gdb -silent -batch \
           -ex "run" \
           -ex "bt" --args build/src/hashkat $args 2>&1
elif handle_flag "--gdb" || handle_flag "-g" ; then
    # Wrap the gdb around the program with -g:
    echo "Wrapping in GDB:" | colorify '1;35'
    gdb -silent  \
        -ex='set confirm off' \
        -ex="handle 31 nostop noprint pass" -ex=r --args build/src/hashkat $args 
#        -ex="handle SIGINT nostop noprint pass" \
elif handle_flag "--lldb" || handle_flag "-l" ; then
    # Wrap lldb around the program with -l:
    echo "Wrapping in LLDB:" | colorify '1;35'
    lldb build/src/hashkat 
elif handle_flag "--oprofile" || handle_flag "--oprof" ; then
    # Requires installation of oprofile!
    # Profile the program (requires oprofile) with -p:
    echo "Profiling with oprofile:" | colorify '1;35'
    operf build/src/hashkat $args
    opreport --exclude-dependent --demangle=smart --symbols build/src/hashkat | less
elif handle_flag "--valgrind" || handle_flag "--vprof"; then
    # Requires installation of valgrind and kCacheGrind!
    echo "Profiling with valgrind:" | colorify '1;35'

    function showOutput() {
        kcachegrind callgrind.out.*
        rm callgrind.out.*        
    }

    trap showOutput INT

    valgrind --tool=callgrind build/src/hashkat $args

    showOutput
else
    # Normal execution
    "$build_folder"/src/hashkat $args
fi
