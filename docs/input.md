[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Input

This page serves as a reference for users working with the input file INFILE.yaml. This page goes through every aspect of the file in which users can manipulate.

<iframe width="560" height="315" src="https://www.youtube.com/embed/4X063IPFzM4" frameborder="0" allowfullscreen></iframe>

## analysis

The first section of this program is titled analysis, and it outlines what is the general focus of this simulation. Let's go over what each variable means.

```python
initial_entities:
  20
```

Entities correspond to Twitter users on the network. The *initial_entities* are therefore the initial Twitter users you will have at the start of the simulation. As we can see, in the above case, this value is 20.

```python
max_entities:
  100001
```

The *max_entities* are the maximum number of Twitter users that will be in the same simulation. Once this number of entities is reached, its magnitude will not increase. In this case we can see its number is 100001.

```python
max_time:
  hour
```

The *max_time* variable coincides with the maximum simulation time in which you want the simulation to run. Once this simulated time has been reached, the simulation ends. This time is in minutes, and as we can see for this case, has a value of 1 hour or 60 minutes.
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

*max_real_time* is the maximumreal itme in which you want the smulaiton to run. Once this actual time has elapsed, the simulation ends. This time is also in minutes, and in the above example has a value of 1 hour or 60 minutes. It is important to note that as
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
lua_script:
  INTERACT.lua
```

*lua_script* contains the script in which all the functions able for use interactive mode can be found. It is highly recommended to keep this variable as INTERACT.lua.

```python
use_barabasi:
  false
```

Input *true* into *use_barabasi* for a simulation of the [Barabasi-Albert model](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model) when using a twitter-preferential, preferential-entity, or twitter (that implements either of the two previous) follow
models. Input *false* to prevent this.

```python
barabasi_connections: # number of connections we want to make when use_barabassi == true
  100
```

*barabasi_connections* outline the number of follows an entity will make when added to the network when *use_barabassi == true*. For a classic Barabasi-Albert model, set this value equal to 1. For a non-classic Barabasi-Albert model, set this value another value
greater than 1.

```python
barabasi_exponent:
  1
```

The order of the degrees within the network, the *barabasi_exponent* is normally kept to 1 for the most realistic simulated network.

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
follow_model:
  random
```

The *follow_model* determines the manner in which entities choose who to follow. This will definitely definitely be confered in much greater detail later. The input options for follow_model are: *random*, *twitter_preferential*, *entity*,
*preferential-entity*, *hashtag*, and *twitter*.

```python  
model_weights: {random: 0.20, twitter_preferential: 0.20, entity: 0.20, preferential_entity: 0.20, hashtag: 0.20}
```

*model_weights* are only relevant when using the *twitter* follow model. The *twitter* model is a combination of all the other follow models, so weigh each follow model to your choice proportion to produce a very complex system.

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

The probability of a tweet containing a # from 0 to 1. As we can see in this example, the probability of using a # is 1, so every tweet will contain a #.

##rates

<iframe width="560" height="315" src="https://www.youtube.com/embed/_NNOtwrPTg0" frameborder="0" allowfullscreen></iframe>

```python
rates:
  add: {function: constant, value: 0.0}
```

The rate at which entities will be added into the network per minute throughout the course of the simulation. This function can either be a constant or linear add rate. For a constant add rate. the function must be identified as *constant* and the
number of entities you wish to add per minute must inputted into *value*. This number must be greater or equal to zero. For a linear add rate, the add rate must be changed to:

```python
rates:
  add: {function: linear, y_intercept: 1.0, slope: 0.1}
```

where the y-intercept is the initial arbitrary value of entities added to the network and the slope, multipled by the number of simulated months that have elapsed within the simulation, is the value by which the y-intercept is increased by. 

## output

This section of the program outlines what is present in the output directory once the simulation is concluded. To acquire as much data as possible, it is recommended to initally set all these to *true*, and to input *false* to whatever you don't need after more 
experience running simulations.

```python
save_network_on_timeout:
  true
```

If *true* all of the simulated network data will be stored in the *save_file* outlined below.

```python
load_network_on_startup:
  true
```

If *true* all of the simulated network data within the save file will be loaded from when the simulated commenced

```python
ignore_load_config_check:
  true
```

If *true*, allows you to load a network from the save file and continue the simulation using an input file with differing parameters. Setting this to *false* will prevent you from doing this.

```python
save_file:
  network_state.sav
```

Determines the file where all the simulated data is saved to.

```python
stdout_basic:
  true
```

If *true* the number of simulated months through a simulation will be displayed on the screen and a message will be printed to the screen once the simulation has finished.

```python
stdout_summary:
  true
```

If *true*, the length of simulated time (in minutes), the number of entities, follows, tweets, retweets, cummulative rate function **R**, and the real time that has elapsed will be displayed on the screen throughout the simulation's run. The intervals at which
this information is changed and displayed is determined by the *summary_output_rate* discussed below.

```python
summary_output_rate:
  100
```

This value corresponds to the number of step intervals in the KMC loop where *stdout_summary* information is updated and displayed on the screen.

```python
visualize:
  true
```

If *true* a simulation will produce two additional files in the output directory, *network.dat* and *network.gexf*. *network.dat* contains two columns, where the entities in the first column are following the entities in the second column. *network.gexf*
is a file which can be used to visualize smulated networks on programs like [Gephi](http://gephi.github.io/).

```python
entity_stats:
  true
```

If *true*, a simulation will produce additional files in the output directory named after the particular entity types within the simulation as *entitytype_info.dat* (e.g. the presence of a Standard entity type will produce a file labelled *Standard_info.dat*).
Within this file can be found !!!!!!!!!!!!!!!

```python
degree_distributions:
  true
```

If *true*, the in, out, and cumulative degree distributions will be produced in the output directory for each simulated month, as, in the case of the zeroth month, in-degree_distribution_month_000.dat, out-degree_distribution_month_000.dat, and
cumulative-degree_distribution_month_000.dat respectively.

```python
tweet_analysis:
  true
```



```python
retweet_visualization:
  true
```

If *true*, a simulation will produce an additional file titled *retweet_viz.gexf*, which can be used to visualize the most retweeted tweets on programs such as [Gephi](http://gephi.github.io/).

```python
main_statistics:
  true
```



## ranks

```python
tweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}
```

Serves as a way to organize and categorize tweets in bins. The *bin_spacing* can be linear, quadratic, or cubic, and the can have an arbitrary minimum, maximum, and increment value. In the above case, entities with 10 or less tweets will be stored on a list in the
intitial bin. If some of these entities continue to tweet, and end up having a total number of tweets greater than 10 but less than 20, they will move to the next bin. If their total number of tweets increases to a value greater than 20 but less than 30, they will
then move on to the next bin, and so on, until they have reached a value of 300 tweets, where they shall remain.

```python
retweet_ranks:
  thresholds: {bin_spacing: linear, min: 10, max: 300, increment: 10}
```

Serves the exact same purpose as *tweet_ranks*, but for retweets.

```python
follow_ranks:
  thresholds: {bin_spacing: linear, min: 0, max: 100001, increment: 1}
  weights:    {bin_spacing: linear, min: 1, max: 100000, increment: 1}
```

Similar to *tweet_ranks* and *retweet_ranks*, *follow_ranks* serves to categorize and organize based on follows. However, an additional factor is taken into consideration, the bin weights.!!!!!!!!!!!!!!

## tweet observations

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



## ideologies

```python
  - name: Red
  - name: Blue
```

The *ideologies* are the arbitrary dogma that you would like entities of that ideology to have. They can be named whatever you would like, and can be as numerous as you would like as well, though their quantity must be equal or less than the number of
N_BIN_IDEOLOGIES specified in config_static.h. 

## regions

```python
- name: Ontario
    add_weight: 1

    preference_class_weights: {StandardPref: 100}
    ideology_weights: {Red: 100, Blue: 100}
    language_weights: {English: 50, French: 25, French+English: 25}
```

This sections outlines the regions in which you would like your entities to be located. You can have as many as you want, though it is important to remember that the number of remember that the number of regions must be equal or less than the value of 
N_BIN_REGIONS in config_static.h. *add_weight* corresponds to the probability that the entity added to the network is from that region, and is weighted with respect to those of the other inputted regions.
The *ideology_weights* are the possible ideologies that an entity from that region can have, and are weighted with respect to one another. Similar to the *ideology_weights*, the *language_weights* are the possible languages that an entity from that region can speak,
and are also weighted with respect to each other.

## config_static

```python
humour_bins: 2
```



## preference classes

<iframe width="560" height="315" src="https://www.youtube.com/embed/Sx0U1Fy5Bbo" frameborder="0" allowfullscreen></iframe>

```python
- name: StandardPref

   tweet_transmission:
      plain:
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
   follow_reaction_prob:
      0.5
```

The *preference classes* are the traits of an entity that influence whether or not that entity will retweet the tweet of another user. The four possible types of tweets are plain tweets, music-related tweets, ideological tweets, and humourous tweets.
Under *tweet_transmissions* one can dictate the probabilities that each type of entity will retweet a tweet, based on the content of that tweet, whether it be plain or musical, have a different ideology, have the same ideology, or be humourous.
The *follow_reaction_prob* relates to the probability that an entity will follow another user after seeing one of their tweets retweeted by an twitter user that that entity is following.   

## entities

<iframe width="560" height="315" src="https://www.youtube.com/embed/w_BVsSwb7cw" frameborder="0" allowfullscreen></iframe>

```python
- name: Standard
    weights:
      add: 80
      follow: 5
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humourous: 1.0
    followback_probability: 44
    hashtag_follow_options:
      care_about_region: true
      care_about_ideology: false
    rates:
        follow: {function: constant, value: 0.1}
        tweet: {function: constant, value: 0.01}
```

In this section one can identify the types of entities with which to include in their simulation. Under weights we can see *add* which correlates to the probability that the type of entity will be the one added to the network,
weighted against the other types of entities. The value inputted into *follow* will be the probability, weighted against the other entity types, in which this entity is followed in the entity follow model by other entities.
*tweet_type* summarizes the weighted probabilities that the content of an entity of this type's tweet will be ideological, plain, musical, or humourous. The *follow_back* probability is the probability that following an entity of this type
will result in that entity following you back. The *hashtag_follow_options* are useful for a hashtag follow model, where entities follow other entities based on the hashtags associated with their tweets. In this follow model, if *care_about_region* is set to *true*, 
then entities will only follow other entities that live in the same region as them, while this won't matter to them if set to *false*. If *care_about_ideology* is set to *true*, then entities will only follow other entities that share the same ideology as them,
while this won't matter to them if set to false. It is important to note that *use_hashtag_probability* in the **analysis** section must be set to a value greater than zero for this to work, or else no one will be implementing hastags into their tweets.
The *follow* and *tweet* rates are the rates in which entities of this particular type will follow and tweet with respect to simulated minues respectively. It works in the exact same manner as the add rate outlined in the **rates** section.

## Commands

* `~[2] mkdocs new [dir-name]` - Create a new project.
* `mkdocs serve` - Start the live-reloading docs server.
* `mkdocs build` - Build the documentation site.
* `mkdocs help` - Print this help message.

To launch #k@, we use the run command

`./run.sh`

The run command can take several command line arguements 

`./run.sh --build`

