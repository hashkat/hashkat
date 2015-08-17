[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# A Complex Network

## Modelling Different Scenarios

Up to this point, we have used #k@ in very abstract terms. 

### Scenario 1 - Organization Rivalry

We have two organizations, *Coke* and *Pepsi*, who are taking full advantage of the access social networks allow them to have with consumers. They have both made accounts on this social network and both currently have zero followers. They have decided to each launch their own marketing campaign to garner as much follows from *Standard* agents in this social network as possible in a span of one week.

Both *Coke* and *Pepsi* only follow agents that follow them, and they will do so for every single one of their followers. *Pepsi* tweets both musical and humourous tweets evenly weighted, while *Coke* only tweets humourous tweets, though they tweet at a slightly higher rate. Humourous tweets do have a better chance of being retweeted however.

The **INFILE.yaml** used for this scenario can be found in the *docs/tutorial_input_files* directory in *hashkat* under *tutorial13_scenario_1* as well as [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial13_scenario_1/INFILE.yaml). To find out who wins this competition, copy this input file into your *hashkat* directory and run the simulation. Feel free to look through the input file to see what we did to produce this scenario. Due to the complexity of this network, the simulation may run slightly longer than the simulations done in previous tutorials.

#### Coke

<p align='center'>
<img src='../img/trading_cards/organization_coke_card.jpg'>
</p>

```python
- name: Organization-Coke
  weights:
    # Weight with which this agent is created
    add: 0.5
    # Weight with which this agent is followed in agent follow
    follow: 100.0
    tweet_type:
      ideological: 0.0
      plain: 0.0
      musical: 0.0
      humourous: 1.0 # Can be considered the humourousness of the agent type
  # Probability that following this agent results in a follow-back
  followback_probability: 1.0
  hashtag_follow_options:
    care_about_region: false # does the agent care about where the agent they will follow is from?
    care_about_ideology: false # does the agent care about which ideology the agent has?
  rates:
      # Rate for follows from this agent:
      follow: {function: constant, value: 0.0}
      # Rate for tweets from this agent:
      tweet: {function: constant, value: 0.11}
```

#### Pepsi

<p align='center'>
<img src='../img/trading_cards/organization_pepsi_card.jpg'>
</p>

```python
- name: Organization-Pepsi
  weights:
    # Weight with which this agent is created
    add: 0.5
    # Weight with which this agent is followed in agent follow
    follow: 100.0
    tweet_type:
      ideological: 0.0
      plain: 0.0
      musical: 1.0
      humourous: 1.0 # Can be considered the humourousness of the agent type
  # Probability that following this agent results in a follow-back
  followback_probability: 1.00
  hashtag_follow_options:
    care_about_region: false # does the agent care about where the agent they will follow is from?
    care_about_ideology: false # does the agent care about which ideology the agent has?
  rates:
      # Rate for follows from this agent:
      follow: {function: constant, value: 0.0}
      # Rate for tweets from this agent:
      tweet: {function: constant, value: 0.10}
```

### Scenario 2 - Different Regions, Different Ideologies

We have two regions in our network, Canada and the USA, and both are in the midst of a federal election. In both regions, there are two main political parties, 'Red' and 'Blue'. However, the 'Red' party and 'Blue' party in Canada are vastly different from the 'Red' party and 'Blue' party in the USA. Citizens of these regions have taken to this social network to voice their views on their particular regions' upcoming elections. These citizens can be categorized into 2 main agent types in the social network. These agent types are:

#### Red

<p align='center'>
<img src='../img/trading_cards/standard_red_card.jpg'>
</p>

```python
- name: Standard-Red
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5
    tweet_type:
      ideological: 1.0
      plain: 1.0
      musical: 1.0
      humourous: 1.0 # Can be considered the humourousness of the agent type
  # Probability that following this agent results in a follow-back
  followback_probability: .44
  hashtag_follow_options:
    care_about_region: true # does the agent care about where the agent they will follow is from?
    care_about_ideology: true # does the agent care about which ideology the agent has?
  rates:
      # Rate for follows from this agent:
      follow: {function: linear, y_intercept: 0.01, slope: 0.001}
      # Rate for tweets from this agent:
      tweet: {function: linear, y_intercept: 0.01, slope: 0.001}
```

#### Blue

<p align='center'>
<img src='../img/trading_cards/standard_blue_card.jpg'>
</p>

```python
- name: Standard-Blue
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 5
    tweet_type:
      ideological: 1.0
      plain: 1.0
      musical: 1.0
      humourous: 1.0 # Can be considered the humourousness of the agent type
  # Probability that following this agent results in a follow-back
  followback_probability: .44
  hashtag_follow_options:
    care_about_region: true # does the agent care about where the agent they $
    care_about_ideology: true # does the agent care about which ideology the $
  rates:
      # Rate for follows from this agent:
      follow: {function: linear, y_intercept: 0.01, slope: 0.001}
      # Rate for tweets from this agent:
      tweet: {function: linear, y_intercept: 0.01, slope: 0.001}
```

Agents started voicing their views 5 days before the day of their country's federal election, which coincidentally happen to occur in both countries on the very same day. The **INFILE.yaml** used for this scenario can be found in the *docs/tutorial_input_files* directory in *hashkat* under *tutorial13_scenario_2* as well as [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial13_scenario_2/INFILE.yaml). Copy this input file into your *hashkat* directory and run the simulation. Feel free to look through the input file to see what we did to produce this scenario. Due to the complexity of this network, the simulation may run slightly longer than the simulations done in previous tutorials. Judging by the number of supporters you will find in the social network, find a way to discover who will win the election in both Canada and the USA.

## Moving Forward with #k@

With the completion of this, the final tutorial, you now have proficency and experience working with all the different implementations and aspects of #k@. However, we have only scratched the surface of all the different applications and utilizations of #k@. From the World Wide Web to a food web, from people to atoms, there is no end to all the entities and situations you can model using #k@. With that, press on and experiment with #k@, and see what you can model using this program.

Check out the following pages for further information on some of the components of #k@. Also, please send us your thoughts on #k@ and this documentation on the **Feedback** page. We would love to hear from you!