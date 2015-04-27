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

Input *true* for a preferential model simulation, input *false* otherwise. Will be discussed in greater detail later.

```python
  use_random_time_increment:
    true
```

Input *true* to have time increase by random values modelled by the KMC loop, or input *false* to have the simulation move by constant increments of time. CHoosing either option will not have a major impact on your results.

```python
  use_followback:
    true
```

Inputting *true* will allow entities to follow other entities that follow them, while inputting *false* will prevent this from happening.

```python
  follow_model: # See notes above
    random
```

The *follow_model* determines the manner in which entities choose who to follow. This will definitely definitely be confered in much greater detail later. The input options for follow_model are: *random*, *preferential*, *entity*,
*preferential-entity*, *hashtag*, and *twitter*.

```python  
  model_weights: {random: 0.20, preferential: 0.20, entity: 0.20, preferential_entity: 0.20, hashtag: 0.20}
```

Only relevant when using the *twitter* follow model. Will be described later.

```python
  stage1_unfollow:
    false
```

Inputting *true* will enable the program to flag entities in which a twitter user is following that are tweeting much more than others. These flagged entities are at risk of being unfollowed by the entity following them later in the simulation.

```python
  unfollow_tweet_rate:
    10
```



```python
  use_hashtag_probability:
    1
```

The probability of a tweet containing a # from 0 to 1. As we can see in DEFAULT.yaml, the probability of using a # is 1, so every tweet will contain a #.

##rates



##output



##ranks



##tweet observations



##ideologies



##regions



##config_static



##preference classes



##entities



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

