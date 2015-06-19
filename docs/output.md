[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Output

This page serves as a reference for users who have run a simulation, and wish to analyze the data this run has outputted. This page goes over every aspect of the files found in the output directory.

`Categories_Distro.dat`

Outlines the number of tweets, follows, and retweets in their respective bin
thresholds. Appears in twitter-preferential modelled simulations.

`connections_vs_nodes.dat`



`cumulative-degree_distribution_month_000.dat`

Outlines all the possible total degrees an agent can have in that particular
month in the simulation, as well as its normalized probability, logarithm, and
normalized probability logarithm in columns in that order.

`dd_by_agent_type.dat`



`dd_by_follow_model.dat`



`in-degree_distribution_month_000.dat`

Outlines all the possible in degrees an agent can have in that particular
month in the simulation, as well as its normalized probability, logarithm, and
normalized probability logarithm in columns in that order.

`main_stats.dat`

Outlines the most important network statistics for this simulation.
Under **USERS** we have the total number of agents by the simulation's end,
and the number and percentage of those agents are of each particular agent
type. Under **TWEETS** we have the total number of tweets made
during the simulation, as well as the number and percentage of those that
have a hashtag and the number and percentage of those that were tweeted by
agents of each type. Under **RETWEETS** we have the total number of
retweets that occurred throughout the course of the simulation, and how
many of those were done by agents of each type.
Under **FOLLOWS** we have !!!!!!!!!!!!

`model_match.dat`



`network.dat`

Outlines which agents are following who. In this file there are two columns,
with the agents in the first column (classified by an ID number)
following the agents in the listed in the second column (also classified
by an ID number).

`network.gexf`

*network.gexf* is a file that can be used to vizualize the connections
between users on your network using visualization software such as
[Gephi](http://gephi.github.io/).

`out-degree_distribution_month_000.dat`

Outlines all the possible out degrees an agent can have in that particular
month in the simulation, as well as its normalized probability, logarithm, and
normalized probability logarithm in columns in that order.

`region_connection_matrix.dat`

Displays the connections between the agents in regions

`retweets_distro.dat`



`retweet_viz.gexf`



`agenttype_info.dat`

Present and named for every single agent type in the network (e.g. 'Standard'
agent type would have a file named *Standard_info.dat*). Outlines the
possible total number of degrees for entites of this type, as well as each
degrees' respective !!!!!!!!!!! 

`tweets_distro.dat`

