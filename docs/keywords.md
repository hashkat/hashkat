[hashkat.org](http://hashkat.org)

# Key Words

Serves as a reference for some of the key terms used in this documentation.

## A

#### Agent

Corresponds to a member of a social network.  In [graph theory](https://en.wikipedia.org/wiki/Graph_theory) an agent is a 'node'.

#### Agent Follow Model

Follow model in which agents follow other agents based on their agent type. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial05/).

## B

#### Barabasi-Albert Model

An algorithm created by [Albert-Laszlo Barabasi and Reka Albert](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model) to generate networks where agents with a greater number of followers have a higher probability of being followed. 

#### Barabasi Configuration

Network where all agents follow a set number of agents. In the **Classic Barabasi** configuration, every agent follows only one other agent. In the **Non-Classic Barabasi** configuration, an agent may follow more than one other agent.

## C

#### Command Line

A place where a user can enter commands into the computer from a terminal.

#### Cumulative-Degree

The total number of connections (followers and followings) an agent has in a network. The sum of an agent's **in-degree** and **out-degree** connections.

#### Cumulative rate function (**R**)

The cumulative rate function **R** is the sum of the network 'add' rate, and the different agent's 'tweet','follow', and 'retweet' rates. Note:  the 'tweet', 'follow', and 'retweet' rates are multiplied by the number of agents in the network.

**R = add_rate + (number of agents) x (tweet_rate + follow_rate + retweet_rate)**

or more specifically, for **n** number of agents_types in the system, the individual agent_type 'tweet', 'follow' and 'retweet' rates are summed.

**R = add_rate + (tweet_rate<sub>1</sub> + follow_rate<sub>1</sub> + retweet_rate<sub>1</sub>) + ... + (tweet_rate<sub>n</sub> + follow_rate<sub>n</sub> + retweet_rate<sub>n</sub>)**

The default build of **#k@** permits 200 different agent_types (n = 200).  However, you may wish modify the proportions of each agent type, i.e. to have 1000 agents of type 1 (T<sub>1</sub> = 1000), but only 10 of type 2, for a 100:1 ratio A:B.  Then the **R** function becomes:

**R = add_rate + T<sub>1</sub>(tweet_rate<sub>1</sub> + follow_rate<sub>1</sub> + retweet_rate<sub>1</sub>)+...+T<sub>n</sub>(tweet_rate<sub>n</sub> + follow_rate<sub>n</sub> + retweet_rate<sub>n</sub>)**

## D

## E

#### Edge

Term in [graph theory](https://en.wikipedia.org/wiki/Graph_theory) that corresponds to a connection between two agents, or a 'follow' in **#k@**.

#### Erdos-Renyi Model

A model developed by [Paul Erdos and Alfred Renyi](https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model). 

## F

#### Follow

The act of an agent (A) connecting with another agent (B) such that A receives the transmissions of B.  In [graph theory](https://en.wikipedia.org/wiki/Graph_theory) a follow creates a uni-directional 'edge' between the two agents ('nodes').
 
#### Follow Model

A framework input into a network simulation that dictates how agents follow (connect) with one another.  There are currently 6 follow models within **#k@**: **random**, **twitter_suggest**, **agent**, **preferential_agent**, **hashtag**, and **twitter**, where **twitter** is a user defined combination of the other 5 models.

## G

#### Gephi

[Gelphi](http://gephi.github.io/). is free third party software which enables network visualization. **#k@** output can be rendered in [Gelphi](http://gephi.github.io/).

#### Gnuplot

[Gnuplot](http://gnuplot.sourceforge.net/) is free third party software that is a command-line driven utility for graphing, and is used in tutorials in this documentation for the plotting of data. 

## H

#### Hashkat or **#k@**

The name of this network simulation program.  The **hashkat** directory contains all the files and directories necessary to run **#k@**.

#### Hashtag (#)

A hashtag is a prefix to words that turns that word into a keyword that can be searched by other users.  If my tweet simply says 'I love pie', my tweet may not be easily found by an agent not connected to me.  If I tweet 'I love **#pie**, my tweet will be accessible to anyone searching keyword 'pie' and may be grouped for display on a page relating to 'pie'.

#### Hashtag Follow Model

Follow model where agents follow other agents based on their ideology and/or region, which can be determined via the hashtags present in their tweets. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial07/).

## I

#### Ideology

An agent's belief system.  The characteristics of an ideology are user defined in **#k@** and can be based on any criteria, for example, agents may be divided on whether they follow **DC** or **MARVEL** comic book universes.   

#### In-Degree

The total number of followers an agent has in a network.  The followers receive input ('tweets') from the agent.

#### Interactive Mode

A feature of **#k@** under development.

## J

## K

#### Kinetic Monte Carlo Algorithm

[Kinetic Monte Carlo (KMC)](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo) is an algorithm designed to carry out a simulation. 

In **#k@** the rate of increase in time is influenced by the cumulative rate function through the formula:

**&Delta;t = -ln(*u*) / R**

where **&Delta;t** is the change in time, **u** is a generated random number, and **R** is the cumulative rate function.

## L

#### Language

The particular language in which an agent transmits.

## M

## N

#### Nano

[Nano](http://www.nano-editor.org/) is a text editor. Typing in the command:

`nano randomfile` 

will allow you to edit 'randomfile'.

#### Networkx

[Networkx](https://networkx.github.io/) is a Python language software package that can be used to create, modify, and analyze networks. Documentation on Networkx can be found [here](https://networkx.github.io/documentation/latest/).

#### Node

A term in [graph theory](https://en.wikipedia.org/wiki/Graph_theory) that corresponds to an 'agent' in the **#k@** network.

## O

#### Out-Degree

The total number of agents a particular agent follows in the network.  The agent receives output ('tweets') from these other agents.

#### Output Directory

Directory which contains all the analysis files created from running a network simulation.

## P

#### Preferential Agent Follow Model

Follow model where agents follow other agents based on their agent type and their popularity within that agent type. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial06/).

#### Preference Class

The Preference Class specifies the probability of an agent re-tweeting (retransmitting) a tweet received based on tweeter ideology, type, or tweet content.  

This class also contains configuration for the probability that an agent will follow another agent based on the content of the retweet.

#### **R** 

See **Cumulative Rate Function** above.

#### Random Follow Model

Follow model where agents follow other agents randomly. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial03/).

#### Region

The geographical location of an agent.  Region characteristics are user defined in **#k@**.

#### Retweet

If a 'tweet' is a unidirectional signal from agent to follower(s), a 're-tweet' is the retransmission of the same signal from the follower to their followers.

##S 

#### Simulation

A simulation is a modelled enactment of a process over simulated time. In ***#k@***, we create simulations of social networks.

#### Social Network

In [graph theory](https://en.wikipedia.org/wiki/Graph_theory), a 'network' is a graph.  A network is an association of agents connecting with one another to interact in a given framework.  The word "social" implies the association is voluntary and recreational.

##T

#### Terminal

A terminal is a device on a computer that allows a user to direct data and commands to the computer and receive system information and processed data from the computer.

#### Tweet

A tweet is a uni-directional transmission (or broadcast, or signal) of limited size and duration that passes from agent to follower.  

On [Twitter](https://twitter.com/?lang=en) the signals are called 'tweets' and are text messages of 140 characters or less that may include an image.  On [Instagram](https://instagram.com/instagram/) the transmissions are images.

In **#k@**, 'tweet' refers to the signal made by agent to their follower(s).  Tweets may contain 'hashtags' to make their content more searchable by unconnected agents, to increase the likelihood of the agent gaining followers.
 
#### Twitter

[Twitter](https://twitter.com/?lang=en) is a social network where users may connect with other agents to receive and send broadcasts called 'tweets'. 

#### Twitter Follow Model

The Twitter Follow Model is a combination follow model that incorporates the other follow models in user defined proportions.  More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial08/).

#### Twitter Suggest Follow Model

Follow model where agents follow other agents based on how many followers they have. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial04/).

##U

#### Unfollow

If a 'follow' is the creation of a connection between two agents, an 'unfollow' deletes this connection.  

In **#k@** an 'unfollow' may be based on a decision by the follower (the agent who created the original connection) or be a random process caused by behavior of the tweeting agent.
 
##V

##W

##X

##Y

##Z