[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Finding the Most Popular Tweet

The most popular tweet is undoubtedly the one that gets retweeted the most. By just looking at the content of the most popular broadcast in a social network, you can get a good idea of what many of the agents in your social network are like. Is there an ideology associated with this popular tweet? A region? Is it a humourous tweet? Did this tweet get retweeted so much partially because the agent who made the tweet was so popular? Perhaps one of that agent's followers was extremely popular, allowing more agents to see that message. It is amazing all the information you can collect from just one simple broadcast. For this tutorial, we are going to run the **INFILE.yaml** created in **Tutorial 9** that does not use the other follow methods and find the most popular tweet in the network created. Using this tweet's content as evidence, we are going to draw conclusions about some of the details of the network, and then analyze the input file and output directory for this simulation to see if we were right.

The input file used for this tutorial can be referenced [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial10/INFILE.yaml). This tutorial should take approximately 20 minutes to complete.

## Analyzing the Most Popular Tweet's Content

Running this network simulation, we shall view the 'most_popular_tweet_content.dat' file in the output directory to gather some information about this tweet. Examining this file, you should information similar to the following:

<p align='center'>
<img src='../img/tutorial10/most_popular_tweet_content.png'>
</p>

## Retweet Visualization

<p align='center'>
<img src='../img/tutorial10/retweet_visualization.png'>
</p>

## Next Steps

With the completion of this tutorial, we have now worked with not only how agents connect with one another, but also the content of the broadcasts they make to each other. However, in the next tutorial, we will discuss how agents may disconnect with each other throughout hte course of a network simulation. Proceed to the next tutorial for a run through on 'Unfollowing'.  