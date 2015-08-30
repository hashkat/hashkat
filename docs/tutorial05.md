[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Agent Follow Model

An agent-based follow method is one in which agents follow a twitter user based on their agent class, though who they follow within that agent class is totally random. Let's run an *agent* follow model using ***#k@***. The input file used in this simulation can be found for reference in the *docs/tutorial_input_files* directory in hashkat, with this one under the title *tutorial05*. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial05/INFILE.yaml). This tutorial should take approximately 20 minutes to complete.

<iframe width="560" height="315" src="https://www.youtube.com/embed/rBwWCohfato" frameborder="0" allowfullscreen></iframe>

## Constructing The Network

Starting with the INFILE.yaml file we used in **Tutorial 4** for [**Other Twitter_Suggest Models**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial04_other/INFILE.yaml), we will of course change the follow model from *twitter_suggest* to *agent*, and finally get to implement our *Celebrity* agents into our network. This can be done by simply changing the *Celebrity* add weight to 100.0. With both the *Standard* agents and *Celebrity* agents having an add weight of 100.0, the number of *Standard* agents and *Celebrity* agents within the simulation will be roughly the same, with the proportion of *Standard* agents and *Celebrity* agents being added to the network being 1:1. Since celebrities tend to garner vastly more followers than ordinary people, it makes sense for our simulation of *Standard* and *Celebrity* agents to mimic this. Therefore, we will change the *Standard* follow weight to 10.0 and the *Celebrity* follow weight to 90.0. From this we should expect to see *Celebrity* agents to garner about 90% of the follows and *Standard* agents to garner about 10%.

```python
agents:
  - name: Standard
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 10.0
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates: 
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.01}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.01}

  - name: Celebrity
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 90.0
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.01}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.01}
```

## Running and Visualizing The Network

Running this simulation we produced the following network:

<p align='center'>
<img src='../img/tutorial05/visualization.png'>
</p>

As expected, the *Celebrity* agents (red dots) tend to have much more followers than the *Standard* agents (blue dots).

We can also do further analysis of this network through use of the output files that have been created from running this simulation. As we discussed in [**Tutorial 1**](http://docs.hashkat.org/en/latest/tutorial01/), an agent's cumulative degree distribution is the total number of connections it has. We found the normalized cumulative degree distribution probability for all agents in that [tutorial](http://docs.hashkat.org/en/latest/tutorial01/), let's do the same here but we will also compare the overall normalized cumulative degree distribution to that of each agent type.

We can do this using 'gnuplot' by entering 'gnuplot' in the *output* directory and inputting the following commands:

`set style fill transparent solid 0.5 noborder`

`set title 'Cumulative-Degree Distribution'`

`set xlabel 'Cumulative-Degree'`

`set ylabel 'Normalized Cumulative-Degree Probability'`

`plot 'Celebrity_info.dat' u 1:4 lc rgb 'goldenrod' w filledcurves above y1=0 title 'Celebrity', 'Standard_info.dat' u 1:4 lc rgb 'blue' w filledcurves above y1=0 title 'Standard', 'cumulative-degree_distribution_month_000.dat' u 1:2 lc rgb 'dark-red' w filledcurves above y1=0 title 'Overall'`

<p align='center'>
<img src='../img/tutorial05/cumulative-degree_distribution_month_000.svg'>
</p>

As we can see, there are no *Standard* agents with a cumulative degree greater than 20, with most having a cumulative degree just under or just above 5. The *Celebrity* agents' cumulative degree distribution has much more variance, with *Celebrity* agents in this network having up to almost 35 connections in this simulation. The *Overall* cumulative degree distribution illustrates this for all agents in the network.

## Next Steps

Continue experimenting with the *agent* follow model. Adding more agent types into your simulation, such as bots and organizations, and manipulating all of their add weights, follow weights, follow rates, etc., can lead to some truly complex and interesting networks. When you are comfortable using the *agent* follow model, proceed to the next tutorial, where we actually combine the *twitter_suggest* follow model and *agent* follow model to create the [*preferential_agent* follow model](http://docs.hashkat.org/en/latest/tutorial06/).