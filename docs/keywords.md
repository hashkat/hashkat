[hashkat.org](http://hashkat.org)

# Key Words

A brief description and explanation for all of the terminology used in this documentation and in hashkat.

##A

####agent
Corresponds to a member of a social network

####‘agent’ follow model
Causes agents to follow other agents based on title alone; the titles are set in the agents section of the input file. The probabilities to follow each agent can also be set in the weights portion of the agents section. The probabilities are normalized, so the probabilities should be set with respect to one another.

##B

####Barabasi-Albert Model
An algorith created by Albert-Laszlo Barabasi and Reka Albert to generate networks where agents with the greater number of followers have a higher probability of being followed. More information on this can be found [here](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model).

##C

####cummulative rate function (**R**)
The sum of the agent add rate, tweet rate, follow rate, and retweet rate.

##D

####degree
The number of connections an agent has. An agent with a degree of 300 is connected to 300 other agents.

####degree distribution
The arrangement of the number of connections that every agent has within the simulation.

##E

####edge
Corresponds to a connection between agents in visualizations.

##F

####follow
The act of an agent connecting with another agent.

####follow model
A framework inputted into the simulation that dictates how agents generally connect with one another. There are currently 6 follow models within ***#k@***: *random*, *twitter_suggest*, *agent*, *preferential_agent*, *hashtag*, and *twitter*.

##G

####Gephi
A visualization tool used heavily in this documentation. You can find more information on and download Gephi [here](http://gephi.github.io/).

#### Gnuplot
A command-line driven utility for graphing used in tutorials for the plotting of data. More information on Gnuplot can be found [here](http://gnuplot.sourceforge.net/).

##H

####hashkat
The 'hashkat' directory contains all the filess and directories necessary to run ***#k@***.

####hashtag
Used in tweets, and marked by the # symbol, to classify messages by keyword or topic.

####‘hashtag’ follow model
A mechanism introduced to follow other agents based on hashtags. If the use hashtag probability parameter is non-zero, then agents will attach hashtags to their tweets.
Depending on the ideology and location of the agent, they are then placed into a dynamic array. If another user wants to follow via hashtag, they look to these dynamic arrays to find a specific agent that relates to
their hashtag preferences. These preferences can be set in the *hashtag* follow options section.

##I

####INFILE.yaml
The input file which users can modify to design the network simulation of their liking.

####interactive mode
A feature of ***#k@*** that allows users to pause the simulation midway through and modify the network by adding an agent, causing one agent to follow another agent, etc.

##J

##K

####Kinetic Monte Carlo Method
An algorithm designed to carry out a simulation where the rate of which time is increased by is influenced by the cummulative rate function of the rates within the system through the formula:

<center>
&Delta;*t* = -ln(*u*) / **R**
</center>

where &Delta;*t* is the change in time, *u* is a generated random number, and **R** is the cumulative rate function.

##L

##M

##N

####*nan*
An acronym for 'not a number'. Appears in the display when running a simulation in the connections between regions chart when there are no agents present in a region, and therefore cannot be connected to agents in any region.

####'nano'
A very basic text editor. Typing in the command `nano randomfile` will allow you to edit 'randomfile' using the text editor 'nano'. There are numerous text editors available for use, but 'nano' is the one we will be using for this documentation.

####node
Corresponds to agents in visualizations.

##O

####output
Directory which contains all the analysis files created from running a network simulation. The files which are created and appear in the 'output' directory can be modified in 'INFILE.yaml'.

##P

####‘preferential_agent’ follow model
Is just the ‘twitter_suggest’ follow method nested in the ‘agent’ follow method. Firstly, a certain agent type will be selected, then based on the degrees of the agents within the certain agent type,
an agent will be selected to follow.

####preference class
**(type: n/a)** details how tweets are retweeted by other agents in the system

##Q

##R

####**R** (cumulative rate funtion)
The sum of the agent add rate, tweet rate, follow rate, and retweet rate.

####'random' follow model
Causes agents to follow other agents randomly; if the number of agents is set to constant and this method is on, you will achieve an Erdos-Renyi degree distribution (Poisson distribution).

####region
**(type: n/a)** The geographical location of an agent, which is randomly selected with weights that can be set in a network ismulation.

####retweet
A retweet is the rebroadcasting of tweet by an agent who did not actually create the tweet.

##S

####simulation
A simulation is an enactment of something that is anticipated or is being tested. In ***#k@***, we create simulations of social networks.

####social network
An association of agents connecting with one another within a certain framework for the means of interacting with one another.

##T

####terminal
A computer terminal is a device on a computer that allows a user to enter data into, and display data out of, it.

####tweet
A tweet is a message, consisting of 140 characters or less, that a Twitter user creates on Twitter.

####Twitter
Twitter is a social network where users can connect with each other by following one another and release messages called tweets. These tweets can contain a hashtag that associate that tweet with a particular topic. These tweets can be rebroadcasted by other agents retweeting them, so that their respective followers can see the message. Through the implementation of retweeting and hashtagging, a simple message has the opportunity to be read by millions of people, and users can connect with each other like never before.

####‘twitter’ follow model
A model that incorporates all of the above follow mechanisms. The weights for each mechanism can be set for each follow method. See model weights for more information.

####‘twitter_suggest’ follow model
Follows the preferential attachment model outlined by Barabasi and Albert. The basics behind the method is that the degree of the agent determines the probability of following the agent;
the greater the degree the greater the probability of the agent being followed. The thresholds and weights variables outlined in the follow ranks section determines how the degree of the agents are binned and the corresponding weights for each bin.
If you want to achieve the preferential attachment method similar to the use barabasi method you can set the thresholds to increment by 1,
with linear spacing from 0 to the max number of agents and set the weights to increment by 1 with linear spacing from 1 to the max number of agents.

##U

####unfollow
An unfollow is when an agent stops following another agent.

##V

##W

##X

##Y

##Z
