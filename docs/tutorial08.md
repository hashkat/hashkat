[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Twitter Follow Model

The twitter follow model is a combination of all of the other follow models, with each model respectively weighted. This potentially results in a network where some follows are random, some are based on which agent has the highest number of followers, some are based on what the agents's agent type is, some are based on the number of followers an agent of a particular agent type has, and some are based on the hashtags present in tweets. Undeniably the most complex of all the follow models, we will create a twitter follow model network simulation based on what we've learned in the previous tutorials. The files that we will use in this simulation can be found for reference in the tutorials directory in hashkat, with this one being under the title *tutorial08*. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/tutorials/tutorial08/INFILE.yaml). This tutorial should take approximately 15 minutes to complete.

## Constructing The Network

Using the INFILE.yaml file used in the previous tutorial as a starting point, we shall change the follow model to *twitter* for this simulation. Since the *twitter* follow model incorporates all the other follow models, we will have to fill out the model weights to control the proportion of each follow model that is present in the simulation relative to the others. We would like to have all the follow models weighted equally for this simulation. Therefore, every follow model will have a weight of 1.0 to accomplish this. Any value you use for the model weights is fine since they will all be normalized, but it is important that all the values are the same value to ensure that they are weighted equally. We would like to have *Celebrity* agents return to our simulation to make use of the agent and preferential agent follow models, so will change their add weight back to 100.0, to ensure that they are added into the network at an equal weight as the *Standard* agents.

## Running and Visualizing The Network

Running this simulation, we produced the following visualization:

![Visualization](/img/tutorial08/visualization.png =1x  "Visualization")

As we can see, this combination of all the follow models produces a very interesting network. Due to the presence of the agent and preferential agent follow models, we can see that *Celebrity* agents (red dots) are predominantly the more connected agents, though there are many highly connected *Standard* agents thanks to the random follow model leading to *Standard* agents being followed randomly and the twitter suggest follow model allowing *Standard* agents who randomly gained a lot of followers to gather even more followers fue to their high degree. Due to the presence of all the other follow models, it is difficult to notice the presence of the hashtag follow model since though agents may care on who they follow based on ideology for a few of their follows, they do not for many others, so there is no obvious separation between ideological groups as there is in the visualization generated for the hashtag follow model example in the previous tutorial. 

## Next Steps

With the completion of this tutorial, you have now familiarized yourslef with all of the follow models offered by #k@. You now have the fundamental knowledge necessary to create a variety of different network simulations using this program. Move onwards to the following tutorials, where we shall move on from the basics, and proceed to some of the more advanced aspects of #k@ which are necessary to be implemented into your network simulations to produce the more realistic data possible. 

