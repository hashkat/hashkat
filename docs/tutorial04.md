[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Twitter Suggest Follow Model

Going through several exercises on the different configurations of the twitter suggest follow model, this tutorial should take approximately 50 minutes to complete

Based on Albert-Laszlo Barabasi's research, a twitter suggest follow model is one in which agents tend to follow users with the highest degree, aka the highest number of followers.
The twitter suggest method is influenced heavily by the [Barabasi-Albert Model](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model), generating networks
where agents with the greater number of followers have a higher probability of being followed.
This particular follow model can be implemented using three different types of configurations of this method, each of which is outlined below.

## Example - Classic Barabasi

The Classic Barabasi configuration is one in which agents that are added to the network make one conection with another agent and no other, unless manipulated to do so by allowing follow back or
following through retweets within your simulation.

Let's try running a Classic Barabasi twitter suggest follow model simulation, with our starting point being the INFILE.yaml we used in the previous tutorial.
As always, all the files that we will use in this simulation can be found for reference in the tutorials directory in hashkat, with this one under the title *tutorial04_classic_barabasi*.
You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial04_classic_barabasi/INFILE.yaml).

### Constructing The Network

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/bHvmbQA2fEk" frameborder="0" allowfullscreen></iframe>
</p>

So let's make some modifications to our input file. As opposed to the random follow model simulations, where the number of agents within the network remained constant throughout the simulation,
we are going to have the number of agents within the simulation increase over time, by setting our *initial_agents* to 10, and our *max_agents* to 1000.
Most importantly, we're going to set *use_barabasi* to *true* causing the simulation to implement the Barabasi configuration.
*barabasi_connections* specifies the number of connections an agent makes when entering the simulation, so for this Classic Barabasi example, we're going to set this value to 1.
Since we are running a twitter suggest follow model, we are going to set the *follow_model* as such. In the *rates* section of the input file, we are now adding agents to the network throughout the simulation,
so we will change the add rate value to 1.0, so that one agent will be added to the network per simulated minute.

```python
analysis:
  initial_agents:
    10  # start out from something small, could be when only the developers were online.
  max_agents: 
    1000     # 1 million max users
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
    false        # followback turned on, from literature it makes sense for a realistic system
  follow_model: # See notes above
    twitter_suggest
  # model weights ONLY necessary for follow method 'twitter'  
  # educated guesses for the follow models  
  model_weights: {random: 0.0, twitter_suggest: 0.0, agent: 0.0, preferential_agent: 0.0, hashtag: 0.0}
  
  stage1_unfollow: 
    false
  unfollow_tweet_rate: 
    10000
  use_hashtag_probability:
    0.5    # 50 % chance of using a hashtag
```

As mentioned in the previous tutorial, the follow ranks are essential to the twitter suggest follow model. Agents are placed into bins based on their degree or the number of followers they have.
All the agents in the bin with threshold 0 have 0 followers, while all the agents in bin 200 have 200 followers.The weight of each bin threshold determines the probability that an agent from this bin
will be randomly chosen to be followed in comparison to agents in other bins. The bin thresholds are linearly spacecd in increments of 1 and have a minimum value of 0 and a maximum value equal to the maximum
number of agents within the simulation. The weights of these bins are also linearly spaced in increments of 1, and the minimum bin threshold has a weight of 1 and the maximum bin threshold has a weight
equal to the max number of agents plus one. Therefore, the weighted probability that an agent with 10 followers will be chosen in comparison with an agent with 100 followers is 11 to 101 respectively. So
agents with a greater number of followers have a better chance of being followed by other agents. It is thus very important that the max follow rank threshold be equal to or greater than the number of max agents
within the simulation. If this number is less than that, your network simulation may give inaccurate results and the total number of followers the most popular agent has may be impossible to determine. Since the
minimum threshold follow rank has a weight of 1, the maximum threshold follow rank must have a weight equal to its value increased by one. 

```python
tweet_ranks: 
  thresholds: {bin_spacing: linear, min: 10, max: 500, increment: 10}
retweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 500, increment: 10}
follow_ranks:
  thresholds: {bin_spacing: linear, min: 0, max: 10000, increment: 1}    # for preferential following
  weights:    {bin_spacing: linear, min: 1, max: 10001, increment: 1}
```

To better demonstrate the results of a twitter preferential follow model, we are again only going to use *Standard* users in this simulation. It is imperative that we have also set the *Standard* agent type's follow rate to 0.0,
so that the only manner in which agents are connecting with each other is through the *barabasi_connections* they are assigned to make.

### Running and Visualizing The Network

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/VDL76lvsa0E" frameborder="0" allowfullscreen></iframe>
</p>

