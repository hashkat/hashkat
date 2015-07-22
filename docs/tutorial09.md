[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Other Methods of Following

In the previous tutorials, we have outlined the various follow models offered by hashkat. When implemented, each one produced a vastly different network, having a huge impact on the connections made by agents. However, when implementing a particular follow model, there are other factors that could be applied to the system to cause agents to follow others outside the framework of that simulation's follow model. These other factors that can be utilized in a network simulation are 'followback' and 'following via retweets'. In this tutorial, we will outline and discuss what these application mean, and compare a network simulation that does not involve these follow methods to a network simulation that does implement them.

All the files that will be used in this simulation can be found for reference in the tutorials directory in hashkat, with the ones used for this tutorial under the titles *tutorial09_without_other_follow* and *tutorial09_with_other_follow* respectively. You can view the input file we will be creating for the network simulation that does not involve the other follow methods [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial09_without_other_follow/INFILE.yaml), and the network simulation that does involve the other follow methods [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial09_with_other_follow/INFILE.yaml). This tutorial should take approximately 30 minutes to complete.

## Followback

'Followback' is when an agent follows another agent who followed them. Obviously, this implementation makes for a more realistic system, since most of the connections on social networks are between friends and family, and it would make sense that they would follow each other on Twitter. It has been found through experiment that when a Twitter user is followed by another user, there is a 44% chance that they will follow that other user back.

## Follow Through Retweets

Following via retweets is a follow method in which an agent sees a tweet that was retweeted by one of the agents they're following, and, liking the content of the tweet, decides to follow the original tweeter. The likelihood of this occurring is dependent on the value of the *follow_reaction_prob* variable in the *preference_classes* section of the input file. The *follow_reaction_prob* variable determines the probability that an agent, if they are selected to act on a retweet, will follow the tweeter of the original tweet as opposed to just retweeting the retweet. For example, if this value was set 0.8, everytime an agent was selected to act on a retweet, there would be an 80% chance that they would follow the original tweeter, and a 20% chance that they would just retweet the retweet.

## Constructing a Network Without These Other Follow Methods

We shall now create a network simulation that does not use these follow methods. We have created plenty of these already in the previous tutorials, so we will use the INFILE.yaml we used in **Tutorial 4** for the a twitter_suggest follow model that does not use the Barabasi Configuration as a starting point. This input file can be found [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial04_other/INFILE.yaml). Some slight changes will be made to this input file. The Ontario region preference class will be changed to the *StandardPref* to enable retweeting in the simulation, with the *StandardPref* preference class being changed as well. The tweet transmission for Standard agents will be changed to 0.1 for *plain* and *same_ideology* tweets and 0.2 for *humourous* tweets, while the *follow_reaction_prob* within the Standard preference class will be reduced to 0.0 to prevent following via retweets to occur. With all these changes in order, we can now run and visualize this network simulation.

```python
preference_classes:
 - name: StandardPref
 
  # Determines the probability that a tweet is reacted to by this 
  # preference class:
   tweet_transmission: 
      plain: # Also applies to musical tweets
        Standard: 0.1
        Celebrity: 0.01
        else: 0.01
      different_ideology:   # no retweeting for different ideologies
        Standard: 0.00
        Celebrity: 0.00
        else: 0.00
      same_ideology:
        Standard: 0.1
        Celebrity: 0.01
        else: 0.01
      humourous:
        Standard: 0.2
        Celebrity: 0.02
        else: 0.02
   # Probability that we will follow as a reaction.
   # Only applies to 2nd generation retweets, otherwise
   # the agent would already be a follower.
   follow_reaction_prob:
      0.0
```

## Running and Visualizing This Network Lacking These Other Follow Methods

Running this network simulation and visualizing it, we produced the following:

<p align='center'>
<img src='../img/tutorial09_without_other_follow/visualization.png'>
</p>

## Constructing a Network With These Other Follow Methods

We shall now implement the other follow methods into a network simulation. Using the INFILE.yaml we just created as a starting point, we will change the *follow_reaction_prob* to 0.2 to enable following via retweets, and have *use_followback* switched to *true* and the *Standard* agent type *followback_probability* to 0.44, to permit followback to occur in the network simulation at a rate that matches what was found through experiment. With these follow methods now permitted to occur, we shall run and visualize this network simulation and see what we find.

```python
follow_reaction_prob:
   0.2
```

```python
use_followback: 
  true        # followback turned on, from literature it makes sense for a realistic system
```

```python
followback_probability: 0.44
```

## Running and Visualizing This Network With These Other Follow Methods

Running this network simulation we produced the following visualization:

<p align='center'>
<img src='../img/tutorial09_with_other_follow/visualization.png'>
</p>

## Compare and Contrast

Looking at these two visualizations, we can draw several conclusions. We can see from the visualization of the network without the other follow methods that the network looks almost exactly the same as the one created in **Tutorial 4** for the Classic Barabasi configuration. This makes sense since the only major change made to the network was the implementation of retweets, which wouldn't affect how agents connect with one another in this simulation since following via retweets was disabled. The visualization for the network with the other follow methods, however, showed a must more deeply connected network. With agents free to not only just make that one *Barabasi connnection*, but other connections through followback and following via retweets as well, agents were able to make much more connections with one another, allowing for the creation of a much more realistic network.

## Next Steps

With the completion of this tutorial, you are now familiar with all of the possible manners in which agents can connect with one another in #k@. The followback and following via retweets method of following discussed in this tutorial are essential applications of hashkat, and their presence in network simulations are necessary for them to be authentic. We've enabled retweeting for the first time in this tutorial. Proceed to the next tutorial where we use these retweets that we are now accustomed to to discover the most popular tweet in a given network simulation.

