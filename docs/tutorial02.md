[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

# Input

In this tutorial we will cover the input through which a user can manipulate for their own ends in #k@. This tutorial should take approximately 90 minutes to complete

The main file through which users will use for input is INFILE.yaml. A default version which users can use for reference is DEFAULT.yaml. We will go through each line of input in DEFAULT.yaml to ensure that the user knows all the options at their disposal.

##analysis

The first section of this program is titled analysis, and it outlines what is the general focus of this simulation. Let's go over what each variable means.

The entities present throughout this simulation correspond to Twitter users on the network.
The “initial_entities” variable used in this analysis corresponds to the inaugural Twitter users at the start of this operation. A small value for this would be ideal, perhaps when it was just in use by the developers.
The “max_entities” variable will of course correspond to the maximum number of Twitter users you would expect to have throughout the course of the simulation and for which to allocate. Once the number of entities within the network has grown to this magnitude, the addin$
The “max_time” will be the maximum length of simulation time you would like this simulation to run for. The simulation will halt once this simulated time has elapsed, though it is important to note that interacting with the simulation will have no impact on the simulati$
The “max_analysis_steps” !!!!
The “max_real_time” corresponds to the maximum length of actual clock time you would like this simulation to run for. The simulation will end once this time has elapsed in real time, though it should be noted that, as opposed to the “max_time”, interacting with the simu$
While the simulation is running, no input from the user is required. However, you do have the choice to implement interactive mode, which will allow user to !!!!!!
Implementing the variable “enable_lua_hooks” will determine whether or not !!!!
“lua_Script”!!!!!

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

```python
  enable_lua_hooks: # Defined in same file as interactive_mode. Can slow down simulation considerably.
    false
```

```python
  lua_script: # Defines behaviour of interactive mode & lua hooks
    INTERACT.lua
```

```python
  use_barabasi:
    false
```

```python
  use_random_time_increment:
    true
```

```python
  use_followback:
    true
```

```python
  follow_model: # See notes above
    random
```

```python  
  model_weights: {random: 0.20, preferential: 0.20, entity: 0.20, preferential_entity: 0.20, hashtag: 0.20}
```

```python
  stage1_unfollow:
    false
```

```python
  unfollow_tweet_rate:
    10
```

```python
  use_hashtag_probability:
    1
```

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

