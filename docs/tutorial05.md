[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Agent Follow Model

An agent-based follow method is one in which agents follow a twitter user based on their agent class, though who they follow within that agent class is totally random. Let's run an agent follow model using #k@. All the files that we will use in this simulation can be found for reference in the tutorials directory in hashkat, with this one under the title *tutorial05*. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorials/tutorial05/INFILE.yaml). This tutorial should take approximately 20 minutes to complete.

## Constructing The Network

Starting with the INFILE.yaml file we used in **Tutorial 4** for **Other Twitter Suggest Models**, we will of course change the follow model from *twitter_suggest* to *agent*, and finally get to implement our *Celebrity* agents into our network. This can be done by simply changing the *Celebrity* add weight to 100.0. With both the *Standard* agents and *Celebrity* agents having an add weight of 100.0, the number of *Standard* agents and *Celebrity* agents within the simulation will be roughly the same, with the proportion of *Standard* agents and *Celebrity* agents being added to the network being 1:1. Since celebrities tend to garner vastly more followers than ordinary people, it makes sense for our simulation of *Standard* and *Celebrity* agents to mimic this. Therefore, we will change the *Standard* follow weight to 10.0 and the *Celebrity* follow weight to 90.0. From this we should expect to see *Celebrity* agents to garner about 90% of the follows and Standard agents to garner about 10%.

<p align='center'>
<img src='../img/tutorial05/INFILE-agents.png'>
</p>

<p align = 'center'>
<iframe width="560" height="315" src="https://www.youtube.com/embed/_SXyLB0O30s" frameborder="0" allowfullscreen></iframe>
</p>

## Running and Visualizing The Network

Running this simulation we produced the following network:

<p align='center'>
<img src='../img/tutorial05/visualization.png'>
</p>

As expected, the *Celebrity* agents (red dots) tend to have much more followers than the *Standard* agents (blue dots).

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/-bYzbWOc8yc" frameborder="0" allowfullscreen></iframe>
</p>

## Next Steps

Continue experimenting with the agent follow model. Adding more agent types into your simulation, such as bots and organizations, and manipulating all of their add weights, follow weights, follow rates, etc., can lead to some truly complex and interesting networks. When you are comfortable using the agent follow model, proceed to the next tutorial, where we actually combine the twitter suggest follow model and agent follow model to create the preferential agent follow model.





