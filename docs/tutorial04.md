[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Different Follow Models

This tutorial should take approximately 90 minutes to complete

## random

As the name implies, a random follow model is one in which the entities in which a twitter user follows is totally arbitrary.
Let's work through two simulations of a random follow model, one with no retweets, and the other allowing retweets.

### No Retweets

The link for this example is shown below.

For simplicity both tweeting an retweeting have been neglected in this
simulation. It is important to note that the maximum simulation time of this
example is 100,000 simulated minutes and that the entity follow rate is 0.001.
Therefore, we can expect most entities to have an in-degree and out-degree
distribution of 100 and a cumulative degree distribution of 200. Running this
simulation, we have produced the following data for the second month of
simulation time using gnuplot:

#### In Degree Distribution

![In Degree Distribution](/img/random_example/no_retweet_in.svg =1x  "In Degree Distribution")

#### Out Degree Distribution

![Out Degree Distribution](/img/random_example/no_retweet_out.svg =1x  "Out Degree Distribution")

#### Cumulative Degree Distribution 

![Cumulative Degree Distribution](/img/random_example/no_retweet_cumulative.svg =1x  "Cumulative Degree Distribution")

## twitter-preferential

Based on Albert-Laszlo Barabasi's research, a twitter-preferential follow model is one in which entities tend to follow users with the highest degree, aka highest number of followers. 

## entity

An entity-based follow method is one in which entities follow a twitter user based on their entity class, though who they follow within that entity class is totally random.

## preferential-entity

A preferential-entity follow method is exactly like the entity-based follow method, except that instead of following a random twitter user within that entity class,
entities tend to follow twitter users within that entity class with the highest number of followers (degrees).

## hashtag

A hashtag follow method is one in which a twitter user follows another entity through a hashtag that matches their hatchtag preferences. In order for this follow method to be possible, *use_hashtag_probabilty* must be greater than zero.

## twitter

The twitter follow model is a combination of all the above models, with each model respectively weighted.

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
