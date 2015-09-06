[hashkat.org](http://hashkat.org)

# The **hashkat** Directory

The following gives a brief summary of every file and directory that you will immediately find in the **hashkat** directory.

<center>
<img src='../img/directory.png'>
</center>

## <span style="color:blue">build</span> 

Contains the files and folders created in the building of ***#k@***.

## <span style="color:green">build.sh</span> 

Script that builds and runs ***#k@***. Typing in the command

`./build.sh -O`

will build ***#k@***.

## CMakeLists.txt

File required to run 'CMake' with the build of ***#k@***.

## COPYING

Contains the GNU General Public License, Version 3, 29 June, 2007.

## DATA_vs_TIME

File created when running a network simulation. Details the length of simulated time that has occurred in simulated minutes, the number of agents that are present, the number of follows, tweets, retweets, and unfollows that have occurred, the cumulative rate function, and the length of real time that has passed in seconds at various points in the network simulation.

## DEFAULT.yaml

Contains all the possible paramters that can be configured in *INFILE.yaml* with assigned values. If any of these variables are not present in *INFILE.yaml*, than the value assigned to them in this file will be used.

## <span style="color:blue">docs</span> 

Contains the pages and images used in the write-up of the ***#k@*** documentation. In its subdirectory **tutorial_input_files**, one can find a directory corresponding to almost every tutorial found in the ***#k@*** documentation, where the *INFILE.yaml* file that will be created in each respective tutorial can be found.

## <span style="color:hotpink">graph.svg</span>

Contains network graph created by running *visualize.py*.

## <span style="color:green">hashkat_pre.py</span>

Generates all inputted parameters made in *INFILE.yaml* to *INFILE.yaml-generated*, which is used in the running of ***#k@***.

## INFILE.yaml

Input file where configurations are made to create the network simulation of your design.

## INFILE.yaml-generated

File generated when *run.sh* is run. Used in the running of network simulations, it contains all the parameters and their respective configurations made in *INFILE.yaml*.

## INSTALL

Contains instructions on how to install ***#k@*** on Mac OS X and Linux.

## INTERACT.lua

File used in the running of Interactive Mode in ***#k@***.

## mkdocs.yml

File used to organize the documentation of ***#k@***.

## network_state.dat

Save file in which your network simulation information will be saved to if 'save_network_on_timeout' is enabled. If a network simulation is paused midway through, it can only be continued if *load_network_on_startup* is enabled, in which case the simulation will look for this file to load the existing network information. 

## <span style="color:blue">output</span>

Directory which contains all the analysis files created from running a network simulation.

## README.md

Summary of what can be found in the **hashkat** directory.

## <span style="color:green">run.sh</span>

Script that runs the ***#k@*** program. With this file and an *INFILE.yaml* file present in the current directory, type in the following command to run ***#k@***:

`./run.sh`

## <span style="color:blue">scripts</span> 

Contains several scripts used in the running of ***#k@***. 

## <span style="color:blue">src</span>

Contains the ***#k@*** source code.

## <span style="color:blue">tests</span>

Contains several basic tests used in the development of ***#k@***. Its subdirectory **referencefiles** contains the various input files that are tested when running *tests.sh*.

## <span style="color:green">tests.sh</span> 

Script that runs *verify.py* on every single reference network simulation in **tests/referencefiles**. Used in the development of ***#k@*** to confirm if any changes made to the source code did or did not affect the output of a network simulation.

## <span style="color:green">verify.py</span> 

Script run in *tests.sh* to compare the data of output files in a network simulation to what is expected. If there is discrepancy between an output file and its reference, the file name is printed to the screen. Used in the development of ***#k@*** to confirm if any changes made to the source code did or did not affect the output of a network simulation.   

## <span style="color:green">visualize.py</span>

Script that creates *graph.svg*, which contains a plot of the network created by running ***#k@***.

## <span style="color:blue">yaml</span>

Contains files necessary to remove all 'PyYaml' and 'SciPy' requirements when building ***#k@***.
