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

In the *analyzer_main.cpp* file, ff the simulated time is less than the maximum simulated time and the real time that has occurreed is less than the maximum real time, a random number, *r*<sub>1</sub>, is generated. If *r*<sub>1</sub> and the add rate have a difference less than zero, a new agent is added to the network. If this is not the case, the value of *r*<sub>1</sub> is decreased by the add rate. If *r*<sub>1</sub>'s new value and the follow rate have a difference less than zero, an agent follows some other agent via *analyzer_follow.cpp*. If this is not the case, the value of *r*<sub>1</sub> is decreased by the follow rate. If *r*<sub>1</sub>'s new value and the tweet rate have a difference less than zero, an agent tweets. If this is not the case, the value of *r*<sub>1</sub> is decreased by the tweet rate. If *r*<sub>1</sub>'s new value and the retweet rate hae a difference less than zero, an agent retweets via the *analyzer_retweet.cpp*. If this is not the case, the value of *r*<sub>1</sub> is decreased by the retweet rate.   

Once this is completed, another random number, *r*<sub>2</sub> is generated. If *use_random_tme_increment*, the simulated time moves forward by:

*t* += -ln(*r*<sub>2</sub>) / **R**

where **R** is the cumulative rate function, the sum of all the rates. All the rates are then updated, and if the maximum simulated or real time has not been reached, this process repeats.

## Build Tests

Build tests can be run in #k@ by running the *tests.sh* script. This script runs network simulations using the input files found in *tests/referencefiles/*, and compares the output of these files to what is expected using the *verify.py* script. If there are any discrepancies between the data of a particular output file and its corresponding reference data, that file for that particular test is printed to the screen.

## The Source Code

Here we will give a brief description of all the files and directories that encompass that source code.

### dependencies



### unit_tests

Tests that have been run throughout the construction of #k@.

### util



### CMakeLists.txt

Used to compile the code.

### CategoryGrouper.h

Handles categorizing agents. Creates the data structures enabling categorizing agents into bins based on the number of tweets, retweets, and follows they have, and moving them into different bins if any of these tweet, retweet, or follow values change. Allows you to move agent ids around within those bins.

### CircularBuffer.h

CHECK TO SEE IF STILL IN USE

### DataReadWrite.h

Handles all the reading and writing to the *network_state.dat* file.

### FollowerSet.cpp

Handles the organizing of each agent's list of followers based on region, language, etc. as well as its size and the addition or removal of a follower.

### FollowerSet.h



### FollowingSet.cpp

Handles the organizing of each agent's list of agent ids they are following based on region, language, etc. as well as its size and the addition or removal of an agent they are following.

### FollowingSet.h



### RateTree.h



### TimeDepBinner.h

Used for updating the retweets. Handles moving tweeters into a different bin after a certain amount of time has elapsed, giving them less of a chance of having that tweet be retweeted.

### TweetBank.cpp

Everyone who tweets is placed here. Determines the age of a tweet. Updates the rate for certain tweets. Tweets are binned by when they occur. After a certain amount of time, tweets are moved to the next bin with a lower transmission probability. obs_prob determines the probability that the tweet will be transmitted. TimeDepRateTree allows #k@ to do this.

### TweetBank.h

Includes google/spare_hash_set. This file takes the list of followers an agent has and organizes it in a way so that followers who retweet an agent are ones that are most similar to the agent personality-wise. Contains numerous functions to allow this to happen. Also enables visualiztions of tweets. 

### agent.h

Contains the *Agent* struct which determines the characteristics and collects information on each and every agent in the network, such as their id number, agent type, number of tweets made, number of followers, etc., and ensures that this is written into the *network_state.dat* file. Also contains the **AgentType* struct, which determines the characteristics of each agent type and ensures that it is also written into *network_state.dat*. 

### analyzer.h

Stores all of the information for a simulation in #k@.

### analyzer_follow.cpp



### analyzer_main.cpp

The principal file in the source code, it deals with many aspect of #k@. It ensures that when you stop a simulation using **Ctrl-c** the simulation halts gracefully, it contains the *Analyzer* struct which contains numerous other structs within #k@, and it has several initialization functions used to make the proper configurations based on the input file.

!!!!! 

### analyzer_rates.cpp

Manages updating all the rates present in #k@ and adding them together to find the cumulative rate function at every time step. This allows us to move forward in time correctly via the KMC Loop. The rates being updated are the add, tweet, retweet, and follow rates, as well as of course the cumulative rate function. If necessary, new simulated months are created in this file and rates are updated accordingly if any of them happen to change over time.

### analyzer_retweet.cpp

Manages all the retweets in a simulation. Analyzes retweet rate and how it changes over time. Keeps track of tweet observation and how it decreases over time. AnalyzerRetwweet struct has all this information passed to it. Retweets are update via calling functions in the tweetbank. Tweets are updated as well, in terms of their relevance. If enough time has passed where they are no longer relevant, checks if they need to be removed. total retweet rate is found here and is called by analyzer_rates. RetweetCHoice determines which agent will be retweeting a tweet, based on google sparse hash table. Ensures that the agents speak the same language, perhaps have the same ideology, etc.

### analyzer_select.cpp

Determines which agent is selected at every KMC step to make a tweet, retweet, etc. Ensures that the agent selection process is done so properly.

### config_dynamic.cpp

Reads the input file. Parses all the necessary information from the input file.

### config_dynamic.h

All the information obtained in *config_dynamic.cpp* is stored here in the *ParsedConfig* struct, which is then stored in the *analyzer.h* file.

### config_static.h 

File where certain fixed configurations of your network simulation are made. Here we can identify the tweet types and languages present in the system, as well as the standard output rate and the maximum humour bins, follow models, preference classes, agent types, regions, and ideolgies present in your network.

### events.cpp

Contains functions which control the actions you can make in interactive mode.

### events.h

Header file used to allow other '.cpp' files to call the functions found in *events.cpp*.

### gexf.lua



### interactive_mode.cpp

This sets up interactive mode, ensures everything is running properly, shows the menu. Connects the '.cpp' and '.lua' files together in order to print out different things in the interactive mode.

### interactive_mode.h

Header file corresponding to 'interactive_mode.cpp' allowing other files to call the 'start_interactive_mode(AnalysisState& state)' function.

### interactive_mode.lua



### io.cpp

Used for analysis at the completion of a network simulation. Makes calculations based on the data collected to create the output files

### io.h

Header file corresponding to 'io.cpp' enabling other files to call particular functions from 'io.cpp'.

### main.cpp

When running a network simulation, this is the file that will read in **INFILE.yaml**, and call *analyzer_main.cpp. It also prints to the screen certain information about the network simulation such as the version of #k@ you are using, the seed you will be using, and how long the analysis took in real time. 
### network.h

Contains the *Network* struct which places all the *Agent* struct into an array and enables several commands that #k@ users can use to gather specific information about the simulated network, such as if an id number exists in the network, how many followers does a specific id have, etc.

### tweets.h

Sets all the information stored within tweets.

### util.h

Contains helper functions. 

## Latest Build

For the latest build of #k@, see the github repository at [github.com/hashkat/hashkat](https://github.com/hashkat/hashkat).