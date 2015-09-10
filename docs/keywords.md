[hashkat.org](http://hashkat.org)

# Key Words

Serves as a reference for some of the key terms used in this documentation.

## A

#### agent

Corresponds to a member of a social network.

#### *agent* follow model

Follow model where agents follow other agents based on their agent type. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial05/).

## B

#### Barabasi-Albert Model

An algorithm created by Albert-Laszlo Barabasi and Reka Albert to generate networks where agents with the greater number of followers have a higher probability of being followed. More information on this can be found [here](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model).

#### Barabasi Configuration

Network where all agents follow a set number of agents. In the Classic Barabasi configuration, every agent follows only one other agent. In the Non-Classic Barabasi configuration, this number is greater than one.

## C

#### command line

A place where a user can issue commands into the computer from a terminal.

#### cumulative-degree

The total number of connections (followers and followings) an agent has in a network. The sum of an agent's in-degree and out-degree.

#### cumulative rate function (**R**)

The sum of the agent *add* rate, *tweet* rate, *follow* rate, and *retweet* rate.

## D

## E

#### edge

Corresponds to a connection between agents in a network.

#### Erdos-Renyi Model

A model developed by Paul Erdos and Alfred Renyi to generate random graphs. More information on thie model can be found [here](https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model).

## F

#### follow

The act of an agent connecting with another agent.

#### follow model

A framework inputted into a network simulation that dictates how agents generally connect with one another. There are currently 6 follow models within ***#k@***: *random*, *twitter_suggest*, *agent*, *preferential_agent*, *hashtag*, and *twitter*.

## G

#### Gephi

A network visualization tool used heavily in this documentation. You can find more information on and download Gephi [here](http://gephi.github.io/).

#### Gnuplot

A command-line driven utility for graphing used in tutorials for the plotting of data. More information on Gnuplot can be found [here](http://gnuplot.sourceforge.net/).

## H

#### **hashkat**

The **hashkat** directory contains all the filess and directories necessary to run ***#k@***.

#### hashtag (#)

Sometimes used in tweets to classify messages by a key word or topic.

#### *hashtag* follow model

Follow model where agents follow other agents based on their ideology and/or region, which can be determined via the hashtags present in their tweets. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial07/).

## I

#### ideology

An agent's belief system or dogma.

#### in-Degree

The total number of followers an agent has in a network.

#### interactive mode

A feature of ***#k@*** under development.

## J

## K

#### Kinetic Monte Carlo Method

An algorithm designed to carry out a simulation where the rate of which time is increased by is influenced by the cumulative rate function through the formula:

<center>
&Delta;*t* = -ln(*u*) / **R**
</center>

where &Delta;*t* is the change in time, *u* is a generated random number, and **R** is the cumulative rate function. More information on this can be found [here](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo).

## L

#### language

The particular dialect that an agent speaks.

## M

## N

#### nano

A very basic text editor. Typing in the command `nano randomfile` will allow you to create and/or edit *randomfile* using the text editor nano. More information on nano can be found [here](http://www.nano-editor.org/)

#### Networkx

[Networkx](https://networkx.github.io/) is a Python language software package that can be used to create, modify, and analyze networks. Documentation on Networkx can be found [here](https://networkx.github.io/documentation/latest/).

#### node

Corresponds to an agent in a network.

## O

#### out-Degree

The total number of agents a particular agent is following in a network.

#### **output**

Directory which contains all the analysis files created from running a network simulation.

## P

#### *preferential_agent* follow model

Follow model where agents follow other agents based on their agent type and their popularity within that agent type. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial06/).

#### preference class

Details the probability that tweets made by agents of a particular agent type will be retweeted based on their tweet content and the probability that an agent will follow another agent based on seeing their retweeted tweet as opposed to just retweeting the retweet.

## Q

## R

#### **R** (cumulative rate funtion)

The sum of the agent *add* rate, *tweet* rate, *follow* rate, and *retweet* rate.

#### *random* follow model

Follow model where agents follow other agents randomly. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial03/).

#### region

The geographical location of an agent.

#### retweet

A retweet is the rebroadcasting of a tweet.

##S

#### simulation

A simulation is an enactment of something that is anticipated or is being tested. In ***#k@***, we create simulations of social networks.

#### social network

An association of agents connecting with one another in order to interact with each other within a given framework.

##T

#### terminal

A computer terminal is a device on a computer that allows a user to enter data into, and display data out of, it.

#### tweet

A tweet is a message, consisting of 140 characters or less, that a Twitter user creates on Twitter. In ***#k@***, tweets are the broadcasts made by agents

#### Twitter

Twitter is a social network where users can connect with each other by following one another and create broadcasts called tweets. These tweets can contain a hashtag that associate that tweet with a particular topic or key word. These tweets can be rebroadcasted by other users retweeting them, so that their respective followers can see the broadcast.

#### *twitter* follow model

Follow model that incorporates all of the other follow models. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial08/).

#### *twitter_suggest* follow model

Follow model where agents follow other agents based on how many followers they have. More information on this follow model can be found [here](http://docs.hashkat.org/en/latest/tutorial04/).

##U

#### unfollow

An unfollow is when an agent stops following another agent.

##V

##W

##X

##Y

##Z