Let's now run this simulation. You can plot the log-log graph of the *cumulative-degree_distribution_month_000.dat* in gnuplot, by following the same plotting steps outlined in Tutorial 1 but when plotting typing in
the command:

`plot 'cumulative-degree_distribution_month_000.dat' u 3:4 title ''`

This gave us the following graph:

<p align='center'>
<img src='../img/tutorial04_classic_barabasi/cumulative-degree_distribution_month_000.svg'>
</p>

We can see that this produces a linear plot with a negative slope, which makes sense since we would expect agents with a small degree to be great in number, while agents with a very large degree to
be very small in number. Plotting these data points with a line of best fit up to a *log(Cumulative Degree)* of 2.5, we can see that this network simulation up to a certain point models a ['Scale-free' network](https://en.wikipedia.org/wiki/Scale-free_network), since this line of best fit clearly has a slope between -2 to -3.

<p align='center'>
<img src='../img/tutorial04_classic_barabasi/cumulative-degree_distribution_fit.svg'>
</p>

A visualization of this network is shown below:

<p align='center'>
<img src='../img/tutorial04_classic_barabasi/visualization.png'>
</p>

As we can see, we have the much more highly connected agents at the centre of the visualization, and the agents lower in degree on the sides. As expected, by implementing the Classic Barabsi configuration,
every agent, by following one other user, has at least one connection.

## Example - Non-Classic Barabasi

The Non-Classic Barabasi configuration is exactly the same as the classic configuration except that the number of connections agents make when entering the simulation is a number greater than 1.

The files used in this simulation can be found for reference in the tutorials directory in hashkat under the title *tutorial04_nonclassic_barabasi*.
You can also view the input file used for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial04_nonclassic_barabasi/INFILE.yaml).

### Constructing The Network

For this example, we will use the exact same INFILE.yaml as used above, but our *barabasi_connections* will have a value of 2 instead of 1.

### Running and Visualizing The Network


<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/dFcaSDv--PI" frameborder="0" allowfullscreen></iframe>
</p>

Running this network simulation, we produced the visualization shown below:

<p align='center'>
<img src='../img/tutorial04_nonclassic_barabasi/visualization.png'>
</p>

As expected, this network is quite similar to the one we produced using the Classic Barabasi configuration, with the more highly connected agents at
the centre of the visualization and those less connected on the sides. This is, however, a much more highly connected network, since every
agent has at least two connections.

## Example - Other Twitter Suggest Models

We shall now run a twitter suggest follow model network simulation without implementing the Barabasi configuration.
The files that were used in this example can be found for reference in the tutorials directory in hashkat under the title *tutorial04_other*.
You can also view the input file used for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial04_other/INFILE.yaml).

### Constructing The Network

Designing a twitter suggest follow model network simulation without implementing the Barabasi configuration can be done by simply going into our INFILE.yaml file and inputting 'false' for  *use_barabasi*, and giving the *Standard* agent types a follow rate, which will be 0.01 again for this tutorial.

### Running and Visualizing The Network

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/qViyqLsfnf4" frameborder="0" allowfullscreen></iframe>
</p>

Running hashkat, we produced the following visualization of this simulation:

<p align='center'>
<img src='../img/tutorial04_other/visualization.png'>
</p>

As we can see, we again have some very highly connected agents in the centre of the visualization and unconnected agents on the sides. However, as opposed to twitter suggest follow model simulations that implement the Barabasi configuration, there are no set number
of connections that every agent has, which explains the presence of agents with 0 connections.

Looking into the *Categories_Distro.dat* file, we can see a vast amount of information that includes the following:

<p align='center'>
<img src='../img/tutorial04_other/categories_distro.png'>
</p>

We can see here that the majority of agents made 10 or less tweets, though some made 20 or less. Retweeting was disallowed in this simulation, so the retweet ranks all have zero agents within them. We can also see how many agents have a certain number of followers in this network simulation. Looking further through this file, we can see that the most followed agent has 86 followers, since after that follow rank, there are zero agents present.

<p align='center'>
<img src='../img/tutorial04_other/categories_distro1.png'>
</p>

A plot of these follow ranks up to a follow rank of 15 is shown below:

<p align='center'>
<img src='../img/tutorial04_other/follow_ranks.svg'>
</p>

## Next Steps

We have now worked with a few configurations of the twitter suggest follow method. Though we did not implement agent types with a follow rate when runing Barabasi configurations,
you are encouraged to try doing so, as well running simulations with more agent types, more agents, less regions, etc. The more practice you get using #k@, the better skilled you will be at
producing your ideal network simulations.

Proceed to the next tutorial, where we try using the agent follow model.
