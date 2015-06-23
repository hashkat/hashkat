[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Other Methods of Following

In the previous tutorials, we have outlined the various follow models offered by hashkat. When implemented, each one produced a vastly different network, having a huge impact on the connections made by agents. However, when implementing a particular follow model, there are other factors that could be applied to the system to cause agents to follow others outside the framework of that simulation's follow model. These other factors that can be utilized in a network simulation are 'followback' and 'following via retweets'. In this tutorial, we will outline and discuss what these application mean, and compare a network simulation that does not involve these follow methods to a network simulation that does implement them.

All the files that will be used in this simulation can be found for reference in the tutorials directory in hashkat, with the ones used for this tutorial under the titles *tutorial09_without_other_follow* and *tutorial09_with_other_follow* respectively. You can view the input file we will be creating for the network simulation that does not involve the other follow methods [here](), and the network simulation that does involve the other follow methods [here](). This tutorial should take approximately 30 minutes to complete.

## Followback

'Followback' is when an agent follows another agent who followed them. Obviously, this implementation makes for a more realistic system, since most of the connections on social networks are between friends and family, and it would make sense that they would follow each other on Twitter. It has been found through experiment that when a Twitter user is followed by another user, there is a 44% chance that they will follow that other user back.

## Follow Through Retweets

Following via retweets is a follow method in which an agent sees a tweet that was retweeted by one of the agents they're following, and, liking the content of the tweet, decides to follow the original tweeter. The likelihood of this occurring is dependent on the value of the *follow_reaction_prob* variable in the *preference_classes* section of the input file, where it can be a value anywhere from 

## Constructing a Network Without These Other Follow Methods

We shall now create a network simulation that does not use these follow methods. We have created plenty of these already in the previous tutorials, so we will use the INFILE.yaml we used in **Tutorial 4** for the Classic Barabasi Configuration as a starting point. The input file we created for this can be found [here](https://github.com/hashkat/hashkat/blob/master/tutorials/tutorial04_classic_barabasi/INFILE.yaml). Some slight changes will be made to this input file. The Ontario region preference class will be changed to the *StandardPref* to enable retweeting in the simulation, with the *StandardPref* preference class being changed as well. The tweet transmission for Standard agents will be changed to 0.1 for *plain* and *same_ideology* tweets and 0.2 for *humourous* tweets, while the *follow_reaction_prob* within the Standard preference class will be reduced to 0.0 to prevent following via retweets to occur. With all these changes in order, we can now run and visualize this network simulation.

## Running and Visualizing This Network Lacking These Other Follow Methods

Running this network simulation and visualizing it, we produced the following:

![Visualization](/img/tutorial09_without_other_follow/visualization.png =1x  "Visualization")



## Constructing a Network With These Other Follow Methods

We shall now implement the other follow methods into a network simulation. Using the INFILE.yaml we just created as a starting point, we will change the *follow_reaction_prob* back to 0.2 to enable following via retweets, and have *use_followback* switched to *true* and the *Standard* agent type *followback_probability* to 0.44, to permit followback to occur in the network simulation at a rate that matches what was found through experiment. With these follow methods now permitted to occur, we shall run and visualize this network simulation and see what we find.

## Running and Visualizing This Network With These Other Follow Methods

Running this network simulation we produced the following visualization:

![Visualization](/img/tutorial09_with_other_follow/visualization.png =1x  "Visualization")



## Compare and Contrast



## Next Steps

With the completion of this tutorial, you are now familiar with all of the possible manners in which agents can connect with one another in #k@. We've also enabled retweeting for the first time in this tutorial. Proceed to the next tutorial where we use these retweets that we are now accustomed to to discover the most popular tweet in a given network simulation.

