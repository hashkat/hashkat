[hashkat.org](http://hashkat.org)

# Developers

If you are interested in contributing to ***#k@***, please check out the latest build of ***#k@*** at our github repository [here](https://github.com/hashkat/hashkat). If you wish to make any developments to ***#k@***, feel free to do so, and create a [pull request](https://help.github.com/articles/creating-a-pull-request/), where it will be reviewed and potentially merged in to the **hashkat** repository. We do ask when modifying ***#k@***, please respect the [Google C++ Style Guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html).

## How Does ***#k@*** Work?

The following delivers a brief overview of how ***#k@*** actually works.  

### Simulation Workflow

The 'simulation workflow' concept map below gives a brief summary of how ***#k@*** runs.

<center>
<img src='../img/developers/simulation_workflow.jpg'>
</center>

As we can see, after configuring the *INFILE.yaml* input file, the input file is read into Python via *hashkat_pre.py*. Python then produces a generated input file in the form of *INFILE.yaml-generated* for C++ to read in the *main.cpp* file. From there all the information in the input file is parsed into the 'Analyzer' struct via *INFILE.yaml-generated* being parsed into *config_dynamic.cpp* while one of the following occurs:

* if starting a new network, the initial agents are created in the *analyzer_main.cpp* file while the agent rates are pre-calculated and the cumulative rate function is calculated in the *analyzer_rates.cpp* file prior to the start of the network simulation; the simulation then runs, and calculations and analysis of the network simulation is done in *io.cpp*; the simulation then finishes and exits

* if continuing on from an existent network, the simulation will continue running with calculations and analysis done in the *io.cpp* file; the simulation then finishes and exits 

### Code Map

The code map below outlines how rates work in ***#k@***.

<center>
<img src='../img/developers/code_map.jpg'>
</center>

In the *analyzer_main.cpp* file, if the simulated time is less than the maximum simulated time and the real time that has occurred is less than the maximum real time, a random number, *r*<sub>1</sub>, is generated. If *r*<sub>1</sub> and the normalized *add* rate have a difference less than or equal to zero, a new agent is added to the network. If this is not the case, the value of *r*<sub>1</sub> is decreased by the normalized *add* rate. If *r*<sub>1</sub>'s new value and the normalized *follow* rate have a difference less than or equal to zero, an agent follows some other agent via *analyzer_follow.cpp*. If this is not the case, the value of *r*<sub>1</sub> is decreased by the normalized *follow* rate. If *r*<sub>1</sub>'s new value and the normalized *tweet* rate have a difference less than or equal to zero, an agent tweets. If this is not the case, the value of *r*<sub>1</sub> is decreased by the normalized *tweet* rate. The difference of *r*<sub>1</sub>'s new value and the normalized *retweet* rate must be less than or equal to zero, resulting in an agent retweeting via the *analyzer_retweet.cpp*.

Once this is completed, another random number, *r*<sub>2</sub> is generated. If *use_random_time_increment* is enabled, the simulated time moves forward by:

*t* += -ln(*r*<sub>2</sub>) / **R**

where **R** is the cumulative rate function, the sum of all the rates.

If *use_random_time_increment* is disabled, the simulated time moves forward by:

*t* += 1 / **R**

All the rates are then updated, and if the maximum simulated or real time has not been reached, this process repeats until either of these times are reached.

Another way to look at how these rates work in ***#k@*** is as follows.

We initially have one agent in the network, with the agent *add* rate having been set to 1 agent per simulated minute and the agent *tweet* rate being set to 1 tweet per simulated minute (we've ignored the *follow* rate and *retweet* rate for this example). At this moment, there is an even chance that another agent will be added to the network or the initial agent will make a tweet. A random number is generated. If the random number is less than or equal to 0.5, another agent will be added, if it is greater than 0.5, the initial agent will tweet. Time will then move forward based on the cumulative rate function and possibly another generated random number, again depending on whether or not 'use_random_time_increment' is permitted.

<center>
<img src='../img/developers/kmc_1.png'>
</center>

Let's say that an agent has been added to the network.

<center>
<img src='../img/developers/kmc_2.png'>
</center>

With this addition to our network, the agent *add* rate will remain the same but the *tweet* rate will now double since we have double the number of agents from what we originally had in the network. With this change, an agent tweeting is twice as likely to happen in comparison to an agent being added in the KMC loop, though the expected rates for each agent will remain the same since the simulated time will move more slowly. This is due to the change in time being inversely proportional to the steadily increasing cumulative rate function.

<center>
<img src='../img/developers/kmc_3.png'>
</center>

Let's add an additional agent to the network.

<center>
<img src='../img/developers/kmc_4.png'>
</center>

Just as before, the agent *add* rate will remain the same but the *tweet* rate will increase since we now have 3 times the number of agents we initially had, though the expected rates for each agent will remain the same.

<center>
<img src='../img/developers/kmc_5.png'>
</center>

This cycle will continue until the end of the simulation.

This is how the [**Kinetic Monte Carlo (KMC)**](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo) method works in ***#k@***. 

## The Source Code

The source code for ***#k@*** can be found in the **src** directory in the **hashkat** directory. Within there can be found *DEVELOPERS.md*, a file which briefly details the role of every file and directory in the source code.

## Build Tests

Build tests can be run in ***#k@*** by running the *tests.sh* script. This script runs network simulations using the input files found in **tests/referencefiles/**, and compares the output of these files to what is expected using the *verify.py* script. If there are any discrepancies between the data of a particular output file and its corresponding reference data, that file for that particular test is printed to the screen.