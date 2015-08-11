[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Output

This section serves as a reference for users who have run a simulation, and wish to analyze the data this run has outputted. This page goes over every aspect of the files found in the output directory.

## Agent Type Info

`agenttype_info.dat`

Present and named for every single agent type in the network (e.g. 'Standard'
agent type would have a file named *Standard_info.dat*). Outlines the percentage of agents 
that follow this agent type, the percentage of the proportion of agent types that this agent 
type follows with respect to each other, and the number of possible degrees for this agent 
type and their weighted in-degree, out-degree, and cumulative-degree that match this value, 
as well as the logs of each. An example of the information stored in this file is shown below:

<p align='center'>
<img src='../img/output/agenttype_info_file.png'>
</p>

## Categories Distribution

`Categories_Distro.dat`

Outlines the number of tweets, follows, and retweets in their respective bin
thresholds.

<p align='center'>
<img src='../img/output/categories_distro_file.png'>
</p>

## Cumulative-Degree Distribution

`cumulative-degree_distribution_month_000.dat`

Outlines all the possible total degrees an agent can have in that particular
month in the simulation, as well as its normalized probability, logarithm, and
normalized probability logarithm in columns in that order. The following shows some of the information stored in this output file:

<p align='center'>
<img src='../img/output/cumulative-degree_distribution_file.png'>
</p>

## Degree Distribution by Follow Model

`dd_by_follow_model.dat`

Indicates the normalized probability and log of the normalized probability for each degree per follow method.

<p align='center'>
<img src='../img/output/dd_by_follow_model_file.png'>
</p>

## In-Degree Distribution

`in-degree_distribution_month_000.dat`

Outlines all the possible in degrees an agent can have in that particular
month in the simulation, as well as its normalized probability, logarithm, and
normalized probability logarithm in columns in that order. An example of this file's contents is shown below:

<p align='center'>
<img src='../img/output/in-degree_distribution_file.png'>
</p>

## Main Statistics

`main_stats.dat`

Outlines the most important network statistics for this simulation.
Under **USERS** we have the total number of agents by the simulation's end,
and the number and percentage of those agents of each particular agent
type. Under **TWEETS** we have the total number of tweets made
during the simulation, as well as the number and percentage of those that
have a hashtag and the number and percentage of those that were tweeted by
agents of each type. Under **RETWEETS** we have the total number of
retweets that occurred throughout the course of the simulation, and how
many of those were done by agents of each type.
Under **FOLLOWS** we have the total number of follows and the total number
of follow attempts that occurred in the simulation, the number and
percentage of follow attempts achieved through each follow method, and the 
number and percentage of follows made by agents of each agent type.

<p align='center'>
<img src='../img/output/main_stats_file.png'>
</p>

## Model Match

`model_match.dat`



## Most Populat Tweet Content

Details basic information regarding the most retweeted, and therefore most popular, tweet in the entire network simulation. Basic information includes: the ID number of the tweet author,the author agent type, the author's ideology, the region the author lives in, the simulated point in time in the simulaiton when the tweet was created, the language of the tweet, the tweet type, whether or not a hashtag is present in the tweet, and the number of retweeters for this tweet. For many of these variables, a number will be printed instead of a name. For example, for 'Author Ideology', if you have 3 possible ideologies in your network, let's say 'Red', 'Green', and 'Blue' and the author of this tweet is ideologically 'Red', the number 0 will be printed beside 'Author Ideology' instead of 'Red', since it is the zeroth element in the list of ideologies. This will occur, for the variables 'Author Agent Type', 'Author Ideology', 'Author Region', 'Tweet Language', and 'Tweet Type'.

An example of the data found in this output file is shown below:

<p align='center'>
<img src='../img/output/most_popular_tweet_content_file.png'>
</p>

## Network Connections

`network.dat`

Outlines which agents are following who. In this file there are two columns,
with the agents in the first column (classified by an ID number)
following the agents in the listed in the second column (also classified
by an ID number). The following is an example of what the content's of this file would like:

<p align='center'>
<img src='../img/output/network_dat_file.png'>
</p>

## Network Visualization

`network.gexf`

*network.gexf* is a file that can be used to vizualize the connections
between users on your network using visualization software such as
[Gephi](http://gephi.github.io/). Though the details in this file are less relevant than the 
visualization created from this file itself, the following shows the contents of this file:

<p align='center'>
<img src='../img/output/network_gexf_file.png'>
</p>

## Network Refactoring

`network_refactoring.dat'



## Out-Degree Distribution

`out-degree_distribution_month_000.dat`

Outlines all the possible out degrees an agent can have in that particular
month in the simulation, as well as its normalized probability, logarithm, and
normalized probability logarithm in columns in that order. Below is an image of some of the contents of this file:

<p align='center'>
<img src='../img/output/out-degree_distribution_file.png'>
</p>

## Region Connection Matrix

`region_connection_matrix_month_000.dat`

Displays the connections made between regions. An example of this output file is shown below:

<p align='center'>
<img src='../img/output/region_connection_matrix_file.png'>
</p>

## Retweet Distribution

`retweets_distro.dat`

Outlines the distribution of how many agents have made every possible number of 
retweets within the network simulation.

<p align='center'>
<img src='../img/output/retweets_distro_file.png'>
</p>

## Retweet Visualization

`retweet_viz.gexf`

*retweet_viz.gexf* is a file that can be used to vizualize how tweets were retweeted 
in the network simulation using visualization software such as
[Gephi](http://gephi.github.io/). Though the contents of this file are less relevant than the 
visualization created from this file itself, an example of this file can be seen below:

<p align='center'>
<img src='../img/output/retweet_viz_gexf_file.png'>
</p>

## Tweet Distribution

`tweets_distro.dat`

Outlines the distribution of how many agents have made every possible number of tweets
within the network simulation.

<p align='center'>
<img src='../img/output/tweets_distro_file.png'>
</p>