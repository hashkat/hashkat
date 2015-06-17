[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Key Words

A brief description and explanation for all of the terminology used in this documentation and in hashkat.
##A

####add weight
**(type: double/float)** This variable declares the percentage of the certain agent type in the network. Before the simulation is done, a loop is placed over all the different agents declared, and the add variables are summed. The value of add
declared in each agent type will then be divided by the add sum. For example if agent type A has an add value of 75, and agent type B has an add value of 25, then the network will consist of 75% of agent type A, and 25% of agent type B.

####agent
Corresponds to a member of a social network

####'agents' section
**(type: n/a)** This variable should never be removed from the input file. This allows the code to read in all of the different agents declared. One can declare as many different
agents as one wants, just be sure to properly declare all of the variables covered below.

####add_rate
**(type: n/a)** This parameter allows one to adjust the rate at which users are being added into the network. To have different numbers of agents, you can adjust the add weights in the agents section of the input file. Also known as *global_add_rate*

####‘agent’ follow model
Causes agents to follow other agents based on title alone; the titles are set in the agents section of the input file. The probabilities to follow each agent can also be set in the weights portion of the agents section. The probabilities are normalized, so the probabilities should be set with respect to one another.

####agent_stats
**(type: boolean)** If set to true, additional calculations will be done to find the relationships between different agent groups and degree distributions for each agent.
For every agent set in the agents section of the input file, there will be a file created with the information mentioned previously. The output files created will take the form “name info.dat” where the name is the name created for the agent.

####'analysis' section
**(type: n/a)** This parameter should never be adjusted, when the software looks for the parameters listed under the analysis heading, it first looks to this variable. This defines to so called analysis section of the input file.

##B

####Barabasi-Albert Model
An algorith created by Albert-Laszlo Barabasi and Reka Albert to generate networks where agents with the greater number of followers have a higher probability of being followed. More information on this can be found [here](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model).

####bin_spacing
**(type: string)** The strings that can be used for the bin spacing variable are linear, quadratic, and cubic; other bin spaces can be implemented if necessary.
If linear, the bins for grouping agents will be spaced based on the increment, if quadratic, the bins for grouping agents will be spaced based on the increment squared, and if cubic, the bins for grouping agents will be spaced based
on the increment cubed.

##C

####'cd'
Command to change directories in the terminal. Entering `cd <directoryname>` will change into the named directory. Entering `cd ..` will have you change into the parent directory, which is the directory from which the directory you are currently in, is in. Just entering the command `cd` into the terminal will have you change into the home directory.

####command
What you type into the terminal to prompt the computer to do. Several fundamental commands include : `ls`, `touch`, `mkdir`, `cd`, `rm`, `rm -r`, `mv`, `cp`, `cp -r`, and `nano`.

####command line
Where you type in the commands you issue on the terminal.

####'config_static' section
**(type: n/a)** There are constant values that can be found in ‘config static.h’. Some of the static values can be changed in the input file for simplicity. An example of this is the humour bins described below.

####'cp'
The command to copy and paste a file to a different location on your computer through the terminal. You must include where the file is located and where you would like it scopy to be placed in the command. An example of copying a file would be `cp ~/hashkat/randomfile ~/hashkat/tutorials/`. This command would result in the randomfile being copied and pasted into the 'tutorials' directory from its parent directory 'hashkat'. Typing in `cp -r` is used instead of `cp` for copying and pasting directories.

####cummulative rate function (**R**)



##D

####DATA_vs_TIME



####degree
The number of connections an agent has. An agent with a degree of 300 is connected to 300 other agents.

####degree distribution



####degree_distribution_by_follow_model
**(type: boolean)** If set to true, the degree distribution by follow model will be outputted to “dd by follow model.dat".

####degree_distributions
**(type: boolean)** If set to true, the cumulative, in-degree, and out-degree distributions will be outputted to “cumulative distribution.dat”, “in-degree distribution.dat”, and “out-degree distribution.dat”.
The data in the files can be easily plotted with Gnuplot.

####density_function
**(type: n/a)** This density function is the function that describes how the retweet rates decay over time. Since the input file is intertwined with Python, you can actually declare mathematical functions with Python syntax as the density function.
An example is ‘2.45 / (x)**1.1’. This function is integrated with scipy and each discrete element of this function is divided by the total integral to normalize the function. This ensures that it is truly a probability density function.
The discrete elements of the function can be adjusted by the parameters mentioned below. The agents that have the chance to be retweeted are binned according to the values of the function, and as time progresses they switch bins to have smaller value.

####'different_ideology' tweets
**(type: n/a)** In the agents section, there are different types of tweets that agents can tweet. One of the tweet types is ‘ideological’ which is a tweet related to an agents ideology. When an agent tweets a ideological tweet, the agents that
follows the agent who tweeted may retweet the tweet depending on the tweet transmission value and density function. An example of the syntax for declaring a ideological tweet preference class is:

preference classes:

-name: Pref1

tweet transmission:

different ideology:

AgentType1: 0.1

AgentType2: 0.1

else: 0.1

As you can see from above, the agent types declared in the agents section are used in the preference classes. If you also declared an agent type ‘AgentType3’ then that agent type would fall under the ‘else’ set above.
You can see that the transmission probabilities for all of the agent types are the same.

####directory
Another name for 'folders', where files are stored.

##E

####edge
Corresponds to a connection between agents in visualizations.

####enable_interactive_mode
**(type: boolean)** While the simulation is running, you can press ‘ctrl-c’. If this variable is set to true, an interpreter will be activated and you can call functions in the interpreter.
If you press the ‘tab’ key, you can go through the built in functions. The interpreter was designed to allow one to manipulate the network. If you are in the interpreter, you can press ‘ctrl-c’ and the simulation will resume.
If you type ‘exit()’ while in the interpreter the simulation will terminate.

##F

####follow



####follow weight
**(type: double/float)** This variable sets the weights for following each agent type. Like the add variable, these weights are summed, and then each follow variable is divided by the total. This variable is not used if the follow model is
random or preferential, but is used for the other models. For example if agent type A has a follow variable of 85, and agent type B has a follow variable of 15, then
agent type A will be followed 85% of the time, and agent type B will be followed 15% of the time.

####follow rate
**(type: n/a)** Like the function parameter to add agents into the network, the functions that can be set for the follow rate are ‘constant’ and ‘linear’. The
usage for defining the follow rate is the exact same as the agent add rate. See link for further implementation details.

####follow model



####follow_model
**(type: string)** Currently, there are 6 follow methods implemented in the software; ‘random’, ‘agent’, ‘twitter_suggest’, ‘preferential_agent’, ‘hashtag’, and ‘twitter’; to use a follow model simply type in

follow_model: random

####follow_ranks
**(type: n/a)** This has the same functionality as the tweet and retweet ranks, except it is for the number of followers an agent has. The major difference between the follow
ranks and the tweet/retweet ranks is that the thresholds have weights associated with them for follow ranks. If you adjust these weights correctly, you can achieve the preferential attachment model.
The descriptions for how you can set your thresholds for tweets, retweets, and follows, as well as how to set the weights for follows is below.

####follow_reaction_prob
**(type: double)** If an agent is selected to act on a retweet and they are not following the original tweeter, then they can either retweet the retweet or follow the original tweeter. This probability set here (from 0 to 1) describes how often
an agent would follow rather than retweeting the retweet. They need to be set for every preference class. An example is below:

preference classes:

-name: Pref1

follow reaction prob: 0.5

This probability set above would allow for half follows and half retweeted retweets.

####followback_probability
**(type: double)** This determines the probability that the agent type will follow another agent back using the use flawed followback method.

####function
**(type: string)** The function for the add rate can either be constant or linear; other options can be coded into the software if necessary. If function is set to ‘constant’, then you must also declare the constant value by setting the ‘value’
parameter to a real constant > 0. An example of of constant add rate is as follows:

add: {function: constant, value: 1.0}

This will set the add rate to 1 agent per minute. If function is set to ‘linear’, then you must set 2 other variables, the ‘y intercept’ and ‘slope’ variables. An example of a linearly increasing add rate is as follows:

add: {function: linear, y_intercept: 1.0, slope: 0.1}

This will create an add user rate of the form:

R_add = 1.0 + n_months · 0.1

where n months is the number of months that have passed in the simulation. The number of months is determined by a constant approximate month of 30 · 60 · 24 minutes. This constant value can be changed in the software to achieve desired result.

##G

####Gephi
A visualization tool used heavily in this documentation. You can find more information on and download Gephi [here](http://gephi.github.io/).

####global_add_rate
**(type: n/a)** This parameter allows one to adjust the rate at which users are being added into the network. To have different numbers of agents, you can adjust the add weights in the agents section of the input file. Also known as *add_rate*

####gnuplot



##H

####hashkat
directory and command


####hashkat_pre.py



####hashtag



####‘hashtag’ follow model
A mechanism introduced to follow other agents based on hashtags. If the use hashtag probability parameter is non-zero, then agents will attach hashtags to their tweets.
Depending on the ideology and location of the agent, they are then placed into a dynamic array. If another user wants to follow via hashtag, they look to these dynamic arrays to find a specific agent that relates to
their hashtag preferences. These preferences can be set in the hashtag follow options section.

####hashtag_follow_options
**(type: n/a)** When agents tweet in the network, they can also attach hashtags to their tweets. When they do attach hashtags, they are binned according to their ideology and region. If another agent searches for a hashtag based on
their characteristics, they can follow agents with a specific ideology or location. They can either follow agents that have the same ideology and region as them or just ideology and different region. An example of these options is below:

agents:

-name: AgentType1

hashtag follow options:

care about region: true

care about ideology: true

From this example, the AgentType1 will only follow other agents that have the same ideology and region because they care about the region and ideology for the hashtag
follow method. If both are set to false, then the AgentType1 will follow agents with any ideology from any location.

####humour_bins
**(type: int)** To determine how a retweet is passed based on humour, there are different values dedicated to each humour bin which describes how a tweet will be passed from agent to agent based on humour.
The value for each bin is what ever value is set in the preference classes section for ‘humourous’ multiplied by the probability density function.

####'humourous' tweets
**(type: n/a)** In the agents section, there are different types of tweets that agents can tweet. One of the tweet types is ‘humourous’ which can be thought of as how often they tweet humourous tweets. When an agent tweets a humourous tweet,
the agents that follows the agent who tweeted may retweet the tweet depending on the tweet transmission value and density function. An example of the syntax for declaring a ideological tweet preference class is:

preference classes:

-name: Pref1

tweet transmission:

humourous:

AgentType1: 0.1

AgentType2: 0.1

else: 0.1

As you can see from above, the agent types declared in the agents section are used in the preference classes. If you also declared an agent type ‘AgentType3’ then that agent type would fall under the ‘else’ set above.
You can see that the transmission probabilities for all of the agent types are the same.

##I

####ignore_load_config_check
**(type: boolean)** If you try to load a network from a save file that does not have the same parameters as the input file and this parameter is set to false, then you will receive a message on the screen and the simulation will not start.
This message will tell you that you must set this parameter to true. Once you do this you can load the saved network and continue simulating that network with different parameters. An example is to generate a random graph and save it.
Then you can load this network with different parameters generating a different network.

####'ideology' section
**(type: n/a)** This section of the input file determines abstract characterizations of agents. The motivation behind it is political views on Twitter and how different or similar people based on political views would act on retweets or hashtags.
Make sure that the number of ideologies declared here is the same as ‘N BIN IDEOLOGIES’ variable in the “config static.h” header file. Once you declare ideologies you can then set the weights for each ideology in the
regions section of the input file. You can also set the weights of a certain agent type tweeting about their ideology in the agents section of the input file.

####ideology_weights
**(type: n/a)** After declaring ideologies which define a characteristic for an agent, you must set the weights for each ideology in the region section. For more information on ideologies, click here: ideologies.
Let’s say you have defined ideologies ‘Red’ and ‘Blue’. The correct syntax for setting the weighting of these ideologies is as follows:

regions:

-name: Canada

add weight: 5

ideology weights: {Red: 10, Blue: 10}

Here the weights for each ideology are also summed and divided by the sum to generate a probability. These probabilities are used when an agent is created to determine which ideology they are.

####increment
**(type: integer)** This variable sets the separation between adjacent thresholds for grouping agents. If bin spacing is linear, then the separation between bins is the increment, if quadratic, the separation between bins is the increment
squared, if cubic, the separation between bins is the increment cubed.

An example of linear thresholds is:

thresholds: {bin spacing: linear, min: 10, max: 100, increment: 10}

This will create thresholds 10, 20, 30,..., 100; the agents will be grouped according to
these thresholds.

####INFILE.yaml



####INFILE.yaml-generated



####initial_agents
**(type: integer)** This parameter is the number of initial agents in the simulation. The agent type and characterizations are determined before any time will pass in the simulation.

####initial_resolution
**(type: double)** This is the initial value for determining where the integral of the density function is evaluated at. If the x start value is 5 and this parameter is set to 1, then the first integral will be from 5 to 6.


####interactive mode



##J

##K

####Kinetic Analysis of Twitter (#k@)



####Kinetic Monte Carlo Method



##L

####language_weights
**(type: n/a)** After declaring languages in ‘config static.h’ which defines the language for the agent, you must set the weights for each language in the region section. Let’s say you have defined languages ‘English’, ‘French’, and ‘English+French’.
The correct syntax for setting the weighting of these languages is as follows:

regions:

-name: Canada

add_weight: 5

language_weights: {English: 10, French: 10, English+French: 10}

Here the weights for each language are also summed and divided by the sum to generate a probability. These probabilities are used when an agent is created to determine which language they speak.

####load_network_on_startup
**(type: boolean)** If set to true, all of the information of the network that was sent into the file declared for the save file will be loaded from when the simulation begins.

####'ls'
Command to list all the contents of a directory. Typing in the command `ls` will print to the screen all the files and directories within the directory that you are currently in. Typing in the command `ls randomdirectory` displays all the files and directories within the directory `randomdirectory`. 

##M

####main_statistics
**(type: boolean)** If set to true, there will be a text file entitled “main_stats.dat” that is generated after the simulation has finished. This file can be found in the ‘output’ directory. It provides some general statistics for the simulation performed.

####max
**(type: integer)** This variable sets the maximum threshold for grouping agents. If the agents exceed the maximum value, they will be grouped in a bin where the threshold can not be exceeded.

####max_agents
**(type: integer)** This parameter is the maximum number of agents that will be allowed in the simulation. Once the maximum number of agents has been reached in the simulation, the program will no longer add agents but will not terminate.

####max_real_time
**(type: integer)** This parameter is the maximum time allowed in real life. Similarly to the max time parameter, the simulation will terminate once the maximum amount of time has been reached.
Also, you can put ‘minute’, ‘hour’, ‘day’, or ‘year’, and the simulation will know the value in minutes.

####max_time
**(type: integer)** This parameter is the maximum time allowed in the simulation. The units of time in the simulation is minutes. The simulation will terminate once the maximum amount of time has been reached. 
Conveniently, you can put ‘minute’, ‘hour’, ‘day’, or ‘year’, and the simulation will know the value in minutes.

####mkdir
Command to create a directory in the terminal. Typing the command `mkdir <newdirectory>` will create the directory called 'newdirectory'.

####min
**(type: integer)** This variable sets the minimum threshold for grouping agents. If the agents have a value less than the minimum threshold, then no binning will occur.

####model_weights
**(type: n/a)** In the model weights, you can set the probability of calling each follow method. You can use arbitrary units to set the weighting for each follow method because the weights are summed and then normalized to find the probability
of calling each method. As an example, if you would like to call the follow methods equally you would set it as so:

model_weights{random: 0.20, preferential: 0.20, agent: 0.20, preferential agent: 0.20, hashtag: 0.20}

####'mv'
Command to move a file from one location to another using the terminal. This command can also be used to rename a file. Typing in the command `mv ~/hashkat/randomfile ~/hashkat/tutorials/` will move the file 'randomfile' from the 'hashkat' directory to its subdirectory 'tutorials'. Typing in the command `mv ~/hashkat/randomfile ~/hashkat/newrandomfile` will change the name of the file 'randomfile' to 'newrandomfile'.

##N

####name
**(type: string)** The names of agents is preference; they can be anything one wants. The agents included in the software are Standard, Celebrity, Bot, and Organization.

####naming_ideologies
**(type: string)** The names of your ideologies is completely up to you and the syntax for declaring ideologies is as such:

ideologies:

-name: Red

-name: Blue

You can see that I have set 2 ideologies, Red and Blue. The weights for each ideology must be set in the regions section of the input file.

####naming_preference_classes
**(type: string)** You can name your preference classes however you want. The names of your preference classes are needed in the regions section of the input file to determine the weights for an agent having a certain preference class.
An example is as follows:

preference classes:

-name: Pref1

####naming_regions
**(type: string)** The name of your region can be anything that consists of a string. The name is a necessary part of the region so you will get a warning if it is not set.

####*nan*
An acronym for 'not a number'. Appears in the display when running a simulation in the connections between regions chart when there are no agents present in a region, and therefore cannot be connected to agents in any region.

####'nano'
A very basic text editor. Typing in the command `nano randomfile` will allow you to edit 'randomfile' using the text editor 'nano'. There are numerous text editors available for use, but 'nano' is the one we will be using for this documentation.

####node
Corresponds to agents in visualizations.

##O

####output
Directory which contains all the analysis files created from running a network simulation. The files which are created and appear in the 'output' directory can be modified in 'INFILE.yaml'.

####'output' section
**(type: n/a)** This parameter should not be removed, when the simulation has completed, functions that do further analysis on the network can be controlled here, a few built in functions are shown below. This defines the output portion of the input file.

##P

####'plain' tweets
**(type: n/a)** In the agents section, there are different types of tweets that agents can tweet. One of the tweet types is ‘plain’ which is a generic tweet. When an agent tweets a plain tweet, the agents that follows the agent who tweeted may retweet
the tweet depending on the tweet transmission value and density function. An example of the syntax for declaring a plain tweet preference class is:

preference classes:

-name: Pref1

tweet transmission:

plain:

AgentType1: 0.1

AgentType2: 0.1

else: 0.1

As you can see from above, the agent types declared in the agents section are used in the preference classes. If you also declared an agent type ‘AgentType3’ then that agent type would fall under the ‘else’ set above.
You can see that the transmission probabilities for all of the agent types are the same.

####‘preferential_agent’ follow model
Is just the ‘twitter_suggest’ follow method nested in the ‘agent’ follow method. Firstly, a certain agent type will be selected, then based on the degrees of the agents within the certain agent type,
an agent will be selected to follow.

####preference class
**(type: n/a)** details how tweets are retweeted by other agents in the system

####preference_class_weights
**(type: n/a)** After declaring preference classes which describe how retweets pass from agent to agent, you must set the weights for each preference class in the region section. For more information on preference classes, click here: preference classes.
Let’s say you have defined preference classes ‘Pref1’ and ‘Pref2’. The correct syntax for setting the weighting of these preference classes is as follows:

regions:

-name: Canada

add weight: 5

preference class weights: {Pref1: 10, Pref2: 10}

Here the weights for each preference classes are also summed and divided by the sum to generate a probability. These probabilities are used when an agent is created to determine which preference class they use.

####'preference_classes' section
**(type: n/a)** This section of the input file is for the declaration of preference classes which describe how tweets are passed in the system. Here you must set the tweet transmission for different scenarios based on different agent characteristics

##Q

##R

####**R** (cumulative rate funtion)
aka cummulative rate function


####'random' follow model
Causes agents to follow other agents randomly; if the number of agents is set to constant and this method is on, you will achieve an Erdos-Renyi degree distribution (Poisson distribution).

####'ranks' section
**(type: n/a)** The section of the input file where the agent tweet, retweet, and follow ranks are defined and modified.

####rates
**(type: n/a)** Every agent must have rates set for tweeting and following. These rates will be updated after a month passes by in the simulation; as mentioned previously
the number of months in the simulation are determined by a constant value defined for a month = 30 · 60 · 24.

####'rates' section
**(type: n/a)** This parameter does not need to be changed nor removed. Used to point to the global add rate for the simulation. This defines the global rates part of the input file.

####region
**(type: n/a)** The geographical location of an agent, which is randomly selected with weights that can be set in a network ismulation.

####region_add_weight
**(type: double)** As mentioned above, when an agent is added into the network, we randomly generate where they are from. The weights set for each region declares the probability of an agent being from a specific region. The weights for
all of the regions are summed and then divided by the sum to create a probability. The units for the weights of each region should be the same to normalize them correctly. The syntax for declaring the add weight is as such:

regions:

-name: Canada

add weight: 5

-name: USA

add weight: 10

From these weights, two thirds of the population will be from USA and one third from Canada.

####'regions' section
**(type: n/a)** This section of the input file is for declaring different regions where the agents in the network are ‘from’. When an agent is created, where the agent is ‘from’ is
selected randomly with weights that can be set. Here you can set the weights for many different parameters, all of which is explained below.

####retweet



####retweet ranks
**(type: n/a)** This has the same functionality as the tweet ranks, except it is for retweets. See above description about tweet ranks for more information.

####retweet_visualization
**(type: boolean)** If set to true, there will be a Gephi file called “retweet_viz.gexf” that is produced after the simulation terminates. This file can be found in the ‘output’ directory. The graph produced is a visualization of the most retweeted
tweet in the network.

####'rm'
The command to remove a file through the terminal. Typing in the command `rm randomfile`, will remove the file named 'randomfile'. The command `rm -r` is used instead of `cp` for copying and pasting directories.

##S

####'same_ideology' tweets
**(type: n/a)** In the agents section, there are different types of tweets that agents can tweet. One of the tweet types is ‘ideological’ which is a tweet related to an agents ideology. When an agent tweets a ideological tweet, the agents that follows the
agent who tweeted may retweet the tweet depending on the tweet transmission value and density function. An example of the syntax for declaring a ideological tweet preference class is:

preference classes:

-name: Pref1

tweet transmission:

same ideology:

AgentType1: 0.1

AgentType2: 0.1

else: 0.1

As you can see from above, the agent types declared in the agents section are used in the preference classes. If you also declared an agent type ‘AgentType3’ then that agent type would fall under the ‘else’ set above.
You can see that the transmission probabilities for all of the agent types are the same.

####save_file
**(type: string)** This is the file where all of the information of the network will be stored if save network on timeout is set to true. The file name can be anything you like.
Also if load network on startup is set to true, the simulation will look for this file to load the existing network information.

####save_network_on_timeout
**(type: boolean)** If set to true, all of the information of the network is sent into the file declared for the save file when the simulation is terminated.

####simulation



####social network



####stage1_unfollow
**(type: boolean)** If set to true, agents can be flagged when followed based on their tweet rates. If the tweet rate of the newly followed agent is greater than twice the average tweet rate of the pre-existent agents you follow,
than the agent is placed into an array. This array is looked to when the unfollow function is called. This algorithm is supposed to encapsulate an unfollow method on Twitter.
In Twitter if your feed is being dominated by a user you may become annoyed and want to unfollow that user. This is the process that occurs in this unfollow method.

####stdout_basic
**(type: boolean)** If set to true, the number of months in the simulation will be outputted to the screen. Once all of the analysis in complete in the simulation, a message will also be printed to the screen.

####stdout_summary
**(type: boolean)** If set to true, the amount of time, number of agents, number of follows, number of tweets, number of retweets, total rate, and real time spent between each successive output is outputted to the screen.

##T

####terminal



####thresholds
**(type: n/a)** The thresholds parameter always take 4 variables; bin spacing, min, max, and increment.

####time_span
**(type: double)** This value determines when the density function will disappear allowing no more retweets or follow by retweets to occur. Since the function found experimentally approaches 0 quite slowly, it is convenient to define a time span for
the function to improve efficiency of the retweet algorithm. Like the max time parameter you can use convenient strings like ‘minute’, ‘hour’, ‘day’, ‘month’, or ‘year’. You can also use Python syntax and multiply numbers to these strings.

####'touch'
Command to create a file in the terminal. Typing the command `touch <newfile>` will create the file called 'newfile'.

####tweet



####'tweet' rate
**(type: n/a)** Again, like the function parameter to add agents into the network, the functions that can be set for the tweet rate are ‘constant’ and ‘linear’.
The usage for defining the tweet rate is the exact same as the agent add rate. See link for further implementation details.

####tweet_analysis
**(type: boolean)** If set to true, further analysis will be done to create a distribution similar to a degree distribution.
The tweet and retweet distributions come from how often the agents tweet in the network; they tell you the probability that a given agent would have tweeted or retweeted n times, and n is first column in the data files produced.
The files created are titled “tweet distro.dat” and “retweets distro.dat.”

####'tweet_observation' section
**(type: n/a)** This denotes the section of the input file that provides the decay function for the retweet rates. Experimentally, the retweet rate for a tweet decays over time by approximately 1/t where t is the time in minutes.

####tweet_ranks
**(type: n/a)** This variable must stay in the input file, it allows the code to determine the thresholds for the tweets in the simulation. The agents in the simulation will be categorized in the thresholds provided.
For example if a threshold is 10, then the agents with 10 or less tweets will be stored an a list. Once an agent in this list tweets more than 10 times, the agent will be moved into a bin with a higher threshold.

####tweet_transmission
**(type: n/a)** Here you must set the transmission probabilities for different situations which can be found below. The transmission probabilities are then multiplied by the density function which has been found experimentally to decrease over time.

####tweet_type
**(type: double)** When an agent tweets they can be different types of tweets they have. These tweet types are incorporated with the preference classes for retweeting.
There are four different tweet types currently implemented and they are ‘ideological’, ‘plain’, ‘musical’, and ‘humourous’. The weights associated with each tweet type can also be set along with the tweet type. These weights can be different for each agent type and
the weights are summed and each weight is divided by the sum to produce a probability for generating each tweet type. An example of how to set the tweet types is below:

agents:

-name: AgentType1

weights:

tweet type:

ideological: 1.0

plain: 1.0

musical: 1.0

humourous: 1.0

From above you can see that all of the weights are the same and therefore there would be the same amount of each tweet type for AgentType1.

####Twitter



####‘twitter’ follow model
A model that incorporates all of the above follow mechanisms. The weights for each mechanism can be set for each follow method. See model weights for more information.

####‘twitter_suggest’ follow model
Follows the preferential attachment model outlined by Barabasi and Albert. The basics behind the method is that the degree of the agent determines the probability of following the agent;
the greater the degree the greater the probability of the agent being followed. The thresholds and weights variables outlined in the follow ranks section determines how the degree of the agents are binned and the corresponding weights for each bin.
If you want to achieve the preferential attachment method similar to the use barabasi method you can set the thresholds to increment by 1,
with linear spacing from 0 to the max number of agents and set the weights to increment by 1 with linear spacing from 1 to the max number of agents.

##U

####unfollow



####unfollow_tweet_rate
**(type: double)** This tweet rate is not associated with the stage1 unfollow method described above. This is a more simplified unfollow algorithm and can be considered the stage 0 unfollow method.
When an agent tweets, we look to see how many tweets they have put out since the agent was created. If the tweet rate of the agent exceeds the value set here, then the agent will be randomly unfollowed by one of its followers.

####use_barabasi
**(type: boolean)** If true, a certain simulation method will be implemented that results in a network that models a scale-free network with a scale-free exponent of 3.
This method is very similar to the ‘barabasi.game’ in the igraph package found in R or Python. The follow model (follow model) in the simulation should be set to ‘twitter_suggest’ to achieve the desired result.
The simulation method is as follows; an agent is added into the network, and then follows another agent based on the degree of the agents in the current network. A higher degree results in a higher probability of being followed.
If set to false, the simulation will run normally.

####use_followback
**(type: boolean)** If set to true, there may be a follow back prior to a follow. When an agent A follows agent B, if agent B’s followback probability (which can be found in the agents section of the input file) is non-zero,
then agent B can follow agent A without moving forward in time. This is why it is called a ‘flawed followback’; in reality there would be some time prior to the initial follow where the followback would occur.

####use_hashtag_probability
**(type: double)** This is a probability (0.0-1.0) that determines how often agents attach hashtags into their tweets. If set to 0.0, then none of the tweets will have hashtags.
If set to 0.5, then half of the tweets will have hashtags. If 1.0, then all of the tweets will have hashtags.

####use_random_time_increment
**(type: boolean)** If true, the time in the simulation will be incremented at a non-constant rate; the increment of time is determined by

&Delta;t = -ln(u) / **R**

where *u* is a random number in the interval 0 < *u* ≤ 1, and **R** is the sum of the rates for the simulation. On average, the value of −ln(*u*) is unity, and therefore you can increment time by 1/**R** ; this is how time is incremented if set to false.

##V

####visualization



####visualize
**(type: boolean)** If set to true, the information from the network is outputted to 2 files, “network.dat” and “network.gexf”. The “network.dat” file consists of two columns; the first column is a list of the agent IDs in order, and the second column is the
agent ID’s for who the first column is following. For example if agent ID 1 is following agent ID 13, 14, and 19, then it would be outputted like:

  1 | 13 

  1 | 14 

  1 | 19 

The structure of this file can be referred to as an edge list. This file can easily be read into Python or R to perform any further analysis. The “network.gexf” file is an xml file that can be used to visualize the network in a program called Gephi.

##W

####weights
**(type: n/a)** In the *ranks* section, this follows the same procedure as thresholds, but instead of defining the thresholds for grouping agents, it defines the probability of selecting one of the groups of agents. In the *agents* sections, this variable allows the code to read in the add and follow variables for each agent, and should always be declared in the input file for every agent.

##X

####x_end
**(type: double)** This is the final value of the density function. Make sure that the value of the density function evaluated at this end value does not cause a discontinuity in the function.

####x_start
**(type: double)** This is the initial value of the density function. Make sure that the value of the density function evaluated at this starting value does not cause a discontinuity in the function.

##Y

##Z

