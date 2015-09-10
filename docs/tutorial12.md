[hashkat.org](http://hashkat.org)

# A Diverse Network of Agents

As you have clearly noticed working through these tutorials, ***#k@*** offers the opportunity for users to create network simulations consisting of a truly dynamic and varied set of agents. A multilingual, multiregional network consisting of agents of many different ideologies, preferences, personalities, and types is one that truly models the human race and is exactly what we strive to do here with ***#k@***.

In this tutorial, we will go over every aspect in which you can individualize agents in your network, to give you the freedom and means to create the most realistic network simulations possible. We will also use what we learn in this tutorial to simulate a diverse network of several different agents and see which agent type ends up being the most popular. This tutorial should take about 40 minutes to complete.

## What Makes an Agent Unique?

### Ideology

An agent's ideology is the particular dogma that they follow. Ideologies can be added, removed, or renamed in the *ideologies* section of *INFILE.yaml*.

```python
ideologies:
  - name: Red
  - name: Blue
  - name: Green
  - name: Orange
```

The proportion of agents that have each particular ideology in a certain region can be modified in the *ideology_weights* of a particular region in the *regions* section of the input file.

```python
regions:
  - name: Ontario

    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100}
```

Agents can also follow other agents based on ideology in the *hashtag* follow model, where setting the *hashtag* follow option *care_about_idelogy* in the agent type section to *true* results in agents only following other agents that share the same ideology as them. 

```python
agents:
  - name: Standard
    hashtag_follow_options:  
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
```

### Language

As you would expect, an agent's language is the particular language that they speak. The language of an agent is determined in the *regions* section of *INFILE.yaml* under *language_weights*, where the proportion of a particular language that agents in that region speak can be weighted against other languages in that region. Agents will never be able to connect with other agents that speak a different language from them, since they are unable to understand one another.

```python
regions:
  - name: Ontario

    language_weights: {English: 100, French: 0, Spanish: 0, French+English: 0}
```

### Region

A region is the geographical location of an agent. *regions* can be named, added, or removed in the *regions* section of *INFILE.yaml*. Their *add_weight* determines the proportion of agents in a network simulation being from that region as opposed to others. Their *preference_class_weights*, *ideology_weights*, and *language_weights*, also weighs the proportion of agents that will have a particular preference class, ideology, or language respectively in that region as opposed to other agents within that region. 

```python
regions:
  - name: Ontario
    add_weight: 5

    preference_class_weights: {NoRetweetPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100}
    language_weights: {English: 100, French: 0, Spanish: 0, French+English: 0}
```

### Preference Class - Tweet Transmission

*tweet_transmission* is the probability that an agent of a particular agent type will be retweeted based on tweet content. This is defined in the *preference_classes* section of *INFILE.yaml*.

```python
preference_classes:
 - name: StandardPref

   tweet_transmission: 
      plain: # Also applies to musical tweets
        Standard: 0.01
        Celebrity: 0.01
        else: 0.0
      different_ideology: # generally no retweeting of tweets with different ideological content
        all: 0.0
      same_ideology:
        Standard: 0.01
        else: 0.2
      humorous:
        Standard: 0.02
        Celebrity: 0.02
        else: 0.4
```

This is also weighted in the *preference_class_weights* factor of a region in the *regions* section. 

```python
regions:
  - name: Ontario
    
    preference_class_weights: {NoRetweetPref: 100}
```

### Preference Class - Follow Reaction Probability

The *follow_reaction_probability* is the probability that an agent, after seeing a tweet be retweeted by someone they follow, will follow the speaker of the original tweet as opposed to just retweeting the retweet.

```python
preference_classes:
 - name: StandardPref

   follow_reaction_prob:
      0.1
```

### Agent - Add Weight

Defined in the *agents* section of *INFILE.yaml*, the *add* weight is the proportion in which agents of a particular agent type are added into the network in comparison to agents of other agent types.

```python
agents:
  - name: Standard
    weights:
      # Weight with which this agent is created
      add: 100.0
```

### Agent - Follow Weight

Defined in the *agents* section of *INFILE.yaml*, the *follow* weight is the proportion in which agents of a particular agent type are followed in comparison to agents of other agent types in the *agent* or *preferential_agent* follow models.

```python
agents:
  - name: Standard
    weights:
      # Weight with which this agent is followed in agent follow
      follow: 100.0
```

### Agent - Tweet Type

Defined in the *agents* section of *INFILE.yaml*, *tweet_type* outlines the weight in which agents of a particular agent type make *ideological*, *plain*, *musical*, or *humorous* tweets. 

```python
agents:
  - name: Standard
    weights:
      tweet_type:
        ideological: 1.0
        plain: 1.0
        musical: 1.0
        humorous: 1.0
```

### Agent - Followback Probability

Defined in the *agents* section of *INFILE.yaml*, *followback_probability* is the probability in which agents of a particular agent type follow agents that have followed them. This value has been found to be about 44% in experiment.

```python
agents:
  - name: Standard
    # Probability that following this agent results in a follow-back
    followback_probability: .44
```

In order for *followback_probability* to be enabled, *use_followback* must be set to *true* in the *analysis* section of *INFILE.yaml*.

```python
analysis:
use_followback: 
  true        # from literature, it makes sense for a realistic system to have followback enabled
```

### Agent - Hashtag Follow Options

Defined in the *agents* section of *INFILE.yaml*, the *hashtag_follow_options* determine who agents of a particular agent type follow in the *hashtag* follow model. Whether they follow another agent only if they share the same region and/or ideology as them is based on whether or not you input *true* for the *care_about_region* and/or *care_about_ideology* variable respectively.

```python
agents:
  - name: Standard
  hashtag_follow_options:
    care_about_region: true # does the agent care about where the agent they will follow is from?
    care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
```

### Agent - Follow Rate

Defined in the *agents* section of *INFILE.yaml*, the *follow* rate dictates the amount of follows an agent of that particular agent type does per simulated minute. The function of this rate can be either *constant* or *linear*. For a constant *follow* rate, the *function* must be set to *constant* and it must have a *value* of 0 or greater assigned to it.

```python
agents:
  - name: Standard
    rates: 
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0001}
```

For a linear *follow* rate, the *function* must be set to *linear* and there must be a *y_intercept* and *slope* instead of a *value*. The *y_intercept* is the initial value of follows per simulated minute and the *slope*, multipled by the number of simulated months that have elapsed within the simulation, is the value by which the *y_intercept* is increased by.

```python
agents:
  - name: Standard
    rates: 
        # Rate for follows from this agent:
        follow: {function: linear, y_intercept: 0.1, slope: 0.1}
```

**Note**: the *add* rate can also be a linear function.

### Agent - Tweet Rate

Defined in the *agents* section of *INFILE.yaml*, the *tweet* rate dictates the amount of tweets an agent of that particular agent type makes per simulated minute. The function of this rate can be either *constant* or *linear*. For a constant *tweet* rate, the *function* must be set to *constant* and it must have a *value* of 0 or greater assigned to it.

```python
agents:
  - name: Standard
    rates: 
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.0001}
```

For a linear *tweet* rate, the *function* must be set to *linear* and there must be a *y_intercept* and *slope* instead of a *value*. The *y_intercept* is the initial value of tweets per simulated minute and the *slope*, multipled by the number of simulated months that have elapsed within the simulation, is the value by which the *y_intercept* is increased by.


```python
agents:
  - name: Standard
    rates: 
        # Rate for tweets from this agent:
        tweet: {function: linear, y_intercept: 0.1, slope: 0.1}
```

**Note**: the *add* rate can also be a linear function.

**Note**: if you'd like to change the number of *ideologies*, *regions*, *preference classes*, and/or agent types in the simulation, make sure these numbers are less than or equal to (in the case of *regions*, EXACTLY equal to) the value set for *N_BIN_IDEOLOGIES*, *N_BIN_REGIONS*, *N_BIN_PREFERENCE_CLASS*, and/or *N_BIN_AGENT_TYPES* respectively in the *config_static.h* file in **src**. You can change any of these values if need be, but make sure to then rebuild ***#k@*** by running *build.sh -O*, in order for these changes to take effect.

## Popularity Contest

Now that we have seen all the factors that make agents unique, we are going to create a network simulation that consists of these diverse agents, and see which agent type out of the ones we create is the most popular.

For this exercise, we will actually not go through how to configure *INFILE.yaml* to create the desired network with you. You've had enough experience with it that you should be able to do this on your own. We're just going to outline the network that we would like. If you're having any problems with this, the desired input file can be found for reference in the **docs/tutorial_input_files** directory in **hashkat**, with this one under the title **tutorial12**. You can also view the desired input file you will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial12/INFILE.yaml). Now, on to creating the network.

For this network simulation, we would like to have the following:

* a constant number of 100 agents
* a maximum simulation time of 1000 simulated minutes
* a maximum real time of 10 minutes
* followback implemented
* the *twitter* follow model implemented, with the *twitter_suggest* and the *hashtag* follow model evenly weighted and the other follow models being excluded from this simulation
* No unfollowing allowed
* Hashtag present in tweets 50% of the time
* 4 ideologies: *Red*, *Blue*, *Green*, and *Orange*
* agents being located in either *Ontario* or *Quebec*, with the ratio of Ontarians to Quebecers being 3:2
* Ontarians having a *StandardPref* preference class, with 4 evenly wieghted ideologies and the ratio of English to French speaking Ontarians being 1:1
* Quebecers having the *NoRetweetPref* preference class, with only 3 ideologies (*Red*, *Blue*, *Green*) evenly weighted, with the ratio of English speaking to French speaking Quebecers being 1:1
* for the *StandardPref* preference class, the tweet transmission rates for the agent types present in this network are 0.1 for *plain*/*musical* tweets, 0.0 for *different_ideology* tweets, 0.2 for *same_ideology* tweets, and 0.4 for *humorous* tweets, with the *follow_reaction_prob* being 0.3
* the *NoRetweetPref* preference class will remain the same, with a value of 0 for all variables

We would also like you to use only the following agent types with their respective characteristics:

### Standard - Self-Centered Chatty Agent

<center>
<img src='../img/trading_cards/standard_self_centered_chatty_card.jpg'>
</center>

```python
- name: Standard-Chatty
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5.0
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
```

### Standard - Friendly Networking Agent

<center>
<img src='../img/trading_cards/standard_friendly_networking_card.jpg'>
</center>

```python
- name: Standard-Friendly
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5.0
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
```

### Standard - Funny Social Commentator Agent

<center>
<img src='../img/trading_cards/standard_funny_social_commentator_card.jpg'>
</center>

```python
- name: Standard-Funny
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5.0
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
```

### Standard - Music Advocate Agent

<center>
<img src='../img/trading_cards/standard_music_advocate_card.jpg'>
</center>

```python
- name: Standard-Musical
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5.0
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
```

### Standard - Political Satirist Agent

<center>
<img src='../img/trading_cards/standard_political_satirist_card.jpg'>
</center>

```python
- name: Standard-Political
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5.0
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
```

Make sure that your tweet, retweet, and follow ranks have also been updated accordingly to properly account for this network. As you've probably noticed, we've set the *max_real_time* to 10 minutes instead of the usual 1 minute. The simulation should definitely not run for this long, we just want to allow enough time for the simulation to run to completion. When running this simulation on our computer, it took almost 4 minutes real time to complete due to the complexity of the network. You do not have to run your simulation for this long if you wish. You can change the *max_real_time* to a time of your choosing or press **Ctrl-c** at any time during the simulation to stop it. Once you have configured *INFILE.yaml* to account for all this, run and visualize your network simulation.

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/tR0TrumrZSM" frameborder="0" allowfullscreen></iframe>
</center>

Running the network simulation to completion, we produced the following network visualization:

<center>
<img src='../img/tutorial12/visualization.png'>
</center>

In the above visualization, the red nodes correspond to the *Standard-Chatty* agents, the green nodes correspond to the *Standard-Friendly* agents, the yellow nodes correspond to the *Standard-Funny* agents, the blue nodes correspond to *Standard-Musical* agents, and the purple nodes correspond to the *Standard-Political* agents.

As we can see, our social network has been divided into networks based on language, with one network corresponding to English-speaking agents and the other corresponding to French-speaking agents.

Clicking the 'Filters' tab on the right-hand side of the page in Gephi and clicking 'Degree-Range' under 'Topology', we can filter our network visualization based on the cumulative-degree of the agents in the network.

<center>
<img src='../img/tutorial12/filter_tab.png'>
</center>

Setting the 'Degree-Range Settings' from 80 to 100 and clicking 'Filter', we can see the following:

<center>
<img src='../img/tutorial12/filtered_visualization.png'>
</center>

As we can see from this visualization, the most highly connected nodes are the green and yellow nodes, also known as the *Standard-Friendly* and *Standard-Funny* agents. Therefore, it is quite clear that the friendly and funny agents won the popularity contest.

Looking into the *region_connection_matrix_month_000.dat* output file, we can also see the connections agents made within and between regions:

<center>
<img src='../img/tutorial12/region_connection_matrix.png'>
</center>

As we can see, agents in *Ontario*, the zeroth region (region '0' corresponds to the first region in *INFILE.yaml*, region '1' to the second region, etc.), had 65.0394% of their follows from agents from *Ontario* and 34.9609% of their follows from agents in *Quebec*, while agents in *Quebec* had 52.946% of their follows from agents in *Ontario* and 47.054% of their follows from agents in *Quebec*. Therefore, it seems that agents in *Ontario* tended to interact amongst themselves while agents in *Quebec* were pretty evenly interacting with agents in both *Ontario* and *Quebec*.

## Next Steps

With the completion of this tutorial, you now have knowledge and experience with every different agent configuration you can implement into ***#k@***. You are welcome to proceed to the next [tutorial](http://docs.hashkat.org/en/latest/tutorial13/), where we demonstrate some of the situations you can model using ***#k@***.