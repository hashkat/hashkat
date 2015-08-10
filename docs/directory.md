[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The *hashkat* Directory

The following gives a brief summary of every file and directory that you will immediately find in the #k@ directory.

<p align='center'>
<img src='../img/directory.png'>
</p>

## <span style="color:blue">benchmarks</span>

Contains several input files used in the testing and development of #k@.

## <span style="color:blue">build</span> 

Contains the files and folders necessary to build **#k@**.

## <span style="color:green">build.sh</span> 

Script that builds and runs #k@. Typing in the command

`./build.sh -O --build`

will build #k@ without running it.

## CMakeLists.txt

File required to run 'CMake' with the build of **#k@**.

## COPYING

Contains the GNU General Public License, Version 3, 29 June, 2007..

## DATA_vs_TIME

File created when running a network simulation. Details the length of simulated time that has occurred in simulated minutes, the number of agents that are present, the number of follows, tweets, retweets, and unfollows that have occurred, the cumulative rate function, and the length of real time that had passed in seconds at various points in the network simulation.

## DEFAULT.yaml

Contains all the possible paramters that can be configured in INFILE.yaml with assigned values. If any of these variables are not present in *INFILE.yaml*, than the value assigned to them in this file will be used.

## <span style="color:blue">docs</span> 

Contains the pages and images used in the write-up of the #k@ documentation.

## <span style="color:blue">examples</span> 

Contains a list of directories, each with there own **INFILE.yaml** that can be used as an example of a particular network simulation.

## <span style="color:green">hashkat_pre.py</span>

Generates all inputted parameters made in INFILE.yaml to prepare the ideal network to be run using **#k@**.

## INFILE.yaml

Input file where configurations are made to design the network simulation of your choosing.

## INFILE.yaml-generated

File generated when *build.sh* or *run.sh* is run. Used in the running of network simulations, it is always replaced with an updated version when a new network simulation is being run.

## INSTALL

Contains instructions on how to install #k@ on Linux and Mac OS X.

## INTERACT.lua

File used in the running of Interactive Mode in **#k@**.

## mkdocs.yml

File used to organize the documentation of #k@.

## network_state.dat

Save file in which your network simulation information will be saved to if *save_network_on_timeout* is enabled. If a network simulation is paused midway through, it can only be continued if *load_network_on_startup* is enabled, in which case the simulation will look for this file to load the existing network information. 

## <span style="color:blue">output</span>

Directory which contains all the analysis files created from running a network simulation.

## README.md

Summary of what can be found in the *hashkat* directory.

## <span style="color:green">run.sh</span>

Script that runs the #k@ program.

## <span style="color:blue">scripts</span> 

Contains several scripts used in the running of **#k@**. 

## <span style="color:blue">src</span>

Contains the #k@ source code.

## <span style="color:blue">tests</span>

Contains several basic tests used in the development of #k@. Its subdirectory *referencefiles* contains the various input files that are tested when running *tests.sh*.

## <span style="color:green">tests.sh</span> 

Script that runs *verify.py* on every single reference network simulation in *tests/referencefiles/*. Used in the development of #k@ to confirm if any changes made to the source code did or did not affect impact the output of a netowkr simulation.

## <span style="color:blue">tutorials</span> 

Contains a directory corresponding to almost every tutorial found in the #k@ documentation. Each tutorial directory contains an INFILE.yaml file that will be created during its respective tutorial.

## <span style="color:green">verify.py</span> 

Script run in *tests.sh* to compare the data of output files in a network simulation to what is expected. If there is discrepancy between an output file and its reference, the file name is printed to the screen. Used in the development of #k@ to confirm if any changes made to the source code did or did not affect impact the output of a network simulation.   

## <span style="color:blue">yaml</span>

Contains files necessary to remove all 'PyYaml' and 'SciPy' requirements when running **#k@**.