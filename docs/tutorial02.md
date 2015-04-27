[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

# Input

In this tutorial we will cover the input through which a user can manipulate for their own ends in #k@. This tutorial should take approximately 90 minutes to complete

The main file through which users will use for input is INFILE.yaml. A default version which users can use for reference is DEFAULT.yaml. We will go through each line of input in DEFAULT.yaml to ensure that the user knows all the options at their disposal.

##analysis

The first section of this program is titled analysis, and it outlines what is the general focus of this simulation. Let's go over what each variable means.

```python
initial_entities:
  20
```

Entities correspond to Twitter users on the network. The *initial_entities* are therefore the initial Twitter users you will have at the start of the simulation. As we can see, in DEFAULT.yaml, this value is 20.

```python
max_entities:
  100001
```

The *max_entities* are the maximum number of Twitter users that will be in the same simulation. Once this number of entities is reached, its magnitude will not increase. In this case we can see its number is 100001.

```python
max_time:
  hour
```

The *max_time* variable coincides with the maximum simulation time in which you want the simulation to run. Once this simulated time has been reached, the simulation ends. This time is in minutes, and as we can see for DEFAULT.yaml, has a value of 1 hour or 60 minutes.
Note that interacting with the simulation has no impact on the simulation time.

```python
max_analysis_steps:
  unlimited
```

The *max_analysis_steps* are the maximum  number of steps undertaken in the KMC loop throughout the simulation. To achieve the most accurate results possible, it is recommended to keep this value at unlimited.

```python
max_real_time:
  hour
```

*max_real_time* is the maximumreal itme in which you want the smulaiton to run. Once this actual time has elapsed, the simulation ends. This time is also in minutes, and in the DEFAULT.yaml file has a value of 1 hour or 60 minutes. It is important to note that as
opposed to the simulated time, interacting with the simulation does affect the real time. 

```python
enable_interactive_mode:
  true
```

Inputting *true* into *enable_interactive_mode* allows the user to interact with the simulation while its running. Inputting *false* into this variable prevents this. We will discuss **Interactive Mode** in much further detail later.

```python
enable_lua_hooks:
  false
```

Inputting *true* into *enable_lua_hooks* implements runtime Lua functions that can react to events such as a tweet, a retweet, a follow, etc, while inputting *false* does not. *enable_lua_hooks* is defined in the same file as interactive mode,
though it should be noted that lua hooks will slow down the simulation immensely.

```python
lua_script: # Defines behaviour of interactive mode & lua hooks
  INTERACT.lua
```

*lua_script* contains the script in which all the functions able for use interactive mode can be found. It is highly recommended to keep this variable as INTERACT.lua.

```python
use_barabasi:
  false
```

Input *true* into *use_barabsi* for a preferential model simulation, input *false* otherwise. Will be discussed in greater detail later.

```python
use_random_time_increment:
  true
```

Input *true* for *use_random_time_increment* to have time increase by random values modelled by the KMC loop, or input *false* to have the simulation move by constant increments of time. Choosing either option will not have a major impact on your results.

```python
use_followback:
  true
```

Inputting *true* into *use_followback* will allow entities to follow other entities that follow them, while inputting *false* will prevent this from happening.

```python
follow_model: # See notes above
  random
```

The *follow_model* determines the manner in which entities choose who to follow. This will definitely definitely be confered in much greater detail later. The input options for follow_model are: *random*, *preferential*, *entity*,
*preferential-entity*, *hashtag*, and *twitter*.

```python  
model_weights: {random: 0.20, preferential: 0.20, entity: 0.20, preferential_entity: 0.20, hashtag: 0.20}
```

*model_weights* are only relevant when using the *twitter* follow model. Will be elaborated on later.

```python
stage1_unfollow:
  false
```

Inputting *true* into *stage1_unfollow*  will enable the program to flag entities in which a twitter user is following that are tweeting much more than others. These flagged entities are at risk of being unfollowed by the entity following them later in the simulation.

```python
unfollow_tweet_rate:
  10
```

The *unfollow_tweet_rate* is an integer by which, if the number of tweets by an entity exceeds this value, will cause this entitity to randomly lose one of its followers.

```python
use_hashtag_probability:
  1
```

The probability of a tweet containing a # from 0 to 1. As we can see in DEFAULT.yaml, the probability of using a # is 1, so every tweet will contain a #.

##rates

```python
rates:
  add: {function: constant, value: 0.0}
```

The rate at which entities will be added into the network per minute throughout the course of the simulation. This function can either be a constant or linear add rate. For a constant add rate. the function must be identified as *constant* and the
number of entities you wish to add per minute must inputted into *value*. This number must be greater or equal to zero. For a linear add rate, the add rate must be changed to:

```python
rates:
  add: {function: constant, y_intercept: 1.0, slope: 0.1}
```

where the y-intercept is the initial arbitrary value of entities added to the network and the slope, multipled by the number of simulated months that have elapsed within the simulation, is the value by which the y-intercept is increased by. 

##output

```python
output:
  save_network_on_timeout:
    false
  load_network_on_startup:
    false
  ignore_load_config_check: # Whether to allow loading configuration with mismatching configuration (generally OK)
    false
  save_file: # File to save to, and load from
    network_state.sav
  stdout_basic:
    true
  stdout_summary:
    true
  summary_output_rate:
    1
  visualize:
    true
  verbose:
    true
  entity_stats:
    true
  degree_distributions:
    true
  tweet_analysis:
    true
  retweet_visualization:
    false
  main_statistics:
    true
```

This section of the program outlines what is present in the output directory once the simulation is concluded. It is recommended to leave these settings as they are.

##ranks

```python
tweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}
```



```python
retweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}
```



```python
follow_ranks:
  thresholds: {bin_spacing: linear, min: 0, max: 100001, increment: 1}
  weights:    {bin_spacing: linear, min: 1, max: 100000, increment: 1}
```



##tweet observations

```python
density_function:
       2.45 / (x)**1.1
```



```python
   x_start:
       5
```



```python
   x_end:
       600
```



```python
   initial_resolution:
       1.0
```



```python
   resolution_growth_factor:
       1.05
```




```python
   time_span:
       8*hour
```



##ideologies

```python
  - name: Red
  - name: Blue
```



##regions

```python
- name: Ontario
    add_weight: 1

    preference_class_weights: {StandardPref: 100}
    entity_class_weights: {Standard: 100}
    ideology_weights: {Red: 100, Blue: 100}
    language_weights: {English: 50, French: 25, French+English: 25}
```



##config_static

```python
humour_bins: 2
```



##preference classes

```python
- name: StandardPref

  # Determines the probability that a tweet is reacted to by this
  # preference class:
   tweet_transmission:
      plain: # Also applies to musical tweets
        Standard: 0
        Celebrity: 0
        else: 0
      different_ideology:
        Standard: 0.0
        Celebrity: 0.0
        else: 0.0
      same_ideology:
        Standard: 0.0
        Celebrity: 0.0
        else: 0.0
      humourous:
        Standard: 0.0
        Celebrity: 0.0
        else: 0.0
   # Probability that we will follow as a reaction.
   # Only applies to 2nd generation retweets, otherwise
   # the entity would already be a follower.
   follow_reaction_prob:
      0.5
```



##entities

```python
- name: Standard
    weights:
      # Weight with which this entity is created
      add: 80
      # Weight with which this entity is followed in entity follow
      follow: 5
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humourous: 1.0 # Can be considered the humourousness of the entity type
    # Probability that following this entity results in a follow-back
    followback_probability: 44
    hashtag_follow_options:
      care_about_region: true # does the entity care about where the entity they will follow is from?
      care_about_ideology: false # does the entiy care about which ideology the entity has?
    rates:
        # Rate for follows from this entity:
        follow: {function: constant, value: 0.1}
        # Rate for tweets from this entity:
        tweet: {function: constant, value: 0.01}
```



<iframe width="420" height="315" src="https://www.youtube.com/embed/g2QeKQ9yXy0" frameborder="0" allowfullscreen></iframe>

## Commands

* `~[2] mkdocs new [dir-name]` - Create a new project.
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs help` - Print this help message.

To launch #k@, we use the run command

`./run.sh`

The run command can take several command line arguements 

`./run.sh --build`

