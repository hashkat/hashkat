[hashkat.org](http://hashkat.org)

# The Twitter Follow Model

The *twitter* follow model implements all of the other follow models into a network simulation, with each model respectively weighted. This potentially results in a network where some follows are random, some are based on which agent has the highest number of followers, some are based on what the agents's agent type is, some are based on the number of followers an agent of a particular agent type has, and some are based on the hashtags present in tweets. Undeniably the most complex of all the follow models, we will create a *twitter* follow model network simulation based on what we've learned in the previous tutorials. The input file that we will create in this tutorial can be found for reference in the **docs/tutorial_input_files** directory in **hashkat**, with this one being under the title **tutorial08**. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial08/INFILE.yaml). This tutorial should take approximately 15 minutes to complete.

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/QthCBz_ejAE" frameborder="0" allowfullscreen></iframe>
</center>

## Constructing The Network

Using the *INFILE.yaml* file created in the previous [tutorial](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial07/INFILE.yaml) as a starting point, we shall change the follow model to *twitter* for this simulation. Since the *twitter* follow model incorporates all the other follow models, we will have to fill out the model weights to control the proportion of each follow model that is present in the simulation relative to the others. We would like to have all the follow models weighted equally for this simulation, therefore, every follow model will have a weight of 1.0 to accomplish this. Any value you use for the model weights is fine since they will all be normalized, but it is important in this example that all the values are the same value to ensure that they are weighted equally. We would also like to have *Celebrity* agents return to our network to make use of the *agent* and *preferential_agent* follow models, so we will change their add weight back to 100.0, to ensure that they are added into the network at an equal weight as the *Standard* agents.

```python
follow_model: # See notes above
  twitter
# model weights ONLY necessary for follow method 'twitter'  
model_weights: {random: 1.0, twitter_suggest: 1.0, agent: 1.0, preferential_agent: 1.0, hashtag: 1.0}
```

## Running and Visualizing The Network

Running this simulation, we produced the following visualization:

<center>
<img src='../img/tutorial08/visualization.png'>
</center>

As we can see, this combination of all the follow models produces a very interesting network. Due to the presence of the *agent* and *preferential_agent* follow models, we can see that *Celebrity* agents (red nodes) are predominantly the more connected agents, though there are many highly connected *Standard* agents as well. This is most likely due to the *random* follow model leading to *Standard* agents being followed randomly and the *twitter_suggest* follow model allowing *Standard* agents who randomly gained a lot of followers to gather even more followers due to their high degree. Due to the presence of all the other follow models, it is difficult to notice the presence of the *hashtag* follow model since though agents may care on who they follow based on ideology for a few of their follows, they do not for many others, so there is no obvious separation between ideological groups as there was in the visualization generated for the *hashtag* follow model example in the previous [tutorial](http://docs.hashkat.org/en/latest/tutorial07/). 

Looking into our *main_stats.dat* output file, we can see that every follow model was used in this network simulation as shown below:

<center>
<img src='../img/tutorial08/main_stats.png'>
</center>

A comparison of the weights in which these follow models were present in this network can be seen in the pie graph constructed below:

<center>
<img src='../img/tutorial08/follow_model_graph.png'>
</center>

(**Note**: this pie graph was constructed using [meta-chart.com/pie](https://www.meta-chart.com/pie))

## Next Steps

With the completion of this, and the previous tutorials, you have now familiarized yourself with all of the follow models offered by ***#k@***. You now have the fundamental knowledge necessary to create a variety of different network simulations using this program. Move onwards to the following tutorials, where we shall move on from the basics, and proceed to some of the more advanced aspects of ***#k@*** which need to be implemented into your network simulations to produce the most realistic data possible. 