#!/bin/bash

# Default HASHKAT to '.', check if unset (using a BASHism):
if [ ! "$HASHKAT" ] ; then 
    export HASHKAT=$(dirname "${BASH_SOURCE[0]}") 
fi

# Good practice -- exit completely on any bad exit code:
set -e 

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
#  --tests: Run tests, including functionality equivalent tests (testing
# against reference files from previous runs), and unit tests.
#
#  TODO: A possible argument like eg --quick to only run unit tests (once
# we have them back in)
###############################################################################

#if handle_flag "--tests" ; then
#    "$HASHKAT/build/src/test" $args
#    exit
#fi

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
Misc options:
    Running ./scripts/stop.sh will send a SIGUSR1 (user-defined signal) to the binary ‘hashkat’, in effect resulting in graceful termination of the program. This can never result in immediate termination.
    --no-ctrlc, do not handle Ctrl+C (note, results in immediate termination with Ctrl+C)"
    exit
fi

# If the flag is NOT present, handle ctrl-c
if ! handle_flag "--no-ctrlc" ; then
    args="$args --handle-ctrlc"
fi

###############################################################################
# Generate INFILE.yaml from DEFAULT.yaml if it's missing.
# INFILE.yaml is under .gitignore to reflect its per-user usage.
###############################################################################

if [ ! -f INFILE.yaml ] ; then
    echo "WARNING: You have no INFILE.yaml, creating one from DEFAULT.yaml"
    cp "$HASHKAT/DEFAULT.yaml" "$HASHKAT/INFILE.yaml"
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

"$HASHKAT/build/src/hashkat" $args
