[hashkat.org](http://hashkat.org)

# Input

This section serves as a reference for users working with the input file *INFILE.yaml*. This page goes through every aspect of the file in which users can manipulate.

### **analysis**

The first section of this program is titled analysis, and it outlines what is the general focus of this simulation. Let's go over what each variable means.

#### Initial Agents

```python
initial_agents:
  20
```

Agents correspond to users in the social network. The *initial_agents* are therefore the initial users you will have at the start of the simulation. As we can see, in the above case, this value is 20.

#### Max Agents

```python
max_agents:
  100001
```

The *max_agents* are the maximum number of users that will be in the network simulation. Once this number is reached, the number of agents in the network will no longer increase. In this case we can see its value is 100001.

#### Max Time

```python
max_time:
  hour
```

The *max_time* variable coincides with the maximum simulation time in which you want the simulation to run for. Once this simulated time has been reached, the simulation ends. This time is in simulated minutes, and as we can see for this case, has a value of 1 hour or 60 minutes. For simplicity, you can also just input *minute*, *hour*, *day*, or *year* multiplied by a constant or each other and ***#k@*** will know the value in simulated minutes (e.g. 3 days can be inputted as 3*day, which is equal to 4,320 minutes). Note that interacting with the simulation has no impact on the simulation time.

#### Max Analysis Steps

```python
max_analysis_steps:
  unlimited
```

The *max_analysis_steps* are the maximum number of steps undertaken in the [KMC](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo) loop throughout the simulation. To achieve the most accurate results possible, it is recommended to keep this value at unlimited.

#### Max Real Time

```python
max_real_time:
  hour
```

*max_real_time* is the maximum real itme in which you want the simulation to run for. Once this actual time has elapsed, the simulation ends. This time is in real minutes, and in the above example has a value of 1 hour or 60 minutes. For simplicity, you can also just input *minute*, *hour*, *day*, or *year* multiplied by a constant or each other and ***#k@*** will know the value in minutes (e.g. 3 days can be inputted as 3*day, which is equal to 4,320 minutes). It is important to note that as opposed to the simulated time, interacting with the simulation does affect the real time. 

#### Enable Interactive Mode

```python
enable_interactive_mode:
  true
```

Inputting *true* into *enable_interactive_mode* allows the user to interact with the simulation while its running. Inputting *false* into this variable prevents this.

#### Enable Lua Hooks

```python
enable_lua_hooks:
  false
```

Inputting *true* into *enable_lua_hooks* implements runtime Lua functions that can react to events such as a tweet, a retweet, a follow, etc, while inputting *false* does not. It should be noted that setting this to *true* will slow down the simulation immensely.

#### Lua Script

```python
lua_script:
  INTERACT.lua
```

*lua_script* contains the script in which all the functions able for use in interactive mode can be found. It is highly recommended to keep this variable as *INTERACT.lua*, since that is the default Lua script in the **hashkat** directory.

#### Use Barabasi

```python
use_barabasi:
  false
```

Input *true* into *use_barabasi* to enable agents in a network simulation to only make as many connections as the value set in *barabasi_connections*. Input *false* to prevent this.

#### Barabasi Connections

```python
barabasi_connections:
  100
```

*barabasi_connections* outlines the number of follows an agent will make in a network simulation when *use_barabassi == true*. For a Classic Barabasi configuration, set this value equal to 1. For a Non-Classic Barabasi configuration, set this value to another value greater than 1.

#### Barabasi Exponent

```python
barabasi_exponent:
  1
```

The value assigned to the exponent of every agent's cumulative-degree within the network.

#### Use Random Time Increment

```python
use_random_time_increment:
  true
```

Input *true* for *use_random_time_increment* to have time increase by random values modelled by the [KMC](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo) loop, or input *false* to have time increase by constant increments of time. Inputting *true* generally gives more refined results.

#### Use Followback

```python
use_followback:
  true
```

Inputting *true* into *use_followback* will allow agents to follow other agents that follow them, while inputting *false* will prevent this from happening.

#### Follow Model

```python
follow_model:
  random
```

The *follow_model* determines the manner in which agents choose who to follow. The input options for follow_model are: *random*, *twitter_suggest*, *agent*, *preferential_agent*, *hashtag*, and *twitter*.

#### Model Weights

```python  
model_weights: {random: 0.20, twitter_suggest: 0.20, agent: 0.20, preferential_agent: 0.20, hashtag: 0.20}
```

*model_weights* are only relevant when using the *twitter* follow model. The *twitter* follow model is a combination of all the other follow models, so this allows you to control the proportion of each follow model that is present in the network simulation relative to the others.

#### Stage1 Unfollow

```python
stage1_unfollow:
  false
```

Inputting *true* into *stage1_unfollow* will enable agents to unfollow agents that have a *tweet* rate double the average *tweet* rate of all the other agents they are following. Inputting *false* prevents this from happening.

#### Unfollow Tweet Rate

```python
unfollow_tweet_rate:
  10
```

The *unfollow_tweet_rate* is a value by which, if the number of tweets by an agent exceeds this value, will cause this agent to randomly lose one of its followers. To prevent this from occurring in your network simulation, set this value to a number much greater than the agent *tweet* rates in the network.

#### Use Hashtag Probability

```python
use_hashtag_probability:
  1
```

The probability of a tweet containing a hashtag(#) from 0(0%) to 1(100%). As we can see in this example, the probability of tweets containing a hashtag is 1, so every tweet will have one.

### **rates**

The rate at which agents will be added into the network per simulated minute throughout the course of the simulation, this function can either be *constant* or *linear*.

```python
rates:
  add: {function: constant, value: 0.0}
```

For a constant *add* rate, the *function* must be identified as *constant* and the number of agents you wish to add per simulated minute must be inputted into *value*. This number can zero or greater. For a linear *add* rate, the *add* rate must be changed to:

```python
rates:
  add: {function: linear, y_intercept: 1.0, slope: 0.1}
```

where the *y_intercept* is the initial value of agents added to the network per simulated minute and the *slope*, multipled by the number of simulated months that have elapsed within the simulation, is the value by which the *y_intercept* is increased by.

### **output**

This section of the program outlines what is present in the output directory once the simulation has concluded. To acquire as much data as possible, it is recommended to initally set all these to *true*, and to input *false* to whatever you don't need after having more experience running simulations.

#### Save Network on Timeout

```python
save_network_on_timeout:
  true
```

If *true* all of the simulated network data will be stored in the *save_file* outlined below.

#### Load Network on Startup

```python
load_network_on_startup:
  true
```

If *true* all of the simulated network data stored in the *save_file* will be loaded when the network simulation recommences, and the simulation will thereby continue from where it left off. If *false*, a network simulation will simply restart from the beginning if it is stopped and then restarted.

#### Ignore Load Config Check

```python
ignore_load_config_check:
  true
```

If *true*, allows you to load a network from the *save_file* and continue the simulation using an input file with differing parameters. Setting this to *false* will prevent you from doing this.

#### Save File

```python
save_file:
  network_state.dat
```

Determines the name of the file where all the simulated data is saved to.

#### Stdout Basic

```python
stdout_basic:
  true
```

If *true*, when a network simulation is stopped or completed, a message will print to the screen stating the reason why the simulation was completed, that analysis files are being created and that this can be aborted by pressing **Ctrl-c** multiple times, and, if the analysis files have been created, that the analysis is complete.

#### Stdout Summary

```python
stdout_summary:
  true
```

If *true*, the length of simulated time (in simulated minutes), the number of agents, follows, tweets, retweets, and unfollows that have occurred, the cumulative rate function **R**, and the real time that has elapsed in seconds during a network simulation will be displayed on the screen throughout the simulation's run. This information is updated at a rate determined by the *summary_output_rate* discussed below.

#### Summary Output Rate

```python
summary_output_rate:
  100
```

This value corresponds to how often the *stdout_summary* information that is printed to the screen during a simulation is updated based on steps in the [KMC](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo). In this case, we can see that this information will update every hundred steps in the [KMC](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo).

#### Visualize

```python
visualize:
  true
```

If *true* a network simulation will produce two additional files in the **output** directory, *network.dat* and *network.gexf*. *network.dat* shows every single follower that each agent has. *network.gexf* is a file which can be used to visualize simulated networks on programs like [Gephi](http://gephi.github.io/).

#### Agent Stats

```python
agent_stats:
  true
```

If *true*, a simulation will produce additional files in the **output** directory named after the particular agent types within the simulation as *agenttype_info.dat* (e.g. the presence of a *Standard* agent type will produce a file labelled *Standard_info.dat*).

#### Degree Distributions

```python
degree_distributions:
  true
```

If *true*, the in-degree, out-degree, and cumulative-degree distributions will be produced in the **output** directory for each simulated month, as, in the case of the zeroth month, *in-degree_distribution_month_000.dat*, *out-degree_distribution_month_000.dat*, and *cumulative-degree_distribution_month_000.dat* respectively.

#### Tweet Analysis

```python
tweet_analysis:
  true
```

Setting to *true* creates the *tweets_distro.dat* and *retweets_distro.dat* output files, which contain the respective distributions for every possible number of tweets and retweets that agents have made in the network simulation.

#### Retweet Visualization

```python
retweet_visualization:
  true
```

If *true*, a network simulation will produce an additional output file titled *retweet_viz.gexf*, which can be used to visualize the most popular tweet on programs such as [Gephi](http://gephi.github.io/).

#### Main Statistics

```python
main_statistics:
  true
```

Setting to *true* creates the *main_stats.dat* output file, which contains all principal information from a given network simulation.

#### Degree Distribution by Follow Model

```python
degree_distribution_by_follow_model:
  true
```

If set to *true*, creates the *dd_by_follow_model.dat* output file, which shows the normalized probability and the normalized probability logarithm that an agent of a particular cumulative-degree made its connections via each particular follow method.

#### Region Connection Matrix

```python
region_connection_matrix:
  true
```

If set to *true*, creates the 'region_connection_matrix.dat' output file, which contains a chart showing the connections made between regions.

#### Categories Distribution

```python
categories_distro:
  true
```

If *true*, creates the *Categories_Distro.dat* output file, which shows how many agents have made a certain number of tweets and retweets and how many agents have a particular number of followers.

#### Most Popular Tweet Content

```python
most_popular_tweet_content:
  true
```

If *true*, creates the *most_popular_tweet_content.dat* output file, which contains details on the most retweeted tweet and its author.

### **ranks**

This section details how agents can be ranked according to the number of tweets and retweets they make, and the number of followers they have.

#### Tweet Ranks

```python
tweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}
```

Serves as a way to organize and categorize agents by the number of tweets they make in bins. The *bin_spacing* can be *linear*, *quadratic*, or *cubic*, which means that bins can be spaced by the *increment* raised to the power of 1, 2, or 3 respectively. The *tweet_ranks* can also be set to have a minimum, maximum, and increment value. In the above case, with tweeting enabled, agents with 10 or less tweets will be stored on a list in the intitial bin. If some of these agents continue to tweet, and end up having a total number of tweets greater than 10 but less than 20, they will move to the next bin. If their total number of tweets increases to a value greater than 20 but less than 30, they will then move on to the next bin. Agents can keep moving bins until they have made (in this case) 291 tweets, since they will be placed in the final bin where agents with 300 or less tweets are placed and remain there. If tweeting is disabled, the number of agents in each tweet rank wll be listed as zero.

#### Retweet Ranks

```python
retweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}
```

Serves the exact same purpose as *tweet_ranks*, but in this case categorizes agents based on the number of retweets they've made.

#### Follow Ranks

```python
follow_ranks:
  thresholds: {bin_spacing: linear, min: 0, max: 100001, increment: 1}
  weights:    {bin_spacing: linear, min: 1, max: 100000, increment: 1}
```

Similar to *tweet_ranks* and *retweet_ranks*, *follow_ranks* serve to categorize and organize agents based on the number of followers that they have. However, an additional factor is taken into consideration, the bin *weights*. The bin *weights* give each of the bin *thresholds* a weight equal to its corresponding value increased by 1 (assuming the *bin_spacing* is *linear* and the *increment* is 1). These weights are essential in the *twitter_suggest* and *preferential_agent* follow models, since they are what cause agents with a higher in-degree to have a better chance of being followed than agents with fewer followers. This is due to highly connected agents being placed in bins that are more heavily weighted, while agents with fewer followers are placed in bins with a lower weight.

### **tweet observations**

Through a network simulation, tweets lose relevance over time, to the point where they will no longer be retweeted. This section deals with the decay function for *retweet* rates.

#### Density Function

```python
density_function:
       2.45 / (x)**1.1
```
The *density_function* in which *retweet* rates decay over time, this function will be integrated and each discrete element of this function will be divided by the total integral. This is done to normalize the function and ensure that it truly is a [probability density function](https://en.wikipedia.org/wiki/Probability_density_function). Agents that can be retweeted are binned according to the values determined by this function. As time progresses, they are moved to bins of smaller values or *retweet* rates. The discrete elements of this function can be adjusted below.

#### x_start

```python
   x_start:
       5
```

This is the initial value of the density function. Make sure that the value you choose for this does not cause a discontinuity in the function (e.g. using 0 for this value would cause the above density function to be divided by 0, causing a discontinuity).

#### x_end

```python
   x_end:
       600
```

This is the final value of the density function. Just like for *x_start*, make sure that this value does not cause a discontinuity in the function.

#### Initial Resolution

```python
   initial_resolution:
       1.0
```

Determines the initial boundary spacing for integration of the density function. For example, here we have an *initial_resolution* of 1.0 and *x_start* equal to 5, so we could expect the first integral to be bounded from 5 to 6, since they have a difference of 1.

#### Resolution Growth Factor

```python
   resolution_growth_factor:
       1.05
```

This value determines the density function integral boundary spacing subsequently after the initial resolution boundary spacing. This has been implemented due to a function resolution needing to be much more accurate during the time immediately after a tweet is tweeted as opposed to some time after this has occurred. If you had an *initial_resolution* of 1.0 and a *resolution_growth_factor* of 1.05 with *x_start* set to 5, the integrals would be evaluated at [5,6], [6, 7.05], [7.05, 8.10], etc. 

#### Time Span

```python
   time_span:
       8*hour
```

Determines for how long after a tweet has been made that it can be retweeted. Once this time has elapsed, the density function will disappear, allowing no more retweets or following via retweets to occur for this tweet.

### **ideologies**

```python
  - name: Red
  - name: Blue
```

Here lists all the different dogmas that agents in the network simulation can possibly have. They can be named whatever you like.

**Note**: if you'd like to change the number of *ideologies* in a network simulation, make sure this number is less than or equal to the value set for *N_BIN_IDEOLOGIES* in the *config_static.h* file in **src**. You can change the value set for *N_BIN_IDEOLOGIES*, but make sure to then rebuild ***#k@*** by running *build.sh -O* in order for these changes to take effect.

### **regions**

This sections outlines the *regions* in which you would like your agents to be located.

```python
- name: Ontario
    add_weight: 1

    preference_class_weights: {StandardPref: 100, NoRetweetPref: 100}
    ideology_weights: {Red: 100, Blue: 100}
    language_weights: {English: 50, French: 25, French+English: 25}
```

A region's *add_weight* corresponds to the probability that an agent added to the network is from that region, and is weighted with respect to the *add_weights* of all the other regions. The *preference_class_weights* are the possible *preference classes* agents from that region can have, and are weighted with respect to one another. The *ideology_weights* are the possible *ideologies* agents can have, and are weighted with respect to one another. The *language_weights* are the possible *languages* that an agent from that region can speak, and are also weighted with respect to each other.

**Note**: if you'd like to change the number of *regions* in the simulation, make sure this number is EXACTLY equal to the value set for *N_BIN_REGIONS* in the *config_static.h* file in **src**. You can change this value set for *N_BIN_REGIONS*, but make sure to then rebuild ***#k@*** by running *build.sh -O* in order for these changes to take effect.

### **preference classes**

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
   follow_reaction_prob:
      0.5
```

The *preference classes* detail how agents react to tweets in a network simulation. The four possible types of tweets are *plain* tweets, *musical* tweets, *ideological* tweets, and *humorous* tweets. Under *tweet_transmission* one can dictate the probability that tweets made by a particular agent type will be retweeted based on the content of that tweet, whether it be plain or musical, have a different ideology, have the same ideology, or be humorous. Instead of inserting a particular agent type, one can also input *all* or *else* for any of the *tweet_transmision* rates to dictate the *tweet_transmission* for all the agent types or the rest of the agent types not specified respectively. The *follow_reaction_prob* determines the probability that an agent will follow another agent based on seeing one of their tweets being retweeted as opposed to just retweeting that retweet. With the *follow_reaction_prob* set to 0.5 in the above example, there is a 50% chance that an agent will follow the speaker of an original tweet being retweeted and a 50% chance that they will just retweet the retweet.   

**Note**: if you'd like to change the number of *preference classes* in the simulation, make sure this number is less than or equal to the value set for *N_BIN_PREFERENCE_CLASS* in the *config_static.h* file in **src**. You can change the value set for *N_BIN_PREFERENCE_CLASS*, but make sure to then rebuild ***#k@*** by running *build.sh -O* to have these changes take effect.

### **agents**

In this section, one can create the types of agents that will be included in their network simulation.

#### Agent Type

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

Under *weights* we can see *add*, which correlates to the probability, weighted against all the other agent types, that, when an agent is added into the network, this is its agent type. The value inputted into *follow* will be the probability, weighted against the other agent types, that an agent of this agent type will be the one followed in the *agent* follow model when a follow occurs. *tweet_type* summarizes the weighted probability that the content of an agent of this type's tweets will be ideological, plain, musical, or humorous. The *followback_probability* is the probability that following an agent of this type will result in that agent following you back, though *use_followback* has to be enabled for this to have any effect. The *hashtag_follow_options* are implemented in the *hashtag* follow model, where agents follow other agents based on the hashtags present in their tweets. In this follow model, if *care_about_region* is set to *true*, agents of this agent type will only follow other agents that live in the same region as them, and if *care_about_ideology* is set to *true*, agents of this agent type will only follow other agents that share the same ideology as them. It is important to note that *use_hashtag_probability* in the **analysis** section must be set to a value greater than zero for this to work, or else no one will be implementing hashtags into their tweets, and no one will therefore be able to follow via this method. The *follow* and *tweet* rates are the rates at which agents of this particular agent type will *follow* and *tweet* per simulated minute. These work in the exact same manner as the *add* rate outlined in the **rates** section.

**Note**: if you'd like to change the number of agent types in the simulation, make sure this value is less than or equal to the value set for *N_BIN_AGENT_TYPES* in the *config_static.h* file in **src**. You can change the value set for *N_BIN_AGENT_TYPES*, but make sure to then rebuild ***#k@*** by running *build.sh -O*, in order for these changes to take effect.