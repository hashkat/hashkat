[hashkat.org](http://hashkat.org)

# Developers

If you are interested in contributing to **#k@**, please check out the latest build of **#k@** at our [github repository](https://github.com/hashkat/hashkat). If you wish to add any developments to **#k@**, feel free to do so, and create a [pull request](https://help.github.com/articles/creating-a-pull-request/), where it will be reviewed and potentially merged in to the **hashkat** repository. We do ask when modifying **#k@**, please respect the [Google C++ Stye Guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html).

The source code for **#k@** can be found in the **src** directory in the **hashkat** directory. The file **~/hashkat/src/DEVELOPERS.md** briefly details the role of every file and directory in the source code. 

## How Does **#k@** Work?  

The following is a brief overview of how **#k@** actually works.  

Four very important files in the source code are summarized below:

<center>
<img src='../img/developers/main_src_files.jpg'>
</center>

### Simulation Workflow

The concept map below gives a brief summary of how **#k@** runs.

<center>
<img src='../img/developers/simulation_workflow.jpg'>
</center>

The input file **INFILE.yaml** is configured, then **INFILE.yaml** is read into Python via **hashkat_pre.py**.  Python then generates an input file **INFILE.yaml-generated** for C++ to read in the **main.cpp** file.  From there the information from the input file is put into the 'Analyzer' structure by parsing **INFILE.yaml-generated** into **config_dynamic.cpp** while one of the following occurs:

* if starting a new network, the initial agents are created in **analyzer_main.cpp** while agent rates and the cumulative rate function are calculated in **analyzer_rates.cpp** prior to the start of the network simulation; the simulation then runs, and analysis of the network simulation is done in **io.cpp**.  The simulation then finishes and exits.

* if continuing an existing network, the simulation will continue running with analysis done in **io.cpp**. The simulation then finishes and exits. 

### Code Map

The rates in **#k@** are: 'add', 'follow', 'unfollow', 'tweet' and 'retweet'.  The code map below outlines how rates work.

<center>
<img src='../img/developers/code_map.jpg'>
</center>

In brief, in **analyzer_main.cpp**, if the simulated time and real time elapsed are less than maximum simulated and real time allotted, a random number, **r**, is generated. The first is **r** is **r<sub>1</sub>**.

*  If **r<sub>1</sub>** and the normalized 'add' rate have a difference less than or equal to zero, a new agent is 'added' to the network. 

*  If this is not the case, the value of **r<sub>1</sub>** is decreased by the normalized 'add' rate. 

*  If **r<sub>1</sub>**'s new value and the normalized 'follow' rate have a difference less than or equal to zero, an agent 'follows' another agent via **analyzer_follow.cpp**. 

*  If this is not the case, the value of **r<sub>1</sub>** is decreased by the normalized 'follow' rate. 

*  If **r<sub>1</sub>**'s new value and the normalized 'tweet' rate have a difference less than or equal to zero, an agent 'tweets'. 

*  If this is not the case, the value of **r<sub>1</sub>** is decreased by the normalized 'tweet' rate. 

*  If **r<sub>1</sub>**'s new value and the normalized 'retweet' rate have a difference less than or must be less than or equal to zero, an agent 'retweets' via **analyzer_retweet.cpp**.

*  If **use_random_time_increment** is enabled, the simulated time moves forward by:

	**t += -log(r<sub>2</sub>) / R**

*  If **use_random_time_increment** is disabled, the simulated time moves forward by:

	**t += 1 / R**

   where **R** is the cumulative rate function, the sum of all the rates.  

*  Once this is completed, another random number, **r<sub>2</sub>** is generated. 

The process repeats until the maximum real or simulated time is reached.

## How Do **#k@** Rates Work?    

Another way to look at how rates work in **#k@** is as follows.  We initially have one agent (**n = 1**) in the network, with agent 'add' rate set to 1 agent per simulated minute (**R<sub>add</sub> = 1**) and agent 'tweet' rate set to 1 tweet per simulated minute (**R<sub>tweet</sub> = 1**).  We've ignored the 'follow' and 'retweet' rates in this example. 

<center>
<img src='../img/developers/kmc_1.png'>
</center>

* At start there is a 1/2 chance that another agent will be added to the network and 1/2 chance the initial agent will make a tweet. A random number is generated. If the random number is less than or equal to 0.5, another agent will be added, if it is greater than 0.5, the initial agent will tweet. Time will then move forward.

<center>
<img src='../img/developers/kmc_2.png'>
</center>

* Let's say that another agent has been added to the network (**n = 2**). The chance of 'add' becomes 1/3 and chance of a tweet becomes 2/3. The chance of an agent tweeting is now twice as likely as a new agent being added in the KMC loop. It should be noted that the expected output of each agent will remain the same since the simulated time will move more slowly due to time being inversely proportional to the increasing cumulative rate function (**R**).

<center>
<img src='../img/developers/kmc_3.png'>
</center>

* Let's add an additional agent to the network (**n = 3**). There is now a 1/4 chance of an 'add' and a 3/4 chance of a 'tweet'.

<center>
<img src='../img/developers/kmc_4.png'>
</center>

This cycle will continue until real or simulated time elapsed reaches the maximum alloted.  

<center>
<img src='../img/developers/kmc_5.png'>
</center>

This is the [**kinetic Monte Carlo (KMC)**](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo) method in **#k@**. 

## Build Tests  

Build tests can be run in **#k@** by running the **tests.sh** script. This script runs network simulations using the input files found in **tests/referencefiles/**, and compares the output of these files to what is expected using the **verify.py** script. If there are any discrepancies between the data of a particular output file and its corresponding reference data, the file for that particular test is printed to the screen.
