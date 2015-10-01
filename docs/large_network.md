[hashkat.org](http://hashkat.org)

# Large Network

The networks used and created in these tutorials are all relatively small networks. 

If you would like to run simulations with much larger networks, we've included a template of one such network below.

This sample large network uses over 128 GB of RAM.  

This input file can be found in the **~/hashkat/docs/tutorial_input_files** directory in file 'large_network'.

```python
#################################################################
# >> analysis:
#
# Attributes for controlling the duration and type of analysis.
#  initial_agents:
#    The initial agent amount to create.
#  max_agents: 
#    The maximum amount of agents for which to allocate. Once the network
#    has grown to this amount, the agent add rate will artifically drop to 0.
#  max_time:
#    The maximum simulation-time for the simulation. Once it has elapsed, the simulation halts.
#    Interacting with the simulation does not alter the simulation-time.
#    In simulated minutes.
#  max_analysis_steps:
#    The maximum number of steps to be made in the KMC loop. For superior results, set this value to 'unlimited'.
#  max_real_time:
#    The maximum real-time for the simulation. Once it has elapsed, the simulation halts.
#    Note that interacting with the simulation DOES detract from this time.
#    In minutes.
#  enable_interactive_mode:
#    Whether interactive mode should be triggered by Ctrl+c or ./scripts/stop.sh (triggers SIGUSR1).
#  enable_lua_hooks:
#    Whether to use runtime Lua functions that can react to events. **Slow!**
#    Hooks are availble for running on every tweet, retweet, follow, etc.
#  lua_script:
#    Script to use to define the behaviour of interactive mode as well as lua hooks.
#  use_barabasi: 
#    If true, agents will make a certain number of connections set by barabasi_connections.
#  barabasi_exponent:
#    The value of the exponent assigned to each agent's cumulative-degree.
#  use_random_time_increment: 
#    If true, simulation time will be incremented at a non-constant rate. Increments by 1/sum(rates) on average
#  use_followback: 
#    Whether to enable follow-back in the simulation.
#  use_follow_via_retweets:
#    Whether to enable following via retweets in the simulation.
#  follow_model: 
#    Accepted models: 'random', 'twitter_suggest', 'agent', 'preferential_agent', 'hashtag', 'twitter'
#  model_weights:
#    The normalized probability of calling each follow model for the 'twitter' follow model.
#  stage1_unfollow: 
#    Whether to have an unfollow model assuming constant 'chattiness', compares the chattiness of an agent wrt to the following set.
#  unfollow_tweet_rate: 
#    Tweets per simulated minute. Chattiness for which to be considered for unfollow.
#  use_hashtag_probability:
#    The probability that tweets will contain a hashtag.
#################################################################

analysis:
  initial_agents:
    100000
  max_agents: 
    1000000
  max_time: 
    2*year
  max_analysis_steps: 
    unlimited
  max_real_time: 
    7*day
  enable_interactive_mode:
    false
  enable_lua_hooks: # Defined in same file as interactive_mode. Can slow down simulation considerably.
    false
  lua_script: # Defines behaviour of interactive mode & lua hooks
    INTERACT.lua
  use_barabasi: 
    false
  barabasi_connections: # number of connections we want to make when use_barabasi == true
    2
  barabasi_exponent:
    1
  use_random_time_increment: 
    true
  use_followback: 
    true        # from literature, it makes sense for a realistic system to have followback enabled
  use_follow_via_retweets:
    true
  follow_model: # See notes above
    twitter
  # model weights ONLY necessary for follow method 'twitter'  
  model_weights: {random: 1.0, twitter_suggest: 1.0, agent: 1.0, preferential_agent: 1.0, hashtag: 1.0}

  stage1_unfollow: 
    true
  unfollow_tweet_rate: 
    0.2
  use_hashtag_probability:
    0.5

#################################################################
# >> rates:
#
# The rate function for adding agents to the network.
#################################################################

rates:
  add: {function: linear, y_intercept: 10.0, slope: 10.0}

#################################################################
# >> output:
#
# Various options for the output of the simulation, both while it
# runs and for post-analysis.
#################################################################

output:
  save_network_on_timeout: 
    true
  load_network_on_startup:
    true
  ignore_load_config_check: # Whether to allow loading configuration with mismatching configuration (generally OK)    
    true
  save_file: # File to save to, and load from
    network_state.dat
  stdout_basic: 
    true
  stdout_summary: 
    true
  summary_output_rate: 
    1
  visualize: 
    true
  agent_stats: 
    true
  degree_distributions: 
    true
  tweet_analysis: 
    true
  retweet_visualization:
    true
  main_statistics:
    true
  degree_distribution_by_follow_model:
    true
  region_connection_matrix:
    true
  categories_distro:
    true
  most_popular_tweet_content:
    true

#################################################################
# >> *_ranks:
# Options for the categorization based on various attributes.
#################################################################

tweet_ranks: 
  thresholds: {bin_spacing: linear, min: 10, max: 10000000, increment: 10}
retweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 10000000, increment: 10}
follow_ranks:
# MUST be adjusted for max_agents for simulations which implement the twitter_suggest and/or preferential_agent follow models
  thresholds: {bin_spacing: linear, min: 0, max: 1000000, increment: 1}
  weights:    {bin_spacing: linear, min: 1, max: 1000001, increment: 1}

#################################################################
# >> tweet_observation: 
#
# An observation probability density function that gives 
# the probability that a tweet is observed at a certain time by an 'ideal observer'. 
# An 'ideal observer' is one which always sees a tweet, eventually.'
# The observation PDF is used for both retweeting and follow-from-tweet.
# We combine this with a relevance factor, r, where 0.0 <= r <= 1.0. We in turn
# determine the probability that a given agent will act on a given tweet, with enough time.
#
#  density_function:
#    Probability 'density' function to sample logarithmatically.
#    Provided as if a Python function of 'x'. It is integrated, and after integration,
#    the function will be normalized for you. 
#    Note: technically not a true PDF because one does NOT need to have an integral range that sums to 1.0.
#
#  x_start:
#    In arbitrary units. The beginning x-value to integrate from.
#  x_end:
#    In arbitrary units. The end x-value to integrate to.
#
#  initial_resolution:
#    In arbitrary units. The initial binning resolution. 
#    That is, the x-step with which to begin binning. The binning resolution is decreased from there on.
#  resolution_growth_factor:
#    How quickly the resolution grows from one bin to the next. 
#    Quicker is more efficient, but with less precise rates in later bins.
#  time_span:
#    In simulated minutes. The time over which the function is defined.
#    After this, tweets will 'disappear'.
#################################################################

tweet_observation:
   density_function: 
       2.45 / (x)**1.1 
   x_start: 
       1
   x_end: 
       600
   initial_resolution: 
       1.0
   resolution_growth_factor: 
       1.05
   time_span: 
       8*hour

#################################################################
# >> ideologies: 
# Abstract categorizations of similar beliefs.
# The amount of ideologies MUST be less than or equal to the value
# set for N_BIN_IDEOLOGIES in config_static.h!
#################################################################

ideologies:
  - name: Red
  - name: Blue
  - name: Green
  - name: Orange
  - name: Yellow
  - name: Purple

#################################################################
# >> regions: 
#
# Locations, such as countries, can be represented abstractly. 
# Note that the number of regions must match the value set for N_BIN_REGIONS
# in config_static.h EXACTLY!
#
# add_weight:
#   Required for each region. The weight with which 
#   this region is chosen.
#
#  preference_class_weights:
#    Normalized probability that an agent has a particular 'preference_class'
#  idealogy_weights:
#    Normalized probability that an agent has a particular 'ideology' 
#  languages_weights:
#    Normalized probability that an agent speaks a particular 'language'
#################################################################

regions:

  - name: USA
    add_weight: 10

    preference_class_weights: {USAPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 85, French: 0, Spanish: 15, French+English: 0}

  - name: UK
    add_weight: 9

    preference_class_weights: {UKPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 95, French: 5, Spanish: 0, French+English: 0}

  - name: Mexico
    add_weight: 8

    preference_class_weights: {MexicoPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 10, French: 0, Spanish: 90, French+English: 0}

  - name: Spain
    add_weight: 7

    preference_class_weights: {SpainPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 30, French: 0, Spanish: 70, French+English: 0}

  - name: Canada
    add_weight: 6

    preference_class_weights: {CanadaPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 80, French: 20, Spanish: 0, French+English: 0}

  - name: Venezuela
    add_weight: 5

    preference_class_weights: {VenezuelaPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 25, French: 0, Spanish: 75, French+English: 0}

  - name: France
    add_weight: 4

    preference_class_weights: {FrancePref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100, Yellow: 100, Purple: 100}
    language_weights: {English: 10, French: 90, Spanish: 0, French+English: 0}

#################################################################
# >> preference_classes: 
# The different preference stereotypes that users in the network can have.
#
# tweet_transmission:
#   The transmission probability for tweets made by agents of a particular agent type
#   given tweet content and the tweet observation PDF. Effectively, this determines
#   the proportion (on average) of tweets made by this agent type that will be retweeted.
#
#   Can be provided for an agent type, or the keys 'else' or 'all'. 
#   Both 'else' and 'all' have the functionality of defining the transmission probability
#   function for all otherwise unspecified agent types.
#
#   Transmission probability functions are automatically converted into the necessary tables
#   using Python. The strings provided can be any valid Python.
#################################################################

preference_classes:
 - name: USAPref

   tweet_transmission: 
      plain: # Also applies to musical tweets
        all: 0.1
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.2
      humorous:
        all: 0.4

 - name: UKPref

   tweet_transmission:
      plain: # Also applies to musical tweets
        all: 0.1
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.5
      humorous:
        all: 0.2

 - name: MexicoPref

   tweet_transmission:
      plain: # Also applies to musical tweets
        all: 0.6
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.3
      humorous:
        all: 0.5

 - name: SpainPref

   tweet_transmission:
      plain: # Also applies to musical tweets
        all: 0.4
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.6
      humorous:
        all: 0.3

 - name: CanadaPref

   tweet_transmission:
      plain: # Also applies to musical tweets
        all: 0.2
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.3
      humorous:
        all: 0.6

 - name: VenezuelaPref

   tweet_transmission:
      plain: # Also applies to musical tweets
        all: 0.4
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.8
      humorous:
        all: 0.4

 - name: FrancePref

   tweet_transmission:
      plain: # Also applies to musical tweets
        all: 0.6
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        all: 0.6
      humorous:
        all: 0.2

#################################################################
# >> agents: 
#
# The different types of agents in the network, and their 
# associated rates.
#################################################################

agents:
  - name: Standard-Chatty
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 1.0
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0.2
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
    rates: 
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.05}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.5}

  - name: Standard-Friendly
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 1.0
      tweet_type:
        ideological: 1.0
        plain: 7.0
        musical: 1.0
        humorous: 3.0
    # Probability that following this agent results in a follow-back
    followback_probability: 1.0
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.3}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.04}

  - name: Standard-Funny
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 1.0
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 0.0
        humorous: 6.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0.44
    hashtag_follow_options:
      care_about_region: true # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.02}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.1}

  - name: Standard-Musical
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 1.0
      tweet_type:
        ideological: 0.0
        plain: 1.0
        musical: 5.0
        humorous: 0.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0.1
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.01}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.02}

  - name: Standard-Political
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 1.0
      tweet_type:
        ideological: 8.0
        plain: 1.0
        musical: 0.0
        humorous: 3.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0.44
    hashtag_follow_options:
      care_about_region: true # does the agent care about where the agent they will follow is from?
      care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.01}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.12}

  - name: Celebrity-Musician
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 10.0
      tweet_type:
        ideological: 0.0
        plain: 1.0
        musical: 7.0
        humorous: 2.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0.44
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0001}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.1}

  - name: Celebrity-Athelete
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 10.0
      tweet_type:
        ideological: 0.0
        plain: 3.0
        musical: 1.0
        humorous: 1.0
    # Probability that following this agent results in a follow-back
    followback_probability: 0.44
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0001}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.1}

  - name: Celebrity-Politician
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 10.0
      tweet_type:
        ideological: 8.0
        plain: 1.0
        musical: 0.0
        humorous: 0.0
    # Probability that following this agent results in a follow-back
    followback_probability: 1.00
    hashtag_follow_options:
      care_about_region: true # does the agent care about where the agent they will follow is from?
      care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0001}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.15}

  - name: Bot
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 1.0
      tweet_type:
        ideological: 0.0
        plain: 1.0
        musical: 0.0
        humorous: 0.0
    # Probability that following this agent results in a follow-back
    followback_probability: 1.00
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.01}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.01}

  - name: Organization-Humorous_Marketing_Campaign
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 5.0
      tweet_type:
        ideological: 0.0
        plain: 1.0
        musical: 2.0
        humorous: 5.0
    # Probability that following this agent results in a follow-back
    followback_probability: 1.00
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.2}

  - name: Organization-Musical_Marketing_Campaign
    weights:
      # Weight with which this agent is created
      add: 100.0
      # Weight with which this agent is followed in agent follow
      follow: 5.0
      tweet_type:
        ideological: 0.0
        plain: 1.0
        musical: 5.0
        humorous: 2.0
    # Probability that following this agent results in a follow-back
    followback_probability: 1.00
    hashtag_follow_options:
      care_about_region: false # does the agent care about where the agent they will follow is from?
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
    rates:
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0}
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.2}
```

It is important to note that in order to run a simulation using the input file above, the following changes must be made to the *config_static.h* file (make sure to rebuild ***#k@*** after modifying the *config_static.h* file in order for the changes to take effect):

* *N_BIN_PREFERENCE_CLASS* must be set to 7:

```python
const int N_BIN_PREFERENCE_CLASS = 7;
```

* *N_BIN_REGIONS* must be set to 7:

```python
const int N_BIN_REGIONS = 7;
```

* *N_BIN_IDEOLOGIES* must be set to 6

```python
const int N_BIN_IDEOLOGIES = 6;
```

Once you have copied the above input file into your *INFILE.yaml* file, you are free to make any changes to it that you'd like.