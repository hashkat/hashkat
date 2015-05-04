[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Twitter-Preferential Follow Model

This tutorial should take approximately 90 minutes to complete

Based on Albert-Laszlo Barabasi's research, a twitter-preferential follow model is one in which entities tend to follow users with the highest degree, aka highest number of followers.

## Example - Classic Barabasi



#### In Degree Distribution



#### Out Degree Distribution



#### Cumulative Degree Distribution



#### Visualization



## Example - Non-Classic Barabasi with No Retweets



#### In Degree Distribution



#### Out Degree Distribution



#### Cumulative Degree Distribution 



#### Visualization



## Example - Non-Classic Barabasi with Retweets



#### In Degree Distribution



#### Out Degree Distribution



#### Cumulative Degree Distribution



#### Visualization



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

## INFILE.yaml
```lang-none
analysis:
  initial_entities:
    10000  # start out from something small, could be when only the developers were online.
  max_entities: 
    10000     # 1 million max users
  max_time: 
    100000
  max_analysis_steps: 
    unlimited
  max_real_time: 
    240*hour*5        # 10 days         
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
    false        # followback turned on, from literature it makes sense for a realistic system
  follow_model: # See notes above
    random
  # model weights ONLY necessary for follow method 'twitter'  
  # educated guesses for the follow models  
  model_weights: {random: 0.0, preferential: 1.0, entity: 0.0, preferential_entity: 0.0, hashtag: 0.0, referral: 0.0}
  
  stage1_unfollow: 
    false
  unfollow_tweet_rate: 
    100
  use_hashtag_probability:
    0.5    # 50 % chance of using a hashtag
```
