[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Random Follow Model

A discussion on follow models and an exercise using the random follow model, this tutorial should take approximately 30 minutes to complete.

There are six different follow models that have been implemented into ***#k@***, to allow greater flexibility and control in the networks the project simulates.
The six follow models are:

* Random
* Twitter Suggest
* Agent
* Preferential Agent
* Hashtag
* Twitter

Each of these follow models will be discussed and analyzed in each of their own tutorial, starting with this one, where we study the random follow model.
The input data used to create the networks will be relatively the same for each follow model tutorial, so that the results from running each different follow model
can be easily compared and constrasted.

As the name implies, a random follow model is one in which the agents in which a twitter user follows is totally arbitrary. Based on the work of Paul Erdos and Alfred Renyi, you can read up on the Erdos-Renyi Model [here](https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model). You've already had some experience working with the random follow model, the simple network we created in [**Tutorial 1](http://docs.hashkat.org/en/latest/tutorial01/) implemented it, but in this tutorial we are going to go into this model in much more detail. We are going to actually go through the input file and make some changes
to create a simulation of our desired framework.

For reference, the *INFILE.yaml* file we will create in this simulation can be found in the *docs/tutorial_input_files* directory in hashkat, under the title *tutorial03*.
You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial03/INFILE.yaml).
For more information on what goes into the input file, and what sort of information you can output from your simulation, check out the [**Input**](http://docs.hashkat.org/en/latest/input/) and
[**Output**](http://docs.hashkat.org/en/latest/output/) page respectively.

## Constructing The Network

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/k6s25BWN4d8" frameborder="0" allowfullscreen></iframe>
</p>

Let's go into the INFILE.yaml file in the hashkat directory and make some changes. Using the INFILE.yaml used in [**Tutorial 1**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial01/INFILE.yaml) as a starting point,
we'll keep the number of agents within the
simulation constant by having 1000 initial and maximum agents. For quickness, we'll lower the maximum simulated time to 1000 simulated minutes.
We'll keep the maximum analysis steps to be unlimited for the most refined simulation, and also keep the max real time to 1 minute since we don't 
want the simulation to run for any longer than that. Interactive mode shall remain off, so both *enable_interactive_mode* and
*enable_lua_hooks* shall be *false* and the lua script will be irrelevant.*use_barabasi* we'll be used in [Tutorial 4](http://docs.hashkat.org/en/latest/tutorial04/),
so this shall be kept *false* and the Barabasi connections will therefore also be inconsequential.
The Barabasi exponent will remain at 1 since this gives the most realistic results. *use_random_time_increment* will be set to true
for superior results. *use_followback* will remain *false*, and the follow model will remain *random*, with the model weights,
which are only necessary for the twitter follow model, being ignored. We set them all to 0 for this example just for clarity,
but they will have no impact on this simulation either way because we are running a random, not a twitter, follow model.
We're going to not allow unfollowing to occur in this example,
but you are encouraged to experiment with it when creating your own network simulations, so we're going to set *stage1_unfollow* to false
and the unfollow tweet rate to an exceptionally high number for this simulation like 10,000 tweets.
We have also kept the probability of hashtag presence in tweets to be 50%.

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
  enable_lua_hooks: # Defined in same file as interactive_mode. Can slow down simulation considerably.
    false
  lua_script: # Defines behaviour of interactive mode & lua hooks
    INTERACT.lua
  use_barabasi: 
    false
  barabasi_connections: # number of connections we want to make when use_barabasi == true
    100
  barabasi_exponent:
    1
  use_random_time_increment: 
    true
  use_followback: 
    false        # from literature, it makes sense for a realistic system to have followback enabled
  follow_model: # See notes above
    random
  # model weights ONLY necessary for follow method 'twitter'  
  model_weights: {random: 0.0, twitter_suggest: 0.0, agent: 0.0, preferential_agent: 0.0, hashtag: 0.0}

  stage1_unfollow: 
    false
  unfollow_tweet_rate: 
    10000
  use_hashtag_probability:
    0.5
```

Since the number of agents within this simulation will remain constant, the add rate will therefore be 0. The output section of INFILE.yaml
will also be left as is, to produce as many data files for analysis as possible. The tweet and retweets ranks have remained the same,
though their purpose is just to organize them and has no real impact on the simulation. The follow ranks max threshold
and weight have also remained the same, with their minimum values being 0 and 1 and their maximum values being a number greater than the number of agents
and that value increased by one. Though this will not impact this simulation, it is very important for the twitter suggest follow model, and will therefore be
elaborated on in [**Tutorial 4**](http://docs.hashkat.org/en/latest/tutorial04/). The *tweet_observations*
section of this program will be kept as is for all of these tutorials, though you are encouraged to change it and experiment with it
when running your own simulations. More information on this can be found on the [**Input**](http://docs.hashkat.org/en/latest/input/) page.

The *ideologies* and *regions* sections have remained the same, but we have added another agent type to our input file. This *Celebrity* agent type is of course intended to mimic a celebrity user, while a
*Standard* agent is used to portray a typical agent on a social network. In contrast to the *Standard* agents from [**Tutorial 1**](http://docs.hashkat.org/en/latest/tutorial01/), the *Standard* agents in this
simulation will have a *followback_probability* of 0, just to illustrate that they will only follow other agents through the follow model, though we did already have *use_followback* set to false,
and a constant follow rate and tweet rate of 0.01. The *Celebrity* agent type will have all of the same input values as the
*Standard* agent type, except that its add weight will be zero. Though we have included them in the input file, the *Celebrity* agents will not be used until [**Tutorial 5**](http://docs.hashkat.org/en/latest/tutorial05/), where agents will follow other agents based on their agent type.  

```python
agents:
  - name: Standard
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
      add: 0
      # Weight with which this agent is followed in agent follow
      follow: 5
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

<p align = 'center'>
<iframe width="560" height="315" src="https://www.youtube.com/embed/6-GtOReq6Cg" frameborder="0" allowfullscreen></iframe>
</p>

Running this simulation, and visualizing it, we produce a network similar to the one shown below: 

<p align='center'>
<img src='../img/tutorial03/visualization.png'>
</p>

As we can quite clearly see this a truly random network, with no clear agent or group of agents being the most popular.

## Network Output

With agents having a follow rate of 0.01 follows per simulated minute in a simulation 1000 simulated minutes long, we would expect most agents to have a cumulative degree distribution of 20. Let's plot this up using 'gnuplot', using data points instead of filled curves to graph our data. The commands to do this are shown below:

`set term svg`

`set title 'Cumulative Degree Distribution'`

`set xlabel 'Cumulative Degree'`

`set ylabel 'Normalized Cumulative Degree Probability'`

`plot 'cumulative-degree_distribution_month_000.dat' title ''`

Giving us:

<p align='center'>
<img src='../img/tutorial03/cumulative-degree_distribution_month_000.svg'>
</p>

As we can see, there is a definite peak surrounding 20 degrees, though it seems that more agents have a cumulative degree just below or just above 20.

## Running a Network Simulation with a Random Seed

What's interesting about ***#k@*** is that if you were to remove the corresponding *network_state.dat* file and *output* files for this simulation and ran it again, you would receive the exact same output as you did before. That is because this simulation we always run with the same seed. A seed is a value used to initialize a random number generator. If the same seed is used every time you run the same network simulation, you will get the exact same output since the exact same random numbers will be used for the simulation. To run a network simulation with a random seed, thereby giving you a different output, type in the command:

`./run.sh --rand`

Running this network simulation multiple times using different seeds, we produced the following cumulative degree distribution plot:

<p align='center'>
<img src='../img/tutorial03/cumulative-degree_distribution_random.svg'>
</p>

As we can see, the cumulative degree distribution of a random follow model network simulation matches the ['Poisson Distribution'](https://en.wikipedia.org/wiki/Poisson_distribution) very well.

## Next Steps

With the completion of this tutorial, we have worked with the simplest of the six types of follow models.
Though we went through it in great detail, this example was just the tip of the iceberg of all the random follow model simulations you can
create. Try running your own random follow model simulation different from the one outlined above, and see what you can create.

When ready, move on to the next tutorial, where things get a lot more interesting and a little more complicated with the
[twitter_suggest follow model](http://docs.hashkat.org/en/latest/tutorial04/).