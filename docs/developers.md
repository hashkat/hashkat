[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# How Does #k@ Work?

## Simulation Workflow

The 'simulation workflow' concept map below gives a brief summary of how the #k@ program runs.

![Simulation Workflow](/img/simulation_workflow.jpg "Simulation Workflow")

As we can see, after configuring the **INFILE.yaml** input file, the input file is read into Python via **INFILE.py**. Python then produces a generated input file in the form of **INFILE.yaml-generated** for C++ to read in the **main.cpp** file. From there all the information in the input file is parsed into the **Analyzer** struct via **INFILE.yaml-generated** being parsed into **config_dynamic.cpp** while one of the following occur:

* if starting a new network, the initial agents are created in the **analyzer_main.cpp** file and the agent rates are pre-calculated and the cumulative rate function is calculated in the **analyzer_rates.cpp** file prior to the start of the network simulation; the simulation then runss with calculations and analysis of network simulation done in **io.cpp**; the simulation finishes and exits

* if continuing on from an existent network, the simulation will continue running with calculations and analysis done in the **io.cpp** file; the simulation finishes and exits 

## Code Map

The code map below outlines how rates work in #k@.

![Code Map](/img/code_map.jpg "Code Map")

## Build Tests

Build tests can be run in #k@ by running the *tests.sh* script. This script runs network simulations using the input files found in *tests/referencefiles/*, and compares the output of these files to what is expected using the *verify.py* script. If there are any discrepancies between the data of a particular output file and its corresponding reference data, that file for that particular test is printed to the screen.

## The Source Code



For the latest build of #k@, see the github repository at [github.com/hashkat/hashkat](https://github.com/hashkat/hashkat)
