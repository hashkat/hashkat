# The Source Code

Here we will give a brief description of all the files and directories that encompass the source code.

## dependencies

Contains various files needed for the running of ***#k@***.

## unit_tests

Tests that have been run throughout the construction of ***#k@***.

## util

Contains *HashedEdgeSet.h*, which uses the Google SparseHash data structure to represent following/follower sets, *SerializeBufferFileMock.h*, which enables Google SparseHash to write into the *network_state.dat* file, and *StatCalc.h*, which is used for computing standard deviation incrementally. 

## CMakeLists.txt

Used to compile the code.

## CategoryGrouper.h

Handles categorizing agents. Creates the data structures that enable categorizing agents into bins based on the number of tweets, retweets, and follows they have, and moves them into different bins if any of these tweet, retweet, or follow values change.

## CircularBuffer.h

Implements a data buffer that deletes its oldest element when its capacity is reached.

## DataReadWrite.h

Handles the reading and writing to the *network_state.dat* file.

## FollowerSet.cpp

Handles the organizing of each agent's list of followers based on region, language, etc. as well as this lists size and the addition or removal of a follower.

## FollowerSet.h

Header file for *FollowerSet.cpp*.

## FollowingSet.cpp

Handles the organizing of each agent's list of agent ids they are following based on region, language, etc. as well as this lists size and the addition or removal of an agent id they are following.

## FollowingSet.h

Header file for *FollowingSet.h*.

## RateTree.h

Used to store tweets.

## TimeDepBinner.h

Used for updating the retweets. Handles moving tweeters into different bins after certain amounts of time has elapsed, giving them less and less of a chance of having their tweet be retweeted.

## TweetBank.cpp

Updates the age of a tweet, and organizes tweets by when they occur, lowering the probability that they will be retweeted as time progresses.

## TweetBank.h

Takes the list of followers an agent has and organizes it in a way so that followers who retweet an agent are ones that are most similar to the agent personality-wise (e.g. they share the same ideology).

## agent.h

Contains the *Agent* struct which determines the characteristics and collects information on each and every agent in the network, such as their id number, agent type, number of tweets made, number of followers, etc., and ensures that this is written into the *network_state.dat* file. Also contains the **AgentType* struct, which determines the characteristics of each agent type and ensures that it is also written into *network_state.dat*. 

## analyzer.h

Stores all of the information in a network simulation.

## analyzer_follow.cpp

Deals with how agents follow and unfollow each other through the various follow and unfollow methods in ***#k@***.

## analyzer_main.cpp

The principle file in the source code, it contains the main KMC loop and prompts agents to be created, to tweet, or to retweet.

## analyzer_rates.cpp

Manages updating all the rates present in ***#k@*** and adding them together to find the cumulative rate function at every time step. This allows us to move forward in time correctly via the KMC Loop. The rates being updated are the add, tweet, retweet, and follow rates, as well as of course the cumulative rate function. If necessary, new simulated months are created in this file and rates are updated accordingly if any of them happen to change over time.

## analyzer_retweet.cpp

Manages all the retweets in a simulation, determines which agents will be retweeting a tweet, and updates a tweet's relevance and the retweet rate since they change over time.

## analyzer_select.cpp

Determines which agent is selected at every KMC step to make a tweet, retweet, etc. Ensures that the agent selection process is done so properly.

## config_dynamic.cpp

Reads the input file. Parses all the necessary information from the input file.

## config_dynamic.h

All the information obtained in *config_dynamic.cpp* is stored here in the *ParsedConfig* struct, which is then stored in the *analyzer.h* file.

## config_static.h 

File where certain fixed configurations of the network simulation are made, namely the tweet types and languages present in the simulation, as well as the maximum follow models, preference classes, agent types, regions, and ideologies present in the network.

## events.cpp

Contains functions which control the actions you can make in interactive mode.

## events.h

Header file used to allow other '.cpp' files to call the functions found in *events.cpp*.

## gexf.lua

File used to create the 'gexf' file format for use with interactive mode.

## interactive_mode.cpp

File used to construct and run interactive mode.

## interactive_mode.h

Header file corresponding to 'interactive_mode.cpp' allowing other files to call the 'start_interactive_mode(AnalysisState& state)' function.

## interactive_mode.lua

Used in the running of interactive mode. 

## io.cpp

Used for analysis at the completion of a network simulation. Makes calculations based on the data collected to create the output files

## io.h

Header file corresponding to *io.cpp* that enables other files to call particular functions from it.

## main.cpp

When running a network simulation, this is the file that will read in *INFILE.yaml*, and call *analyzer_main.cpp*. It also prints to the screen certain information about the network simulation such as the version of ***#k@*** you are using, the seed you will be using, and how long the analysis took in real time. 

## network.h

Contains the *Network* struct which places all the *Agent* structs into an array and several convenient network queries.

## tweets.h

Sets all the information stored within tweets.

## util.h

Contains helper functions. 