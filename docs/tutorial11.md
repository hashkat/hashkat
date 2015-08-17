[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Unfollowing

Unfollowing is the act in which an agent stops following another agent due to a variety of reasons. In this tutorial, we are going to discuss how unfollowing can be implemented into your network simulation, where agents unfollow other agents due to their tweet content..

## Unfollowing Due to Chattiness

One of the many factors that influence an agent's decision to unfollow another agent is how talkative they are. If an agent is constantly tweeting at an excessive rate, their followers may find that this chattiness may make it difficult for them to view the tweets of other agents they are following, and they may unfollow this chatty agent because of it. There are currently two implementations of unfollowing due to chattiness found in ***#k@***, *stage1_unfollow* and *unfollow_tweet_rate* which are discussed below.

#### Stage 1 Unfollow

If *stage1_unfollow* is implemented into a network configuration, whenever an agent is following another agent, the tweet rate of this newly followed agent will be compared to that of the other agents that the agent was following. If this followed agent's tweet rate is greater than double the average tweet rate of all the other agents that the original agent was following, then this chatty agent will be unfollowed.

#### Unfollow Tweet Rate

The *unfollow_tweet_rate* is a number inputted into the **INFILE.yaml** file that causes an agent who has a tweet rate greater than this value to be randomly unfollowed by one of his/her followers.

### A Network that Implements Unfollowing Due to Chattiness

Let's now create a network simulation that implements unfollowing due to chattiness. We will use the input file we used in **Tutorial 1** as a starting point. The *INFILE.yaml* file created for this tutorial can be found in the *docs/tutorial_input_files* directory in the directory *tutorial11*. This file can also be viewed [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial11/INFILE.yaml).

So using the input file from **Tutorial 1** as a starting point, let's first add another agent type into this network. Let's call this agent type 'Chatty', and it'll have the same paramters as the 'Standard' agent type, except that the tweet rates for these agents will be 0.01 instead of 0.001.

```python
- name: Chatty
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5
    tweet_type:
      ideological: 1.0
      plain: 1.0
      musical: 1.0
      humourous: 1.0 # Can be considered the humourousness of the agent type
  # Probability that following this agent results in a follow-back
  followback_probability: .44
  hashtag_follow_options:
    care_about_region: false # does the agent care about where the agent they will follow is from?
    care_about_ideology: false # does the agent care about which ideology the agent has?
  rates:
      # Rate for follows from this agent:
      follow: {function: constant, value: 0.0001}
      # Rate for tweets from this agent:
      tweet: {function: constant, value: 0.01}
```

Running a simulation with this new agent type, we can see in the *main_stats.dat* output file that there are 9943 total follows that have occurred in the network. It'll be interesting to see how this value changes when unfollowing is implemented.

<p align='center'>
<img src='../img/tutorial11/main_stats_without_unfollowing.png'>
</p>

Let's now implement unfollowing to this network. We will set 'stage1_unfollow' to true and set the *'unfollow_tweet_rate' to 0.01. Since half the agents in this network, the 'Chatty' agents, have a tweet rate 10x that of the other agents, we should definitely expect unfollows to occur due to the incorporation of 'stage1_unfollow'. We should also expect some unfollows to occur due to the 'unfollow_tweet_rate' being equal to the 'Chatty' agent tweet rate. This is due to agents having a tweet rate either equal to, or arund the value of their assigned tweet rate. Therefore, we can expect some 'Chatty' agent to have a tweet rate greater than their assigned tweet rate, and thereby having a tweet rate greater than the 'unfollow_tweet_rate'.

Running this network simulation with unfollowing implemented, we produced the following data in our *main_stats.dat* file:

<p align='center'>
<img src='../img/tutorial11/main_stats_with_unfollowing.png'>
</p>

As we can see, there are 9863 total follows at the end of this simulation as opposed to the 9943 total follows present at the end of the simulation that didn't implement unfollowing. With unfollowing implemented, almost 100 connections were cut in this network. Plotting the in-degree distributions of the 'Standard' agents and 'Chatty' agents, we can see that 'Chatty' generally have less followers than 'Standard' agents, due to them losing followers due to their excessive tweet rate.

<p align='center'>
<img src='../img/tutorial11/agent_type_in-degree_plot.svg'>
</p>

## Next Steps

With the completion of this tutorial, not only have we learned all the different ways in which agents can connect with each other, but also how they cut these connections. Moving on to the next tutorial, we shall discuss how to make these agents in our network simulations unique.