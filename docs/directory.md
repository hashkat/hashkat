[hashkat.org](http://hashkat.org)

# The **#k@** Directory

The following gives a brief summary of every file and directory that you will immediately find in the **hashkat** directory.

<center>
<img src='../img/directory.png'>
</center>

### <span style="color:blue">build</span> 

Contains the files and folders created in the building of **#k@**.

### <span style="color:green">build.sh</span> 

Script that builds and runs **#k@**. 

`./build.sh -O`

The command **./build.sh** is followed by 'space dash capital oh'.


### CMakeLists.txt

File required to run **CMake** with the build of **#k@**.

### COPYING

Contains the GNU General Public License, Version 3, 29 June, 2007.

### DATA_vs_TIME

File created when running a network simulation. Details the simulated time that has elapsed in minutes, the real time that has passed in seconds, the number of agents present, the number of 'follows', 'tweets', 'retweets', and 'unfollows' that have occurred and the cumulative rate function at various points in the simulation.

### DEFAULT.yaml

Contains defaults for all the parameters configured in **INFILE.yaml**. If any of these variables are not configured in **INFILE.yaml**, then the default value assigned to them in this file will be used.

### <span style="color:blue">docs</span> 

Contains the pages and images used in the **#k@** documentation. 

Contains **~/hashkat/docs/tutorial_input_files** where one can find the network configuration files (the **INFILE.yaml** files) that were used in the tutorials.  

### <span style="color:hotpink">graph.svg</span>

Contains network graph created by running **visualize.py**.

### <span style="color:green">hashkat_pre.py</span>

Generates input parameters from **INFILE.yaml** and transfers them to **INFILE.yaml-generated**, which is used in the running of **#k@**.

### INFILE.yaml

Input file where configurations are made to create the network simulation of your design.

### INFILE.yaml-generated

File generated when **run.sh** is run. Used in the running of network simulations, it contains all the parameters and their respective configurations made in **INFILE.yaml**.

### INSTALL

Contains instructions on how to install **#k@** on Mac OS X and Linux.

### INTERACT.lua

File used in the running of Interactive Mode in **#k@**.

### mkdocs.yml

File used to organize the documentation of **#k@**.

### network_state.dat

Save file in which your network simulation information will be saved, if 'save_network_on_timeout' is enabled. If 'load_network_on_startup' and 'save_network_on_timeout' are enabled in **INFILE.yaml** and the simulation is paused midway through, the simulation will look for this file on re-start to load the existing network information. 

### <span style="color:blue">output</span>

Directory which contains all the analysis files created from running a network simulation.

### README.md

Summary of what can be found in the **hashkat** directory.

### <span style="color:green">run.sh</span>

Script that runs the **#k@** program. With this file and an **INFILE.yaml** file present in the current directory, type in the following command to run a **#k@** simulation:

`./run.sh`

### <span style="color:blue">scripts</span> 

Contains several scripts used in the running of **#k@**. 

### <span style="color:blue">src</span>

Contains the **#k@** source code.

### <span style="color:blue">tests</span>

Contains several basic tests used in the development of **#k@**. Its subdirectory **referencefiles** contains the various input files that are tested when running **tests.sh**.

### <span style="color:green">tests.sh</span> 

Script that runs **verify.py** on every single reference network simulation in **tests/referencefiles**. Used in the development of **#k@** to confirm if any changes made to the source code did or did not affect the output of a network simulation.

### <span style="color:green">verify.py</span> 

Script run in **tests.sh** to compare the output data from a network simulation to data expected. If there is discrepancy between an output file and its reference, the file name is printed to the screen. Used in the development of **#k@** to confirm if any changes made to the source code did or did not affect the output of a network simulation.   

### <span style="color:green">visualize.py</span>

Script that creates **graph.svg**, which contains a plot of the network created by running **#k@**.

### <span style="color:blue">yaml</span>

Contains files necessary to remove all 'PyYaml' and 'SciPy' requirements when building **#k@**.
