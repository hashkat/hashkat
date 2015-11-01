[hashkat.org](http://hashkat.org)

# Followback and Follow_Via_Retweet

In the previous tutorials we outlined the follow_models offered by **#k@**, however there are other modes of following outside of a follow_model. These other methods are 'followback' and 'follow_via_retweets'. In this tutorial, we will discuss 'followback' and 'follow_via_retweet' and compare a network simulation that does not involve these methods to a network simulation that does.

All the files that will be used in this simulation can be found for reference in [/hashkat/docs/tutorials/tutorial09/tutorial09_without_other_follow/](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial09_without_other_follow/INFILE.yaml) and [../tutorial09_with_other_follow/](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial09_with_other_follow/INFILE.yaml). This tutorial should take approximately 20 minutes to complete.

#### Followback

'Followback' occurs when an agent follows another agent because that agent has followed them.  It has been found through experiment that when a Twitter user is followed by another user there is a 44% chance that they will follow the other user back.  

Followback is be enabled by setting 'use_followback' to true.  Then an agent_type is configured with a 'followback_probability' between zero and one.

#### Follow Through Retweets

'Follow_via_retweets' is a method in which an agent sees a tweet that was retweeted by an agent they are following and, liking the content of the tweet, decides to follow the original tweeter.

<center>
<img src='../img/tutorial09_with_other_follow/following_via_retweets.png'>
</center>

'Follow_via_retweets' is enabled by setting 'use_follow_via_retweets' to 'true'. This will cause an agent who receives a retweet to follow the original tweeter (assuming they aren't already following them) based on preferential attachment.  The recipient is more likely to follow_via_retweet based on how popular the original tweeter is (based on culmulative-degree).

#### Constructing a Network Without These Other Follow Methods

We shall now create a network simulation that does not use these follow methods and then one that does.  For the network without followback or follow_via_retweets we start with the [Tutorial 4](http://docs.hashkat.org/en/latest/tutorial04/) input file for the twitter_suggest (preferential Global) follow model (without Barabasi). 

The 'Ontario' region preference class will be changed to the **StandardPref** which enables retweeting (called 'tweet_transmission'). The **StandardPref** preference class will also be changed.  The retweet rate ('tweet_transmission') for 'Standard' agents will be changed to 0.1 (10%) for 'plain' and 'same_ideology' tweets and 0.2 (20%) for 'humorous' tweets.  Although stated below, the 'tweet_transmission' rate does not matter for the 'Celebrity' or any other agent type (labelled 'else') since there were no agent_types other than 'Standard' in the Tutorial 4 simulation.

```python
preference_classes:
 - name: StandardPref

   tweet_transmission: #retweet transmission rate
      plain: # Also applies to musical tweets
        Standard: 0.1
        Celebrity: 0.01
        else: 0.01
      different_ideology: # generally no retweeting of tweets with different ideological content
        Standard: 0.00
        Celebrity: 0.00
        else: 0.00
      same_ideology:
        Standard: 0.1
        Celebrity: 0.01
        else: 0.01
      humorous:
        Standard: 0.2
        Celebrity: 0.02
        else: 0.02
```

#### Running and Visualizing This Network Without Other Follow Methods

We produced the following network with this configuration:

<center>
<img src='../img/tutorial09_without_other_follow/visualization.png'>
</center>

#### Constructing a Network With These Other Follow Methods

We shall now implement followback and follow_via_retweets. Using the [INFILE.yaml](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial09_without_other_follow/INFILE.yaml) we just created, we change 'use_followback' to 'true' and the 'Standard' agent_type's 'followback_probability' to 0.44. We will switch 'use_follow_via_retweets' to 'true'. With these follow methods now permitted we shall run this network simulation and see what we find.

```python
use_followback: 
  true        # from literature, it makes sense for a realistic system to have followback enabled
use_follow_via_retweets:
  true
```

#### Running and Visualizing This Network With These Other Follow Methods

Running this network simulation we produced the following:

<center>
<img src='../img/tutorial09_with_other_follow/visualization.png'>
</center>

#### Compare and Contrast

From the data ouput for the two simulations, it is clear that implementing followback and follow_via_retweets resulted in a substantial increase in the number of follows in the simulation. However looking at the visualizations it is hard to see much difference. This is due to most of the additional follows being 'followbacks'. The visualization does not show the dual direction of the edges between the nodes, although if one looks closely one can see arrowheads at both ends of the edge. 

In these network visualizations we've been creating, there are two different types of edges that can occur. They are a:

* Simple Directed Edge

<center>
<img src='../img/tutorial09_without_other_follow/one_follow_connection.png'>
</center>

* Bidirected Edge

<center>
<img src='../img/tutorial09_with_other_follow/followback_connection.png'>
</center>

A simple directed edge is when one node is following another node. A bidrected edge is when the two nodes are following each other. Though a bidrected edge is considered two edges in 'Gephi' (and **#k@**), they appear as one in the network visualizations. 

Implementing followback and follow_via_retweets in our simulation resulted in an increase in the total number of retweets.  Because more follows occur, tweets have a greater audience, resulting in a higher likelihood of retweet. A plot of the retweet distributions for both network simulations is shown below:

<center>
<img src='../img/tutorial09_with_other_follow/retweets_distro.svg'>
</center>

As we can see, most agents in the network without the other follow methods made fewer than 5 retweets , while in the network with followback and follow_via_retweet, agents with up to almost 30 retweets are present. More follows leads to much more retweets.

You can plot this graph in **Gnuplot** by going to the output directory and entering following commands:

`set boxwidth 0.75`

`set style fill solid 0.5`

`set xrange[0:]`

`set title 'Retweet Distribution'`

`set xlabel 'Number of Retweets'`

`set ylabel 'Normalized Probability of Agents with that Many Tweets'`

`plot 'tutorial09_without_other_follow/output/retweets_distro.dat' with boxes`
`title 'Without Other Follow',`
`'tutorial09_with_other_follow/output/retweets_distro.dat' with boxes title 'With Other Follow'`

## Next Steps

With the completion of this tutorial you are now familiar with all of the ways in which agents can connect with one another in **#k@**. The followback and follow_via_retweet methods discussed in this tutorial are essential applications of **#k@** and their presence in network simulations is necessary for simulations to be authentic.  

We enabled retweeting for the first time in this tutorial.  In the next tutorial we use retweeting to discover the [most popular tweet](http://docs.hashkat.org/en/latest/tutorial10/) in a given network simulation.
