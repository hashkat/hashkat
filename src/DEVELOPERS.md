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