[hashkat.org](http://hashkat.org)

# Unfollowing

Unfollowing is the act in which an agent stops following another agent. In this tutorial, we are going to discuss how unfollowing can be implemented into your network simulation through various means. This tutorial should take approximately 20 minutes to complete.

## Unfollowing Due to Chattiness

One of the many factors that influence an agent's decision to unfollow another agent is how talkative they are. If an agent is constantly tweeting at an excessive rate, their followers may find that this chattiness may make it difficult for them to view the tweets of other agents they are following, and they may unfollow this chatty agent because of it. There are currently two applications of unfollowing due to chattiness found in ***#k@***, **stage1_unfollow** and **unfollow_tweet_rate**, both of which are discussed below.

#### Stage 1 Unfollow

If **stage1_unfollow** is implemented into a network simulation, whenever an agent is following another agent, the tweet rate of this newly followed agent will be compared to the average tweet rate of all the other agents that the agent is following. If this tweet rate is greater than double the average tweet rate of all the other agents being followed, then this chatty agent will be unfollowed.

#### Unfollow Tweet Rate

The **unfollow_tweet_rate** is a number inputted into the *INFILE.yaml* file that causes an agent who has a tweet rate greater than this value to be randomly unfollowed by one of his/her followers.

### A Network that Implements Unfollowing Due to Chattiness

Let's now create a network simulation that incorporates unfollowing due to chattiness. The *INFILE.yaml* file that we will create for this tutorial can be found in the **docs/tutorial_input_files** directory in **hashkat** in the directory **tutorial11**. This file can also be viewed [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial11/INFILE.yaml).

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/xcZW05GMVZo" frameborder="0" allowfullscreen></iframe>
</center>

Using the input file from [**Tutorial 1**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial01/INFILE.yaml) as a starting point, let's first add another agent type into this network. Let's call this agent type *Chatty*, and it'll have the same parameters as the *Standard* agent type, except that the tweet rates for these agents will be 0.01 instead of 0.001.

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
      humorous: 1.0
  # Probability that following this agent results in a follow-back
  followback_probability: .44
  hashtag_follow_options:
    care_about_region: false # does the agent care about where the agent they will follow is from?
    care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
  rates:
      # Rate for follows from this agent:
      follow: {function: constant, value: 0.0001}
      # Rate for tweets from this agent:
      tweet: {function: constant, value: 0.01}
```

Running a simulation with this new agent type, we can see in the *main_stats.dat* output file that there are 9943 total follows that have occurred in the network. It'll be interesting to see how this value changes when unfollowing is implemented.

<center>
<img src='../img/tutorial11/main_stats_without_unfollowing.png'>
</center>

Let's now incorporate unfollowing into this network. We will set *stage1_unfollow* to true and set the *unfollow_tweet_rate* to 0.01. Since half the agents in this network, the *Chatty* agents, have a tweet rate 10x that of the other agents, we should definitely expect unfollows to occur due to the incorporation of *stage1_unfollow*. We should also expect some unfollows to occur due to the *unfollow_tweet_rate* being equal to the *Chatty* agent tweet rate. This is due to the fact that agents have a tweet rate either equal to, or around the value of their assigned tweet rate. Therefore, we can expect some *Chatty* agents to have a tweet rate greater than their assigned tweet rate, and thereby have a tweet rate greater than the *unfollow_tweet_rate*.

Running this network simulation with unfollowing implemented, we produced the following data in our *main_stats.dat* file:

<center>
<img src='../img/tutorial11/main_stats_with_unfollowing.png'>
</center>

As we can see, there are 9863 total follows at the end of this simulation as opposed to the 9943 total follows present at the end of the simulation that didn't incorporate unfollowing. With unfollowing implemented, almost 100 connections were cut in this network. Plotting the in-degree distributions of the *Standard* agents and *Chatty* agents for this network simulation, we can see that the *Chatty* agents generally have slightly less followers than the *Standard* agents, due to them losing followers due to their excessive tweet rate.

<center>
<img src='../img/tutorial11/agent_type_in-degree_plot.svg'>
</center>

The commands used to plot this graph in Gnuplot were:

`set style fill transparent solid 0.5 noborder`

`set title 'In-Degree Distribution'`

`set xlabel 'In-Degree'`

`set ylabel 'Normalized In-Degree Probability'`

`plot 'Chatty_info.dat' u 1:2 lc rgb 'red' with filledcurves title 'Chatty',`
`'Standard_info.dat' u 1:2 lc rgb 'blue' with filledcurves title 'Standard'`

## Next Steps

With the completion of this tutorial, not only have we learned all the different ways in which agents can connect with each other, we've now also learned how they can cut these connections. Moving on to the next tutorial, we shall discuss how to make these agents in our network simulations [unique](http://docs.hashkat.org/en/latest/tutorial12/).