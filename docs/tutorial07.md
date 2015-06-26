[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Hashtag Follow Model

The hashtag follow method is one in which an agent follows another agent through a hashtag that matches their hashtag preferences. In order for this follow method to be possible, *use_hashtag_probabilty* must be greater than zero, or else there will be no hashtags present in the simulation. Let's run a hashtag follow model simulation. The files that we will use in this simulation can be found for reference in the tutorials directory in hashkat, with this one under the title *tutorial07*. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/tutorials/tutorial07/INFILE.yaml). This tutorial should take approximately 15 minutes to complete.

## Constructing The Network

Using the INFILE.yaml we used from **Tutorial 6**, as a starting point, we will of course change the follow model to *hashtag*. We will let the *use_hashtag_probability* remain at 0.5 or 50%, so that 50% of the tweets will contain the hashtags necessary for following in the hashtag follow model. Under the agent types section, there are *hashtag_follow_options*, where you can decide if agents of that particular agent type will only follow other agents living in the same region as them and/or that share the same ideology as them through hashtags present in tweets by inputting *true* for *care_about_region* and/or *care_about_ideology* respectively. For this simulation, we will only have agents follow other agents that share the same ideology as them, so input *true* for *care_about_ideology* and have *care_about_region* remain *false*. For simplicity, we will also remove any *Celebrity* agents from this network simulation by changing the *Celebrity* add rate to 0.0.

## Running and Visualizing The Network

Creating a network with this configuration, we produced the following visualization:

![Visualization](/img/tutorial07/visualization.png "Visualization")

As you can see from this visualization, agents are separated into 4 main groups, with several other agents having no connections, being unable to follow or be followed by another agent in time. These 4 groups correspond to the 4 different ideologies we have in our network, named *Red*, *Blue*, *Green*, and *Orange*. It is important to note that the colours of agents in the visualization do not correspond to ideolgy colours, but are arbitrarily given to differentiate enities of different types (e.g. *Standard* agents, *Celebrity* agents, etc.).

<iframe width="420" height="315" src="https://www.youtube.com/embed/50nUnyawKNU" frameborder="0" allowfullscreen></iframe>

## Next Steps

You have now gained experience with the hashtag follow model. You are encouraged to try experimenting with different simulations of this model. Implementing more regions and ideologies into a network using the follow model is bound to lead to some fascinating results. When you feel you are ready, move on to the next tutorial, where we will discuss the final follow model, the twitter follow model, which incorporates all of the other follow models into a network simulation.


