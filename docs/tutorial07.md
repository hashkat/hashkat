[hashkat.org](http://hashkat.org)

# The Hashtag Follow Model

The *hashtag* follow model is one in which an agent follows another agent through a hashtag present in their tweets that matches their hashtag preferences. In order for this follow method to be possible, tweeting must be enabled and *use_hashtag_probabilty* must be greater than zero, or else there will be no hashtags present in the simulation. Let's run a *hashtag* follow model simulation. The *INFILE.yaml* that we will create in this tutorial can be found for reference in the **docs/tutorial_input_files** directory in **hashkat**, with this one under the title **tutorial07**. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial07/INFILE.yaml). This tutorial should take approximately 15 minutes to complete.

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/szo8_lx4Rzc" frameborder="0" allowfullscreen></iframe>
</center>

## Constructing The Network

Using the *INFILE.yaml* we created in [**Tutorial 6**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial06/INFILE.yaml) as a starting point, we will of course change the follow model to *hashtag*. We will let the *use_hashtag_probability* remain at 0.5 or 50%, so that 50% of the tweets will contain the hashtags necessary for following in the *hashtag* follow model. Under the agent types section, there are *hashtag_follow_options*, where you can decide if agents of that particular agent type will only follow other agents living in the same region as them and/or that share the same ideology as them through hashtags present in tweets by inputting *true* for *care_about_region* and/or *care_about_ideology* respectively. For this simulation, we will only have agents follow other agents that share the same ideology as them, so input *true* for *care_about_ideology* and have *care_about_region* remain *false*. For simplicity, we will also remove any *Celebrity* agents from this network simulation by changing the *Celebrity* add weight to 0.0.

```python
hashtag_follow_options:
  care_about_region: false # does the agent care about where the agent they will follow is from?
  care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
```

## Running and Visualizing The Network

Creating a network with this configuration, we produced the following visualization using the *Yifan Hu* and *Contraction* layouts:

<center>
<img src='../img/tutorial07/visualization.png'>
</center>

As you can see from this visualization, agents are separated into 4 main groups, with several other agents having no connections, being unable to follow or be followed by another agent in time. These 4 groups correspond to the 4 different ideologies we have in our network, named *Red*, *Blue*, *Green*, and *Orange*. It is important to note that the colours of agents in the visualization do not correspond to ideology colours, but are arbitrarily given to differentiate agents of different types (e.g. *Standard* agents, *Celebrity* agents, etc.).

A plot of the cumulative-degree distribution of these agents is shown below:

<center>
<img src='../img/tutorial07/cumulative-degree_distribution_month_000.svg'>
</center>

As expected given the number of unconnected agents in this network, there are either agents with a very low degree or high degree in this network, though there are much more agents with a low degree.

The commands used to create this graph in Gnuplot were:

`set style data linespoints`

`set title 'Cumulative-Degree Distribution'`

`set xlabel 'Cumulative-Degree'`

`set ylabel 'Normalized Cumulative-Degree Probability'`

`plot 'cumulative-degree_distribution_month_000.dat' title ''`

We can also plot the tweet distribution through use of the *tweets_distro.dat* file.

<center>
<img src='../img/tutorial07/tweets_distro.svg'>
</center>

As we can see from this plot of the tweet distribution, most agents have made 1 to 6 tweets in this simulation and that as the number of tweets increases, the number of agents that have made this many tweets decreases.

The commands used to create this graph in Gnuplot were:

`set boxwidth 0.75`

`set style fill solid 0.5`

`set xrange[0:]`

`set title 'Tweet Distribution'`

`set xlabel 'Number of Tweets'`

`set ylabel 'Normalized Probability of Agents with that Many Tweets'`

`plot 'tweets_distro.dat' with boxes title ''`

## Next Steps

You have now gained experience with the *hashtag* follow model. You are encouraged to try experimenting with this follow model by implementing more regions and ideologies into your network simulations, as well as several agent types with differing *hashtag_follow_options* and *tweet* rates. Doing so is bound to lead to some fascinating results. When you feel you are ready, move on to the next tutorial, where we will discuss the final follow model, the [*twitter* follow model](http://docs.hashkat.org/en/latest/tutorial08/), which incorporates all of the other follow models into a network simulation.