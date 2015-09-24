[hashkat.org](http://hashkat.org)

# Input

This page lists all user modifiable configurations in the input file **INFILE.yaml**. 

## Analysis

The first section of this program outlines the general focus of this simulation. Let's go over what each variable means.

### Initial Agents

`initial_agents:
  20`

The **initial_agents** are therefore the initial users you will have at the start of the simulation. As we can see, in the above case, this value is 20.

### Max Agents

`max_agents:
  100001`

This is the maximum number of users that are permitted in the simulation. Once this number is reached, the number of agents in the network will no longer increase. In this case we can see its value is 100001.

### Max Time

`max_time:  hour`

This variable refers to the maximum **simulated** time in **minutes** for the simulation to run.  You may also describe time as *minute*, *hour*, *day*, or *year* multiplied by a constant or each other and **#k@** will convert the value to minutes (e.g. 3 days can be input as 3*day, which is equal to 4,320 minutes). 

Note: interacting with the network simulation DOES NOT affect simulated time elapsed.

### Max Analysis Steps

`max_analysis_steps:  unlimited`

This is the maximum number of loops undertaken in the the simulation. To achieve the most accurate results possible, it is recommended to keep this value at unlimited.

### Max Real Time

`max_real_time:   hour`

This sets the maximum **real** time you will permit the simulation to run. Once this actual time has elapsed, the simulation ends. This time is in **seconds**, and may be input in the same way as **max_time** above.

Note: interacting with the network simulation during the run DOES affect real time elapsed.


### Enable Interactive Mode

`enable_interactive_mode:  true`

If 'true', the user may interact with the simulation while it is running, while 'false' prevents interaction.

### Enable Lua Hooks

`enable_lua_hooks: false`

If set to 'true', runtime Lua functions are enabled that can react to events such as a 'tweet', 'retweet', and 'follow', while 'false' does not. 

Note: setting this to 'true' will slow down the simulation immensely.

### Lua Script

`lua_script: INTERACT.lua`

**lua_script** contains all the script for interactive mode. It is highly recommended to keep this variable as 'INTERACT.lua', since that is the default Lua script in the **hashkat** directory.

### Use Barabasi

`use_barabasi:  false`

A setting of 'false' allows agents in the simulation to 'follow' as many other agents as the simulation allows.  If this variable is set to 'true', the user may impose a maximum number of connections in **barabasi_connections** below.

### Barabasi Connections

`barabasi_connections:  100`

If **use_barabasi** is set to 'true', this variable limits the number of follows an agent may make in a network simulation.

For a **Classic Barabasi** configuration, set this value equal to 1. 

For a **Non-Classic Barabasi** configuration, set this value greater than 1.

### Barabasi Exponent

`barabasi_exponent:  1`

The value assigned to the exponent of every agent's **cumulative_degree** within the network.

### Use Random Time Increment

`use_random_time_increment: true`

If set to 'true' time increases in random increments modelled by the **KMC** loop.  An input of 'false' makes time increase by a constant value. A setting of 'true' generally gives more refined results.

### Use Followback

`use_followback: true`

If set to 'true', agents are permitted to follow agents who follow them.  

### Use Follow via Retweets

`use_follow_via_retweets:  true`

If set to 'true', agents are permitted to...

### Follow Model

`follow_model:  random`

The input options for **follow_model** are: 'random', 'twitter_suggest', 'agent', 'preferential_agent', 'hashtag', and 'twitter'.  

The **follow_model** determines how agents chose whom to follow.  The **follow_model** option will have a significant effect on the outcome of your simulation.

### Model Weights

`model_weights: {random: 0.20, twitter_suggest: 0.20, agent: 0.20, preferential_agent: 0.20, hashtag: 0.20}`

This variable is only relevant when using the **twitter** follow model. The **twitter** follow model is a combination of all the other follow models.  This variable allows weighting of each follow model in the simulation, to a maximum combined weight of 1.0.

### Stage1 Unfollow

`stage1_unfollow: false`

Inputting 'true' enables agents to 'unfollow' agents that have a tweet rate double the average tweet rate of the other agents followed by the agent. Inputting 'false' prevents an agent from unfollowing for 'chattiness'.

This variable allows an agent to deliberately unfollow a 'chatty' tweeter, where 'chattiness' is defined as exceeding a **relative average** tweet rate, calculated based on the sub-set of agents the agent follows.

### Unfollow Tweet Rate

`unfollow_tweet_rate: 10`

The **unfollow_tweet_rate** describes a **constant** tweet rate that, if exceeded, will cause the tweeting agent to **randomly** lose a follower. To prevent this, set this much greater than the agent 'tweet_rate' in the simulation.

### Use Hashtag Probability

`use_hashtag_probability:  1`

The probability of a tweet containing a hashtag(**#**) may range from 0 to 1 (0% to 100%).

### Rates

The 'add' rate at which agents will be added per simulated minute during the simulation. This function can be constant or linear.

`rates:  add: {function: constant, value: 0.0}`

For a constant 'add' rate, the function must be identified as **constant** and the number of agents you wish to add per simulated minute is the 'value'. This number can zero or greater.  Although described as **constant** this function describes a rate.  A zero rate would not add agents during the simulation.

`rates:  add: {function: linear, y_intercept: 1.0, slope: 0.1}`

For a linear 'add' function, the slope and y-intercept must be specified.  The classic linear function is **y = ax +b**, where **'a'** is the slope, **'x'** the variable, and **'b'** the y-intercept. 

In **#k@**, the 'y_intercept' is the initial rate at which agents are added (the 'value' of the **constant** function above).  The 'slope' is the rate at which the 'add' rate increases over time, where time ('x') is in simulated months.

The 'follow' and 'retweet' rates work in the same manner.

## Output

This section of the **INFILE.yaml** defines the output to the output directory once the simulation has concluded. Initially it is recommended to set all these variables to 'true'.  With more experience, unnecessary output variables can be set to 'false'.

### Save Network on Timeout

`save_network_on_timeout: true`

If 'true', all of the simulated network data will be stored in the **save_file** outlined below should a timeout occur.

### Load Network on Startup

`load_network_on_startup: true`

If 'true', all of the data stored in the **save_file** will be loaded when the network simulation recommences, and the simulation will continue. If 'false', the simulation will simply restart from the beginning.

### Ignore Load Config Check

`ignore_load_config_check: true`

If 'true', allows you to load a network from the **save_file** and continue the simulation using an input file with different parameters. Setting this to 'false' will prevent you from doing this.

### Save File

`save_file: network_state.dat`

Determines the name of the file where the simulated data will be saved to.

### Stdout Basic

`stdout_basic: true`

If 'true', when a network simulation is stopped or completed, a message will print to the screen.  If the simulation ends, the message will state why the simulation ended, that analysis files are being created, and that analysis can be aborted by pressing **Ctrl-c** multiple times.  If the analysis files have been created, the message will state the analysis is complete.

### Stdout Summary

`stdout_summary:  true`

If 'true', the length of simulated time (in simulated minutes), the number of agents, 'follows', 'tweets', 'retweets', and 'unfollows', the cumulative rate function **R**, and the real time elapsed in seconds during the simulation will be displayed on the screen **throughout** the simulation's run. This information is updated at a rate determined by the **summary_output_rate** described below.

### Summary Output Rate

`summary_output_rate:   100`

This defines how often the **stdout_summary** information will be updated on screen during a simulation.  The value is based on the loops in the **KMC**. In this case, we can see that information will update every hundred loops or steps.

### Visualize

`visualize:  true`

If 'true', the simulation will output two additional output files, **network.dat** and **network.gexf**. The file **network.dat** shows every individual follower each agent has. The file **network.gexf** can be used to visualize simulated networks with third party programs such as [Gephi](http://gephi.github.io/).

### Agent Stats

`agent_stats:  true`

If 'true', the simulation will produce additional output files for each **agent type** within the simulation, as **agenttype_info.dat** (i.e. a **Standard** agent type will result in the file **Standard_info.dat**).

### Degree Distributions

`degree_distributions:  true`

If 'true', the **in-degree**, **out-degree**, and **cumulative-degree** distributions will be output with a file for each simulated month. In the case of the zeroth month, **in-degree_distribution_month_000.dat**, **out-degree_distribution_month_000.dat**, and **cumulative-degree_distribution_month_000.dat** respectively.

### Tweet Analysis

`tweet_analysis:  true`

Setting to 'true' creates two output files, **tweets_distro.dat** and **retweets_distro.dat**, which contain the distributions for the tweets and retweets that agents have made in the network simulation.

### Retweet Visualization

`retweet_visualization:  true`

If 'true', the network simulation will produce an output file titled **retweet_viz.gexf**, which can be used to visualize the most popular tweet (most 'retweeted') in third party programs such as [Gephi](http://gephi.github.io/).

### Main Statistics

`main_statistics:  true`

Setting to 'true' creates the **main_stats.dat** output file, which contains all principal information from a given simulation.

### Degree Distribution by Follow Model

`degree_distribution_by_follow_model: true`

If 'true', creates the **dd_by_follow_model.dat** output file, which shows the normalized probability and the normalized probability logarithm that an agent of a particular cumulative-degree made its connections via each particular follow method.

### Region Connection Matrix

`region_connection_matrix:  true`

Setting to 'true' creates the **region_connection_matrix.dat** output file, which contains a chart showing the connections made between regions.

### Categories Distribution

`categories_distro:  true`

If 'true', creates the **Categories_Distro.dat** output file, which shows how many agents have made a certain number of tweets and retweets and how many agents have a particular number of followers.

### Most Popular Tweet Content

`most_popular_tweet_content:  true`

Setting to 'true' creates the **most_popular_tweet_content.dat** output file, which contains details on the most retweeted tweet and its author.

## Ranks

This section details how agents may be ranked according to their number of tweets, retweets, and number of followers.

### Tweet Ranks

`tweet_ranks: thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}`

Serves as a way to categorize agents by their number of tweets. The 'bin_spacing' may be **linear**, **quadratic**, or **cubic**, which means that bins can be spaced by the 'increment' raised to the power of 1, 2, or 3 respectively. The **tweet_ranks** can also be set with a minimum, maximum, and increment value. 

In the above case, with tweeting enabled, agents with 10 or less tweets will be stored on a list in the initial bin. If some of these agents continue to tweet, and end up having a total number of tweets greater than 10 but less than 20, they will move to the next bin. Agents can keep moving bins until they have made (in this case) 291 tweets, since they will be placed in the final bin where agents with 300 or less tweets are placed and remain there. If tweeting is disabled, the number of agents in each tweet rank wll be listed as zero.

### Retweet Ranks

`retweet_ranks: thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}`

Similarly, this case categorizes agents based on their number of retweets.

### Follow Ranks

`follow_ranks:  thresholds: {bin_spacing: linear, min: 0, max: 100001, increment: 1}`
`weights:    {bin_spacing: linear, min: 1, max: 100000, increment: 1}`

This rank serves to categorize agents based on the number of followers they have. However the bins are **weighted**. Here the bins have a weight equal to its corresponding value increased by 1 (assuming the 'bin_spacing' is 'linear' and 'increment' is 1). 

The weights are essential in the **twitter_suggest** and **preferential_agent** follow models, since they are what cause agents with a higher **in-degree** (number of followers) to have a better chance of being followed than agents with fewer followers. 

## Tweet Observations

Tweets lose relevance over time, to the point where they will no longer be retweeted. This section deals with the decay function for retweet rates.

### Density Function

`density_function: 2.45 / (x)**1.1`

This is the function which describes how retweet rates decay over time.  This function is integrated and each discrete element is divided by the total integral to normalize the function and ensure that it truly is a [probability density function](https://en.wikipedia.org/wiki/Probability_density_function).  Agents that can still be retweeted are binned according to the values determined by this function. As time progresses, they are moved to bins of smaller retweet rates. The discrete elements of this function can be adjusted below.

### x_start

`x_start:  5`

This is the initial value of the density function. Make sure that the value you choose for **x** does not cause a discontinuity (e.g. using **x = 0** would cause the above density function to be divided by 0, causing a discontinuity).

### x_end

`x_end:  600`

This is the final value of the density function. Make sure that this value does not cause a discontinuity in the function.

### Initial Resolution

`initial_resolution:  1.0`

Determines the initial boundary spacing for integration of the density function. For example, here we have **initial_resolution** equal to 1.0 and **x_start** equal to 5, so we could expect the **first** integral to be bounded from 5 to 6 [5,6]

### Resolution Growth Factor

`resolution_growth_factor:  1.05`

Determines the density function integral boundary spacing after the initial resolution. This has been implemented due to a function resolution needing to be much more accurate during the time immediately after a tweet is tweeted as opposed to some time after this has occurred. If you had an **initial_resolution** of 1.0 and a **resolution_growth_factor** of 1.05 with **x_start** set to 5, the integrals would be evaluated at [5,6], [6, 7.05], [7.05, 8.10], etc. 

### Time Span

`time_span:  8*hour`

Determines for how long after a tweet has been made that it can be retweeted. Once this simulated time has elapsed, the density function will disappear, allowing no more retweets or following via retweets to occur for this tweet.

## Agent Attributes

Agents may be given different attributes such as **agent_types**, **ideology**, **regions**, and **preference_class**.  

In **#k@** the number and name of the attributes are found in:

`~\hashkat\src\config_static.h`
	
	N_BIN_IDEOLOGIES = 4;
	N_BIN_PREFERENCE_CLASS = 2;
	N_BIN_REGIONS = 3;
	N_BIN_AGENT_TYPES = 200;

**Language** is an enumerated class in the same file.  

The values of the attributes are configured through **INFILE.yaml**.  If a greater number of, for example, **regions** is desired, the value of the **N_BIN** file must be changed and **#k@** must be rebuilt before the simulation can be run. To build **#k@**, run:

`./build.sh -O`

### Ideologies

Ideologies in **#k@** are named after colours and have no other attributes. The names may be changed. 

Through **INFILE.yaml** the simulation may configure 0-4 **ideologies**.  

Note: if you'd like to increase the number of **ideologies** in the simulation, change the value set in the **N_BIN** file.  If **N_BIN** is changed, remember to then rebuild **#k@** to have these changes take effect.

### Preference Classes

The **preference classes** detail how agents react to tweets in a network simulation. The four possible types of tweets are: 'plain', 'musical', 'ideological', and 'humorous'. 

```python
- name: StandardPref

   tweet_transmission:
      plain:
        Standard: 0.5
        Celebrity: 1.0
      different_ideology:
        all: 0.0        
      same_ideology:
        Standard: 0.3
        else: 0.6
      humorous:
        Standard: 1.0
        Celebrity: 1.0
        else: 0.0
```

Under **tweet_transmission** one can specify the probability that tweets made by a particular **agent_type** ('Standard', 'Celebrity') will be retweeted based on the content of that tweet. Instead of inserting a particular **agent_type**, one can also input 'all' or 'else' for any of the **tweet_transmision** rates.

Note: if you'd like to change the number of **preference classes** in the simulation, make sure this number is less than or equal to the value set in the **N_BIN** file.  If **N_BIN** is changed, remember to then rebuild **#k@** to have these changes take effect.


### Regions

**#k@** has three potential geographic regions, defined in **INFILE.yaml**.

```python
- name: Ontario
    add_weight: 1
    preference_class_weights: {StandardPref: 100, NoRetweetPref: 100}
    ideology_weights: {Red: 100, Blue: 100}
    language_weights: {English: 50, French: 25, French+English: 25}
```

A region's **add_weight** corresponds to the probability that an agent added to the network is from that region, and is weighted with respect to the **add_weights** of all the other regions. 

The **preference_class_weights** are the possible 'preference classes' agents from that region can have, and are weighted with respect to one another. 

The **ideology_weights** are the possible 'ideologies' agents can have, and are weighted with respect to one another. 

The **language_weights** are the possible 'languages' that an agent from that region can speak, and are also weighted with respect to each other.

Note: make sure the number of **regions** is EXACTLY equal to the value set for **N_BIN_REGIONS**.  You may change the value of the **N_BIN**, but remember to then rebuild **#k@** to have these changes take effect.

### Agent Types

In this section, one can create the types of agents that will be included in their network simulation.  There may be up to 200 **agent_types**.

```python
- name: Standard
    weights:
      add: 80
      follow: 5
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
    followback_probability: 0.44
    hashtag_follow_options:
      care_about_region: true
      care_about_ideology: false
    rates:
        follow: {function: constant, value: 0.1}
        tweet: {function: constant, value: 0.01}
```

Under **weights** we can see 'add', which correlates to the probability, weighted against all the weights of the other **agent_types**, that a new agent added is of this **agent_type**. 

The value input into 'follow' will be the probability, weighted against the other **agent_types**, that an agent of this type will be the one followed in the **agent_follow model** when a follow occurs. 

**tweet_type** summarizes the weighted probability that an agent's tweets will be ideological, plain, musical, or humorous. 

The **followback_probability** is the probability that following an agent of this type will result in that agent following you back, though 'use_followback' has to be enabled for this to have any effect. 

The **hashtag_follow_options** are implemented in the **hashtag follow model**, where agents follow other agents based on the hashtags present in their tweets. In this follow model, if **care_about_region** is set to 'true', agents of this type will only follow other agents from the same region.  If **care_about_ideology** is set to 'true', agents will only follow agents that share their ideology. It is important to note that **use_hashtag_probability** (above in the **Analysis** section) must not be zero. 

The 'follow' and 'tweet' rates are the rates at which agents of this particular agent type will 'follow' and 'tweet' per simulated minute. These work in the exact same manner as the 'add' rate outlined in the **rates** section.

Note: the number of **agent_types** in the simulation may be less than or equal to 200. You may increase the value in the **N_BIN**, but make sure to then rebuild **#k@** for these changes to take effect.