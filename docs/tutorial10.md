[hashkat.org](http://hashkat.org)

# Finding the Most Popular Tweet

The most popular tweet is the one that gets retweeted the most. 

####  Types of Tweets

There are four types of tweets that an agent may create, 'plain', 'musical', 'humorous', and 'ideological'.  

In **#k@** the signals between nodes (agents) are simulated, there is no real content (Agent A is not funnier than Agent B).  The coded type of tweet defines the tweet's potential scope of retransmission (retweet).  An 'plain' or 'humorous' tweet will be retweeted only within the same language.  An 'ideological' tweet may be retweeted within the same region and language.  A 'musical' tweet may transcend region and language. 

For this tutorial we are going to run the INFILE.yaml created in [Tutorial 9](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial09_without_other_follow/INFILE.yaml) without the other follow methods enabled.

We will set 'use_follow_via_retweets' to 'true', run the simulation and find the most popular tweet in the network. Using this tweet's content as evidence, we are going to see what conclusions can be drawn about our network, and then analyze the input file and **output** directory for this simulation to see if we were right.   Because these tutorials have used simple worlds with limited regions, preference_classes and agent_types, the most popular tweet will be pretty simple.  However in a more complex network the most popular tweet, and it's path, can be fascinating.

A copy of the input file used in this tutorial may be found in [/hashkat/docs/tutorial_input_files/tutorial10/](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial10/INFILE.yaml). This tutorial should take approximately 10 minutes to complete.

#### Analyzing the Most Popular Tweet's Content

After running the simulation we look at **most_popular_tweet_content.dat** file in the **output** directory. Examining this file, we see the following:

<center>
<img src='../img/tutorial10/most_popular_tweet_content.png'>
</center>

Note: in output many of the variables will have a number printed instead of a name. For example if you have 3 possible ideologies in your network, let's say {'Red', 'Green', 'Blue'}, and the author of this tweet is 'Red', the number '0' will be printed beside it since 'Red' is the zeroth element in the list of ideologies. This will occur for the variables 'Author Agent_Type', 'Author Ideology', 'Author Region', 'Tweet Language', and 'Tweet Type.

In this case, the author of this tweet was agent ID 5 (their individual record number).  Agent ID 5 was a 'Standard' agent (since 'Standard' is the zeroth agent_type) with a 'Green' ideology living in 'Ontario'. 

The tweet was created 496 simulated minutes after the simulation started, and was a humorous tweet in English, with no hashtag present. There were 17 agents who retweeted this tweet.

All of the information that we collected on the most popular tweet makes sense since this simulation had only a Standard agent type, 100% of agents were from the Ontario region, and English was the language of this region.

Since the ideologies were all equally weighted, 'Author Ideology' could just as easily have been Red, Blue, or Orange instead of Green.  It also makes sense that the most popular tweet was a humorous one, since the preference class enabled for agents in this simulation ('StandardPref') had humorous tweets with double the transmission rate (20%) of other tweet types.

#### Retweet Visualization

We can visualize this data by opening the graph file **retweet_viz.gexf** in Gephi. The visualization we created is shown below:

<center>
<img src='../img/tutorial10/retweet_visualization.png'>
</center>

In the above visualization, the pink node is the author of the most popular tweet, the various coloured nodes are the author's followers, who received the original tweet and did not retweet it.  The green nodes are the retweeted the tweet and the blue nodes are non-followers who received the retweet and did not retransmit it. 

From the visualization it is clear that one of the main reasons this agent was the author of most popular tweet was because the agent was highly connected, giving their original tweet a broad audience, and therefore a better chance of being retweeted. 


#### Next Steps

With the completion of this tutorial, we have now worked with not only how agents connect with one another, but also the content of the broadcasts they make to each other. In the next tutorial, we will discuss how agents may disconnect with each other during the course of a simulation, or ['unfollowing'](http://docs.hashkat.org/en/latest/tutorial11/).  