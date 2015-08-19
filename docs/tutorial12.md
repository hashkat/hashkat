[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# A Diverse Network of Agents

As you have clearly noticed working through these tutorials, ***#k@*** offers the opportunity for users to create network simulations consisiting of a truly dynamic and varied set of agents. A multilingual, multiregional network consisting of agents of many different ideologies, preferences, personalities, and types is one that truly models the human race and is exactly what we strive to do here with ***#k@***.

In this tutorial, we will go over every aspect in which you can individualize agents in your network, to give you the freedom and means to create the most realistic network simulations possible. We will also use what we learn in this tutorial to simulate a diverse network of several different agents and see which agent type ends up being the most popular. This tutorial should take about 45 minutes to complete.

## What Makes an Agent Unique?

### Ideology

As previously discussed, an agent's idelogy is the particular dogma that they follow. Ideologies can be added, removed, or renamed in the *ideologies* section of **INFILE.yaml**.

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

Agents can also follow other agents based on ideology in the *hashtag* follow model, where setting the *hashtag* follow option *care_about_idelogy* in the agent type section to true results in agents only following other agents that share the same ideology as them. 

```python
agents:
  - name: Standard
    hashtag_follow_options:  
      care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
```

### Language

As you would expect, an agent's language is the particular language that they speak. The language of an agent is determined in the *regions* section of **INFILE.yaml** under *language_weights*, where they can be added, removed, or renamed, as well as the proportion of a particular language that agents in that region speak can be weighted against other languages in that region. Agents will never be able to connect with other agents that speak a different language from them, since they are unable to understand one another.

```python
regions:
  - name: Ontario

    language_weights: {English: 100, French: 0, Spanish: 0, French+English: 0}
```

### Region

'Region' outlines the particular geographical area that an agent lives in. *regions* can be named, added, or removed in the *regions* section of **INFILE.yaml**. Their *add_weight* determines the proportion of agents in a network simulation being from that region as opposed to others. Their *preference_class_weights*, *ideology_weights*, and *language_weights*, also weighs the proportion of agents that will have a particular preference class, ideology, or language respectively in the network as opposed to others within the region. 

```python
regions:
  - name: Ontario
    add_weight: 5

    preference_class_weights: {NoRetweetPref: 100}
    ideology_weights: {Red: 100, Blue: 100, Green: 100, Orange: 100}
    language_weights: {English: 100, French: 0, Spanish: 0, French+English: 0}
```

### Preference Class - Tweet Transmission

An agent's tweet transmission is the probability that an agent will retweet a tweet of a particular type. This is defined in the *preference_classes* section of **INFILE.yaml**, where the probability that an agent will retweet a tweet of the possible tweet types (plain and musical, different ideology, same ideology, humorous) is determined by its agent type. 

```python
preference_classes:
 - name: StandardPref

   tweet_transmission: 
      plain: # Also applies to musical tweets
        Standard: 0.01
        Celebrity: 0.01
        else: 0.1
      different_ideology: # generally no retweeting of tweets with different ideological content
        Standard: 0.00
        Celebrity: 0.00
        else: 0.0
      same_ideology:
        Standard: 0.01
        Celebrity: 0.01
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

An agent's *follow_reaction_probability* is the probability that an agent, after seeing a tweet be retweeted by someone they follow, will follow the speaker of the original tweet as opposed to just retweeting the tweet.

```python
preference_classes:
 - name: StandardPref

   follow_reaction_prob:
      0.1
```

### Agent - Add Weight

Defined in the *agents* section of **INFILE.yaml**, an agent's add weight is the proportion in which agents of a particular agent type are added into the network in comparison to those of other types.

```python
agents:
  - name: Standard
    weights:
      # Weight with which this agent is created
      add: 100.0
```

### Agent - Follow Weight

Defined in the *agents* section of **INFILE.yaml**, an agent's follow weight is the proportion in which agents of a particular agent type are followed with respect to those of other agent types in the *agent* or *preferential_agent* follow models.

```python
agents:
  - name: Standard
    weights:
      # Weight with which this agent is followed in agent follow
      follow: 100.0
```

### Agent - Tweet Type

Defined in the *agents* section of **INFILE.yaml**, *tweet_type* outlines the weight in which agents of a particular agent type make tweets categorized into *ideological*, *plain*, *musical*, or *humorous* based on their content. 

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

Defined in the *agents* section of **INFILE.yaml**, an agent's *followback_probability* is the probability in which agents of a particular agent type follow other agents that have followed them. This value has been found to be about 44% or 0.44 in experiment.

```python
agents:
  - name: Standard
    # Probability that following this agent results in a follow-back
    followback_probability: .44
```

In order for *followback_probability* to be enabled, *use_followback* must be set to *true* in the *section* of **INFILE.yaml**.

```python
analysis:
use_followback: 
  true        # from literature, it makes sense for a realistic system to have followback enabled
```

### Agent - Hashtag Follow Options

Defined in the *agents* section of **INFILE.yaml**, the *hashtag_follow_options* determine who agents follow in the *hashtag* follow model. Whether they follow another agent only if they share the same region and/or ideology as them is based on whether or not you put *true* for the *care_about_region* and/or *care_about_ideology* variable respectively.

```python
agents:
  - name: Standard
  hashtag_follow_options:
    care_about_region: true # does the agent care about where the agent they will follow is from?
    care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
```

### Agent - Follow Rate

Defined in the *agents* section of **INFILE.yaml**, an agent's follow rate dictates the amount of follows an agent of that particular agent type does per simulated minute. The function of this rate can be either constant or linear. For a constant follow rate, the function must be set to *constant* and it must have a value of 0 or greater assigned to it.

```python
agents:
  - name: Standard
    rates: 
        # Rate for follows from this agent:
        follow: {function: constant, value: 0.0001}
```

For a linear follow rate, the function must be set to *linear* and there must be a *y_intercept* and *slope* instead of a *value* (this can also be done for the add rate).

```python
agents:
  - name: Standard
    rates: 
        # Rate for follows from this agent:
        follow: {function: linear, y_intercept: 0.1, slope: 0.1}
```

### Agent - Tweet Rate

Defined in the *agents* section of **INFILE.yaml**, an agent's tweet rate dictates the amount of tweets an agent makes of the particular agent type per ismulated minute. The function of this rate can be either constant or linear. For a constant tweet rate, the function must be set to *constant* and it must have a value of 0 or greater assigned to it.

```python
agents:
  - name: Standard
    rates: 
        # Rate for tweets from this agent:
        tweet: {function: constant, value: 0.0001}
```

For a linear tweet rate, the function must be set to *linear* and there must be a *y_intercept* and *slope* instead of a *value* (this can also be done for the add rate).

```python
agents:
  - name: Standard
    rates: 
        # Rate for tweets from this agent:
        tweet: {function: linear, y_intercept: 0.1, slope: 0.1}
```

## Popularity Contest

Now that we have seen all the factors that make agents unique, we are going to create a network simulation that consists of these diverse agents, and see which agent type out of the ones we created are the most popular.

For this exercise, we will actually not go through how to configure **INFILE.yaml** to create the desired network with you. You've had enough experience with it that you should be able to do this on your own. We're just going to outline the network that we would like. If you're having any problems with this, the desired input file can be found for reference in the *docs/tutorial_input_files* directory in hashkat, with this one under the title *tutorial12*. You can also view the desired input file you will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial12/INFILE.yaml). Now, on to creating the network.

For this network simulation, we would like to have the following:

* a constant number of 100 agents
* a maximum simulation time of 1000 simulated minutes
* a maximum real time of 10 minutes
* followback implemented
* the *twitter* follow model implemented, with the *twitter_suggest* and the *hashtag* follow model evenly weighted and the other follow models being excluded from this simulation
* No unfollowing allowed
* Hashtag presence in tweets 50% of the time
* 4 ideologies: *Red*, *Blue*, *Green*, and *Orange*
* agents being located in either *Ontario* or *Quebec*, with the ratio of Ontarians to Quebecers being 3:2
* Ontarians having a *StandardPref* preference class, with 4 evenly wieghted ideologies and the ratio of English to French speaking Ontarians being 1:1
* Quebecers having the *NoRetweetPref* preference class, with only 3 ideologies (*Red*, *Blue*, *Green*) evenly weighted, with the ratio of English speaking to French speaking Quebecers being 1:1
* for the *StandardPref* preference class, the tweet transmission rates for the agent types present in this network are 0.1 for 'plain'/'musical' tweets, 0.0 for 'different_ideology' tweets, 0.2 'same_ideology' tweets, and 0.4 for 'humorous' tweets, with the 'follow_reaction_prob' being 0.3
* the *NoRetweetPref* preference class will remain the same, with 0 for all

We would also like you to use only the following agent types with their respective characteristics:

### Standard - Self-Centered Chatty Agent

<p align='center'>
<img src='../img/trading_cards/standard_self_centered_chatty_card.jpg'>
</p>

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

<p align='center'>
<img src='../img/trading_cards/standard_friendly_networking_card.jpg'>
</p>

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

<p align='center'>
<img src='../img/trading_cards/standard_funny_social_commentator_card.jpg'>
</p>

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

<p align='center'>
<img src='../img/trading_cards/standard_music_advocate_card.jpg'>
</p>

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

<p align='center'>
<img src='../img/trading_cards/standard_political_satirist_card.jpg'>
</p>

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

Make sure that your tweet, retweet, and follow ranks have also been updated accordingly to properly account for this network. As you've probably noticed, we've set the max real time to 10 minutes instead of the regular 1 minute. The simulation should definitely not run for this long, we just want to allow enough time for the simulation to run to completion. When running this simulation on our computer, it took almost 4 minutes real time to complete due to the complexity of the network. You do not have to run your simulation for this long if you wish. You can change the maximum real time to a time of your choosing or press **Ctrl-c** at any time during the simulation to pause it. Once you have configured **INFILE.yaml** to account for all this, run and visualize your network simulation.

Running the network simulation to completion, we produced the following network visualization:

<p align='center'>
<img src='../img/tutorial12/visualization.png'>
</p>

In the above visualization, the red nodes correspond to the *Standard-Chatty* agents, the green nodes correspond to the *Standard-Friendly* agents, the yellow nodes correspond to the *Standard-Funny* agents, the blue nodes correspond to *Standard-Musical* agents, and the purple nodes correspond to the *Standard-Political* agents.

As we can see, our social network has been divided into networks based on language, with one network corresponding to English-speaking agents and the other corresponding to French-speaking agents.

Clicking the 'Filters' tab on the right-hand side of the page on 'Gephi' and clicking 'Degree-Range' under 'Topology', we can filter our network visualization based on the cumulative degree of the agents in the network.

<p align='center'>
<img src='../img/tutorial12/filter_tab.png'>
</p>

Setting the 'Degree-Range Settings' from 80 to 100 and clicking 'Filter', we can see the following:

<p align='center'>
<img src='../img/tutorial12/filtered_visualization.png'>
</p>

As we can see from this visualization, the most highly connected nodes are the green and yellow nodes, also known as the *Standard-Friendly* and *Standard-Funny* agents. Therefore, it is quite clear that the friendly and funny agents won the popularity contest.

## Next Steps

With the completion of this tutorial, you now have knowledge and experience with every different agent configuration you can implement into ***#k@***. You are welcome to proceed to the next [tutorial](http://docs.hashkat.org/en/latest/tutorial13/), where we demonstrate some of the situations you can model using ***#k@***.