[hashkat.org](http://hashkat.org)

# The Twitter_Suggest Follow Model (The Barabasi-Albert Model)

Based on the [Barabasi-Albert model](https://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model), the **twitter_suggest** follow model is based on a **preferential attachment** mechanism.  A preferential attachment mechanism is one where those who have (x) get more (x).  In **#k@** this means an agent with many followers will get more followers. 

The twitter_suggest preferential attachment follow model can be implemented using three different ways, each of which is outlined below:

* Classic Barabasi
* Non-Classic Barabasi
* Without Barabasi (Other)

### Classic Barabasi

The Classic Barabasi configuration is one in which new agents to the network make one connection with another agent and no other. 

#### Constructing The Network

Our starting point is the **INFILE.yaml** created in [Tutorial 3](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial03/INFILE.yaml). The **INFILE.yaml** file we will create in this example can be found for reference in the [hashkat/docs/tutorial_input_files/tutorial04_classic_barabasi](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial04_classic_barabasi/INFILE.yaml).

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/ENwr6_pTo0k" frameborder="0" allowfullscreen></iframe>
</center>

To run the Classic Barabasi configuration, we set:

* follow_model: twitter_suggest
* use_barabasi: true
* barabasi_connections: 1 
* all agent_type follow_rate:  0

Until now we've had the number of agents within the network remain constant through the simulation.  Now we are going to have the number of agents increase over time, by setting initial_agents to 10, and max_agents to 1000. 

For simplicity, we will only use 'Standard' agent_types. We must set the 'Standard' agent_type's follow rate to 0.0, so that the only manner in which agents are connecting with each other is through the barabasi_connections they are assigned to make.  In **#k@** allowing follow_back or following_through_retweets may permit the agent to follow even though their follow_rate is zero.


```python
analysis:
  initial_agents:
    10
  max_agents: 
    1000
  max_time: 
    1000
  max_analysis_steps: 
    unlimited
  max_real_time: 
    1
  enable_interactive_mode:
    false
  enable_lua_hooks: # Defined in same file as interactive_mode. Can slow down simulation considerably.
    false
  lua_script: # Defines behaviour of interactive mode & lua hooks
    INTERACT.lua
  use_barabasi: 
    true
  barabasi_connections: # number of connections we want to make when use_barabasi == true
    1
  barabasi_exponent:
    1
  use_random_time_increment: 
    true
  use_followback: 
    false        
  follow_model: 
    twitter_suggest
 
  model_weights: {random: 0.0, twitter_suggest: 0.0, agent: 0.0, preferential_agent: 0.0, hashtag: 0.0}
			  # model weights ONLY necessary for follow method 'twitter' 
  stage1_unfollow: 
    false
  unfollow_tweet_rate: 
    10000
  use_hashtag_probability:
    0.5
```

In the **rates** subsection of the **INFILE.yaml**, we will change the 'add' rate value to 1.0, so that one agent will be added to the network per simulated minute. 

```python
rates:
  add: {function: constant, value: 1.0}
```

Now we will also look at follow_ranks.  Generally, agents are grouped into 'bins' to simplify graphing and for data collection.  Many agents are placed in a bin based on their similarity (ie similar number of tweets, retweets & followers). 

The follow_ranks are essential to the twitter_suggest follow model because new following is based on an agent's existing number of followers.  Therefore, we must know the exact number of followers a person has in order to decide if they get new followers.  The follow_rank bins change range, from 0 to maximum, and to increment by 1.  There may still be many agents per bin.

```python
tweet_ranks: 
  thresholds: {bin_spacing: linear, min: 10, max: 500, increment: 10}

retweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 500, increment: 10}

follow_ranks:
# MUST be adjusted for max_agents for simulations which implement the twitter_suggest and/or preferential_agent follow models
  thresholds: {bin_spacing: linear, min: 0, max: 10000, increment: 1}
  weights:    {bin_spacing: linear, min: 1, max: 10001, increment: 1}
```

#### Running and Visualizing The Network

Let's now run this simulation.

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/Joezk6X4XPU" frameborder="0" allowfullscreen></iframe>
</center>

After the simulation is run, to plot the log-log graph of the **cumulative-degree_distribution_month_000.dat** in [Gnuplot](http://gnuplot.sourceforge.net/), type the commands:

`set style data linespoints`

`set title 'log of Cumulative-Degree Distribution'`

`set xlabel 'log(Cumulative-Degree)'`

`set ylabel 'log(Normalized Cumulative-Degree Probability)'`

`plot 'cumulative-degree_distribution_month_000.dat' u 3:4 title ''`

This gave us the following graph:

<center>
<img src='../img/tutorial04_classic_barabasi/cumulative-degree_distribution_month_000.svg'>
</center>

We can see that this produces a fairly linear plot with a negative slope.  This would be expected.  Agents with few followers would be many and agents with many followers would be few. 

Plotting these data points with a line of best fit up to a **log(Cumulative Degree)** of 2.5, we can see that this simulation partially models a ['Scale-free' network](https://en.wikipedia.org/wiki/Scale-free_network), since the line of best fit  has a slope between -2 to -3.

<center>
<img src='../img/tutorial04_classic_barabasi/cumulative-degree_distribution_fit.svg'>
</center>

The commands used to graph this plot in [Gnuplot](http://gnuplot.sourceforge.net/) were:

`f(x) = m*x + b`

`set xrange[0:2.5]`

`set title 'log of Cumulative-Degree Distribution Fit'`

`set xlabel 'log(Cumulative-Degree)'`

`set ylabel 'log(Normalized Cumulative Degree Probability)'`

`fit f(x) 'cumulative-degree_distribution_month_000.dat' u 3:4 every ::1::13 via m,b`

`plot f(x), 'cumulative-degree_distribution_month_000.dat' u 3:4 title ''`

A visualization of this network is shown below:

<center>
<img src='../img/tutorial04_classic_barabasi/visualization.png'>
</center>

As we can see, we have the much more highly connected agents at the centre of the visualization, and the agents lower in degree on the sides. By implementing the Classic Barabsi configuration
every agent has at least one connection.

### Non-Classic Barabasi

The Non-Classic Barabasi configuration is exactly the same as the Classic configuration except that the number of connections new agents make when entering the simulation is more than 1.

The **INFILE.yaml** file we will create in this example can be found for reference in the [/hashkat/docs/tutorial_input_files/tutorial04_nonclassic_barabasi](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial04_nonclassic_barabasi/INFILE.yaml).

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/JErCat4Qdbw" frameborder="0" allowfullscreen></iframe>
</center>

#### Constructing The Network

We use the same **INFILE.yaml** as above except that barabasi_connections is set at 2 instead of 1.

#### Running and Visualizing The Network

Running this network simulation, we produced the visualization shown below:

<center>
<img src='../img/tutorial04_nonclassic_barabasi/visualization.png'>
</center>

This network similar to the one produced using the Classic Barabasi configuration, with the more highly connected agents at the centre and those less connected on the sides.  However this is a much more connected network since every agent has at least two connections.

### Other Twitter_Suggest Models

We shall now run a **twitter_suggest** follow model network simulation without implementing the Barabasi configuration. The **INFILE.yaml** file that we will create in this example can be found for reference in the [/hashkat/docs/tutorial_input_files/tutorial04_other](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial04_other/INFILE.yaml).

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/NNK9bQ4K9aU" frameborder="0" allowfullscreen></iframe>
</center>

#### Constructing The Network

A twitter_suggest follow model may be implemented without a Barabasi configuration. Set:

* use_barabasi: false
* change 'Standard' agent_type follow_rate to 0.01


#### Running and Visualizing The Network

Running **#k@**, we produced the following visualization of this simulation:

<center>
<img src='../img/tutorial04_other/visualization.png'>
</center>

As we can see, we again have some highly connected agents in the centre of the visualization and less connected agents on the sides. However, as opposed to the Barabasi configurations, there is no set number of connections that every new agent will make, which explains the presence of agents with 0 connections.

To learn more about this network we look into the **Categories_Distro.dat** file.

<center>
<img src='../img/tutorial04_other/categories_distro.png'>
</center>

In this file we see that the majority of agents made 10 or fewer tweets. Retweeting was disallowed, so the retweet ranks have zero agents. We also see how many agents had a certain number of followers. Looking further through this file, we can see that the most followed agent had 86 followers.

<center>
<img src='../img/tutorial04_other/categories_distro1.png'>
</center>

A plot of the follow_ranks up to a follow_rank of 15 is shown below:

<center>
<img src='../img/tutorial04_other/follow_ranks.svg'>
</center>

#### Next Steps

We have now worked with a few configurations of the **twitter_suggest** preferential attachment follow model.  Proceed to the next tutorial, where we demonstrate the [agent follow_model](http://docs.hashkat.org/en/latest/tutorial05/).