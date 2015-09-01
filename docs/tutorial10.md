[hashkat.org](http://hashkat.org)

# Finding the Most Popular Tweet

The most popular tweet is undoubtedly the one that gets retweeted the most. By just looking at the content of the most popular broadcast in a social network, you can get a good idea of what many of the agents in your social network are like. Is there an ideology associated with this popular tweet? A region? Is it a humorous tweet? Did this tweet get retweeted so much partially because the agent who made the tweet was so popular? Perhaps one of that agent's followers was extremely popular, allowing more agents to see that message. It is amazing all the information you can collect from just one simple broadcast. For this tutorial, we are going to run the **INFILE.yaml** created in [**Tutorial 9**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial09_without_other_follow/INFILE.yaml) that does not use the other follow methods and find the most popular tweet in the network created. Using this tweet's content as evidence, we are going to draw conclusions about some of the details of the network, and then analyze the input file and output directory for this simulation to see if we were right.

The input file created in this tutorial can be referenced in the *tutorial10* directory within the *docs/tutorial_input_files* directory as well as [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial10/INFILE.yaml). This tutorial should take approximately 20 minutes to complete.

## Analyzing the Most Popular Tweet's Content

Running this network simulation, we shall view the 'most_popular_tweet_content.dat' file in the output directory to gather some information about this tweet. Examining this file, you should information similar to the following:

<p align='center'>
<img src='../img/tutorial10/most_popular_tweet_content.png'>
</p>

As we can gather from this file, the author of this tweet was agent ID 5, who was a *Standard* agent (since *Standard* is the zeroth agent type in the list of agent types) with a *Green* ideology living in *Ontario*. The tweet was created 496.326 simulated minutes after the simulation started, being a 'humorous' tweet in English, with no hashtag present. There were 17 agents who retweeted this tweet. Analyzing the input file used for this simulation, all of this info that we have collected on the most popular tweet makes sense. Since there was only a *Standard* agent type, the *Ontario* region, and the 'English' language implemented in our network, it of course makes sense that these would be the qualities of the author of this tweet. Since the ideology weights were all equally weighted, the author ideology could have just as easily have been *Red*, *Blue*, or *Orange* instead of *Green*. The fact that the author of this tweet had a 'Green' ideology was simply by chance. It makes sense that the most popular tweet was a 'humorous' one since if we look at the preference class enabled for agents in this simulation (the *StandardPref* preference class), 'humorous' tweets have a higher tweet transmission rate than all the other tweet types, meaning that it is much more likely to be tweeted and therefore the most retweeted tweet in most likely to be a humorous one.

## Retweet Visualization

We can visualize this data by opening the graph file we created, *retweet_viz.gexf*, and visualizing it using 'Gephi'. The visualization we created is shown below:

<p align='center'>
<img src='../img/tutorial10/retweet_visualization.png'>
</p>

In the above visualization, the purple node is the author of the most popular tweet, the blue nodes are the retweeters, the green nodes are the non-retweeters, and the other various coloured nodes are followers of the most popular tweet author who did not retweet but are also not connected to an agent that did. Viewing the above visualization, it is quite clear that one of the main reasons that this agent is the author of the most popular tweet is because they are very highly connected, thereby giving them a better chance of having their tweets retweeted. 

## Next Steps

With the completion of this tutorial, we have now worked with not only how agents connect with one another, but also the content of the broadcasts they make to each other. However, in the next tutorial, we will discuss how agents may disconnect with each other throughout the course of a network simulation. Proceed to the next tutorial for a run through on ['Unfollowing'](http://docs.hashkat.org/en/latest/tutorial11/).  