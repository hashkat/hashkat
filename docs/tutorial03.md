[hashkat.org](http://hashkat.org)

# The Random Follow Model

There are six different follow models in **#k@**, to model how agents chose to follow other agents. Each of these follow models will be discussed in their own tutorial, starting with this one relating to the random follow model. The input data used to create the networks will be almost the same for each follow model tutorial, so that the results may be easily compared.  This tutorial should take approximately 20 minutes.

The six **#k@"" follow models are:

* Random
* Twitter_Suggest
* Agent
* Preferential_Agent
* Hashtag
* Twitter (Combination)

As the name implies, a random follow model is one in which the agents choice of whom to follow is arbitrary, or random, and is based on the model of [Paul Erdos and Alfred Renyi](https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model). The simple network we created in [**Tutorial 1](http://docs.hashkat.org/en/latest/tutorial01/) used the random follow model.  In this tutorial we are going to go into this model in more detail. We are also going to configure the input file to create the network we desire.

The file for configuration is called **INFILE.yaml**.  The infile we create in this tutorial can be found in **hashkat/docs/tutorial_input_files/tutorial03**, [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial03/INFILE.yaml). 

For more information on configuring your simulation, check the [Input](http://docs.hashkat.org/en/latest/input/) and [Output](http://docs.hashkat.org/en/latest/output/) pages.

### Constructing The Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/xx2-3Lh0Dps" frameborder="0" allowfullscreen></iframe>
</center>

To configure the simulation we will start with Tutorial 1's [INFILE.yaml](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial01/INFILE.yaml).  If we have not changed the variable, it may not be shown below.

This simulation will be short and unrealistic.  We'll keep the number of agents constant by having 1000 initial_agent and maximum ('max') max_agents. For speed, we'll lower the max_simulated_time to 1000 minutes and keep the max_real_time at 1 minute. We'll let the max_analysis_steps be 'unlimited' to obtain the most refined simulation. 

Enable_interactive mode shall be 'false' (off), so enable_lua_hooks must also be 'false', and the lua_script setting will be ignored by the compiler.  

We will set use_barabasi to 'false' therefore barabasi_connections will be ignored by the compiler.  The barabasi_exponent will remain at 1 since this gives the most realistic results.  We will set use_barabasi to 'true' in [Tutorial 4](http://docs.hashkat.org/en/latest/tutorial04/). 

We shall set use_random_time_increment to 'true' for superior results.  use_followback will remain *false* for simplicity. 

The follow model will be 'random'. Model_weights are only necessary for the twitter follow model, [Tutorial 8](http://docs.hashkat.org/en/latest/tutorial04/), and will be ignored.  We set them to 0 in this example simply for clarity. 

We're going to not allow 'unfollowing', so we set stage1_unfollow to 'false' and the 'unfollow_tweet_rate' to an exceptionally high number (i.e. 10,000).  We have set the chance of a hashtag in a tweet at 50%.

The code looks like this (note that items after **#** are comments to assist the user and are ignored by the compiler).

```python
analysis:
  initial_agents:
    1000
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
  enable_lua_hooks: 	# only compiled when enable_interactive_mode: true
    false
  lua_script: 		# only compiled when enable_interactive_mode: true
    INTERACT.lua
  use_barabasi: 
    false
  barabasi_connections:	# only compiled when use_barabasi: true
    100
  barabasi_exponent:
    1
  use_random_time_increment: 
    true
  use_followback: 
    false       
  follow_model: 
    random 
  model_weights: {random: 0.0, twitter_suggest: 0.0, agent: 0.0, preferential_agent: 0.0, hashtag: 0.0}
			# model weights are ONLY necessary for follow_method 'twitter' 
  stage1_unfollow: 
    false
  unfollow_tweet_rate: 
    10000
  use_hashtag_probability:
    0.5
```

Since the number of agents in this simulation will remain constant, the *add* rate will be 0 (not shown).  We will only use one type of agent ('Standard').  The 'Celebrity' agent_type is named but not added ('add: 0').  'Celebrity' agents will be used in [Tutorial 5](http://docs.hashkat.org/en/latest/tutorial05/) and onward.

In this tutorial we will leave the the output section of **INFILE.yaml** as in Tutorial 1, to produce as many data files for analysis as possible. 

The tweet and retweets rates are as in Tutorial 1, 1 per 100 simulated minutes.  The follow_ranks_threshold and _weight entries, ideologies and regions sections have also remained the same.

We have added the 'Celebrity' agent type to our input file, by simply copying the 'Standard" agent_type and renaming it.  We will not be adding this agent_type until Tutorial 4 so configuration can be done later.

We have changed the 'Standard' followback_probability to 0, though this is irrelevant since we have use_followback set to false. This is to give consistency to later tutorials.


```python
agents:
  - name: Standard
    weights:
      add: 100.0    			# Weight with which this agent is created
      follow: 5				# Weight with which this agent is followed in agent follow
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
    followback_probability: 0		# Probability that following this agent results in a follow-back
    hashtag_follow_options:
      care_about_region: false 		# does the agent care about where the agent they will follow is from?
      care_about_ideology: false 	# does the agent care about the ideology of the agent they will follow?
    rates: 
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.01}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.01}

  - name: Celebrity
    weights:
      add: 0
      follow: 5
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
    followback_probability: 0 # Probability that following this agent results in a follow-back
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        follow: {function: constant, value: 0.01} 
        tweet: {function: constant, value: 0.01}
```

### Running and Visualizing The Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/16tuo4YkVbo" frameborder="0" allowfullscreen></iframe>
</center>

Running this simulation, and visualizing it, we produce a network similar to the one shown below: 

<center>
<img src='../img/tutorial03/visualization.png'>
</center>

As we can quite clearly see this a truly random network, with no clear agent or group of agents being the most popular.

### Network Output

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/b-skHf5rnpk" frameborder="0" allowfullscreen></iframe>
</center>

With agents having a follow rate of 1/100 simulated minutes, in a simulation 1000 sim_minutes long, we would expect most agents to have an in_degree of 10 (ten people they follow), and an out_degree of 10 (10 people following them) for a cumulative_degree distribution of 20. 

Let's plot this distribution using **Gnuplot**, with data points instead of filled curves. In Gnuplot the commands to do this are:

`set title 'Cumulative-Degree Distribution'`

`set xlabel 'Cumulative-Degree'`

`set ylabel 'Normalized Cumulative-Degree Probability'`

`plot 'cumulative-degree_distribution_month_000.dat' title ''`

Giving us:

<center>
<img src='../img/tutorial03/cumulative-degree_distribution_month_000.svg'>
</center>

As we can see, there is a definite peak surrounding 20 degrees, though it seems that more agents happen to have a cumulative-degree just below or just above 20 for this simulation. 

To run the EXACT SAME simulation again, remove the output files **network_state.dat** and **output** from this simulation and run it again.  It will give the EXACT SAME output.  The same seed for the random number generator will be used.

### Running a Network Simulation with a Random Seed

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/_36T2gGvrEw" frameborder="0" allowfullscreen></iframe>
</center>

To run a network simulation with a random seed, thereby giving you a different output each time you run the simulation, when you run the program, type it in with the suffix space-dash-dash rand:

`./run.sh --rand`

Running this network simulation multiple times, each time using  './run.sh --rand', we produced the following cumulative-degree distribution plot:

<center>
<img src='../img/tutorial03/cumulative-degree_distribution_random.svg'>
</center>

As we can see, the cumulative-degree distribution of a random follow model matches the ['Poisson Distribution'](https://en.wikipedia.org/wiki/Poisson_distribution).

To create this plot, we ran three simulations with random seeds and renamed their cumulative-degree distributions as cumulative-degree_distribution_month_000.a, cumulative-degree_distribution_month_000.b, and cumulative-degree_distribution_month_000.c.

We input the following into Gnuplot:

`poisson( x , mu ) = exp(-mu) * mu**(x) / gamma(x+1)`

`set xrange [0:50]`

`set title 'Cumulative-Degree Distribution for Simulations with Different Seeds'`

`set xlabel 'Cumulative-Degree'`

`set ylabel 'Normalized Cumulative-Degree Probability'`

`plot poisson( x , 20 ) title 'Poisson Distribution',`
`'cumulative-degree_distribution_month_000.a' title '',`
`'cumulative-degree_distribution_month_000.b' title '',`
`'cumulative-degree_distribution_month_000.c' title ''`

Note: your output will vary, especially if you are using random seeds.

### Next Steps

Try running your own *random* follow model simulation that is different from the one outlined above, and see what you can create.

When ready, move on to the next tutorial, where things get a lot more interesting and a little more complicated with the [twitter_suggest follow model](http://docs.hashkat.org/en/latest/tutorial04/).