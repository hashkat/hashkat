[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# A Complex Network

## Modelling Different Scenarios

Up to this point, we have used #k@ in very abstract terms. 

### Scenario 1 - Organization Rivalry

We have two organizations, *Coke* and *Pepsi*, who are taking full advantage of the access social networks allow them to have with consumers. They have both made accounts on this social network and both currently have zero followers. They have decided to each launch their own marketing campaign to,  garner as much follows from *Standard* agents on this social network as possible in a span of one week.

Both *Coke* and *Pepsi* only follow agents that follow them, and they will do so for every single one of their followers. *Pepsi* tweets both musical and humourous tweets evenly weighted, while *Coke* only tweets humourous tweets, though they tweet at a slightly higher rate. Musical tweets do have a slightly better chance of being retweeted however.

The **INFILE.yaml** used for this scenario can be found in the *tutorials* directory in *hashkat* under *tutorial14* as well as [here](). To find out who wins this competition, copy this input file into your *hashkat* directory and run the simulation. Feel free to look through the input file to see what we did to produce this scenario.

#### Coke

```python
- name: Coke
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 50
    tweet_type:
      ideological: 0.0
      plain: 0.0
      musical: 0.0
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
      tweet: {function: constant, value: 0.0101}
```

#### Pepsi

```python
```python
- name: Pepsi
  weights:
    # Weight with which this agent is created
    add: 100.0
    # Weight with which this agent is followed in agent follow
    follow: 50
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
      tweet: {function: constant, value: 0.01}
```
```

### Scenario 2 - Different Regions, Different Ideologies



### Scenario 3 - A Moment That Gets People Talking



## Constructing a Realisitic Network



## Moving Forward with #k@

With the completion of this, the final tutorial, you now have proficency and experience working with all the different implementations and aspects of #k@. However, we have only scratched the surface of all the different applications and utilizations of #k@. From the World Wide Web to a food web, from people to atoms, there is no end to all the entities and situations you can model using #k@. With that, press on and experiment with #k@, and see what you can model using this program.

Check out the following pages for further information on some of the components of #k@. Also, please send us your thoughts on #k@ and these tutorials on the **Feedback** page. We would love to hear from you.
