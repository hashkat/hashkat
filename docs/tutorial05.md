[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Agent Follow Model

This tutorial should take approximately 20 minutes to complete

An agent-based follow method is one in which agents follow a twitter user based on their agent class, though who they follow within that agent class is totally random.

Let's run an agent follow model using #k@. All the files that we will use in this simulation can be found for reference in the tutorials directory in hashkat, with this one under the title *tutorial05*. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/tutorials/tutorial05_classic_barabasi/INFILE.yaml).

Starting with the INFILE.yaml file we used in **Tutorial 4** for **Other Twitter Suggest Models**, we will of course change the follow model from *twitter_suggest* to *agent*, and finally get to implement our *Celebrity* agents into our network. This can be done by simply changing the *Celebrity* add weight to 100.0. With both the *Standard* agents and *Celebrity* agents having an add weight of 100.0, the number of *Standard* agents and *Celebrity* agents within the simulation will be roughly the same, with the proportion of *Standard* agents and *Celebrity* agents being added to the network being 1:1.



