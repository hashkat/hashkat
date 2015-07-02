[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Unfollowing

Unfollowing is the act in which an agent stops following another agent due to a variety of reasons. In this tutorial, we are going to discuss the different methods in which unfollowing can be implemented into your network simulation.

## Unfollowing Due to Chattiness

One of the many factors that influence an agent's decision to unfollow another agent is how talkative they are. If an agent is constantly tweeting at an excessive rate, their followers may find that this chattiness may make it difficult for them to view the tweets of other agents they are following, and they may unfollow this chatty agent because of it. There are currently two implementations of unfollowing due to chattiness found in #k@, *stage1_unfollow* and *unfollow_tweet_rate* which are discussed below.

#### Stage 1 Unfollow

If *stage1_unfollow* is implemented into a network configuration, whenever an agent follows another agent, the tweet rate of this newly followed agent will be compared to that of the other agents that the agent was following. If this newly followed agent's tweet rate is greater than double the average tweet rate of all the other agents that the original agent was following, then this newly followed agent will be put into an array, where it may be randomly chosen to be unfollowed when the unfollow function is called.

#### Unfollow Tweet Rate

The *unfollow_tweet_rate* is a number inputted into the **INFILE.yaml** file that causes an agent who has created more tweets than this value to be randomly unfollowed by one of his/her followers.

## Next Steps



