[hashkat.org](http://hashkat.org)

# Modelling Different Scenarios

Up to this point, we have demonstrated **#k@** with simple networks. Let's use it now to model real life scenarios that may occur in social networks.

### Scenario 1 - Organization Rivalry

We have created two agent_types to be rival organizations in this simulation, and have named them 'Coke' and 'Pepsi'.  Both organizations want to take full advantage of social networks to connect with consumers.  Both organizations have made social media accounts but both currently have zero followers.  Each has decided to launch a marketing campaign to obtain as many 'Standard' followers as possible within one simulated week.

Both Coke and Pepsi only follow agents that follow them, and they will each follow_back 100% of the agents who follow them.

Pepsi's' strategy is to tweet both 'musical' and 'humorous' tweets in equal proportions, while Coke tweets only 'humorous' tweets but at a slightly higher rate. Humourous tweets have a better chance of being retweeted however their scope is limited by language, while the scope of 'musical' tweets is unlimited.

The configuration file for this scenario can be found in the [/hashkat/docs/tutorial_input_files/tutorial13_scenario_1/](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial13_scenario_1/INFILE.yaml). 

To find out who wins this competition, copy these agent_types into your **INFILE.yaml** and run the simulation.  

#### Coke

<center>
<img src='../img/trading_cards/organization_coke_card.jpg'>
</center>

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
      humorous: 1.0
  # Probability that following this agent results in a follow-back
  followback_probability: 1.0
  hashtag_follow_options:
    care_about_region: false # does the agent care about where the agent they will follow is from?
    care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
  rates:
      # Rate for follows from this agent:
      follow: {function: constant, value: 0.0}
      # Rate for tweets from this agent:
      tweet: {function: constant, value: 0.11}
```

#### Pepsi

<center>
<img src='../img/trading_cards/organization_pepsi_card.jpg'>
</center>

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
      humorous: 1.0
  # Probability that following this agent results in a follow-back
  followback_probability: 1.00
  hashtag_follow_options:
    care_about_region: false # does the agent care about where the agent they will follow is from?
    care_about_ideology: false # does the agent care about the ideology of the agent they will follow?
  rates:
      # Rate for follows from this agent:
      follow: {function: constant, value: 0.0}
      # Rate for tweets from this agent:
      tweet: {function: constant, value: 0.10}
```
Due to the complexity of this network, this simulation may take longer to run than simulations in previous tutorials.

### Scenario 2 - Different Regions, Different Ideologies

For this simulation we created two regions, 'Canada' and the 'USA'.  Both regions are in the lead up to a national election, which will take place on the same day. 

In both regions there are two main political parties, 'Red' and 'Blue', however the Red and Blue parties in Canada are vastly different from the Red and Blue parties in the USA.  Citizens of these regions have taken to the social network to voice their views on the upcoming elections. These citizens can be categorized into 2 main agent_types in the social network. These agent_types are:

#### Red

<center>
<img src='../img/trading_cards/standard_red_card.jpg'>
</center>

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
      humorous: 1.0
  # Probability that following this agent results in a follow-back
  followback_probability: .44
  hashtag_follow_options:
    care_about_region: true # does the agent care about where the agent they will follow is from?
    care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
  rates: 
      # Rate for follows from this agent:
      follow: {function: linear, y_intercept: 0.01, slope: 0.001}
      # Rate for tweets from this agent:
      tweet: {function: linear, y_intercept: 0.01, slope: 0.001}
```

#### Blue

<center>
<img src='../img/trading_cards/standard_blue_card.jpg'>
</center>

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
      humorous: 1.0
  # Probability that following this agent results in a follow-back
  followback_probability: .44
  hashtag_follow_options:
    care_about_region: true # does the agent care about where the agent they will follow is from?
    care_about_ideology: true # does the agent care about the ideology of the agent they will follow?
  rates:
      # Rate for follows from this agent:
      follow: {function: linear, y_intercept: 0.01, slope: 0.001}
      # Rate for tweets from this agent:
      tweet: {function: linear, y_intercept: 0.01, slope: 0.001}
```

Five days before the national election both agent_types start tweeting ideological content, and increase their use of the social network. The configuration file used for this scenario can be found in [/hashkat/docs/tutorial_input_files/tutorial13_scenario_2/](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial13_scenario_2/INFILE.yaml). 

Remove Coke and Pepsi and copy these two regions and agent_types into your **INFILE.yaml** file and run the simulation.  

#### Pick a Winner

Use the output files generated to discover, in each simulation, which agent_type 'won' the most number of followers.

#### Moving Forward

With the completion of this tutorial you now have proficiency and experience working with the different implementations and aspects of **#k@**. However we have only scratched the surface of the different simulations that may be run in **#k@**. From the World Wide Web to an environmental food web, from people to atoms, there is no end to networks that may be modelled with **#k@**. 

The following pages contain further information on some of the components of **#k@**.  Please send us your thoughts on **#k@** and this documentation via [Feedback](http://docs.hashkat.org/en/latest/feedback/). We would love to hear from you!
