[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Twitter Preferential Follow Model

This tutorial should take approximately 90 minutes to complete

Based on Albert-Laszlo Barabasi's research, a twitter preferential follow model is one in which entities tend to follow users with the highest degree, aka highest number of followers.
The twitter preferential method is influenced heavily by the [Barabasi-Albert Model](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model), generating networks
where entities with the greater number of followers have a higher probability of being followed.
This particular follow model can be implemented using three different types of configurations of this method, each of which is outlined below.

## Example - Classic Barabasi

The Classic Barabasi configuration is one in which entities that are added to the network make one conection with another entity and no other, unless manipulated to do so by allowing followback or
following through retweets within your simulation.

Let's try running a Classic Barabasi twitter preferential follow model simulation, with our starting point being the INFILE.yaml we used in the previous tutorial.
As always, all the files that we will use in this simulation can be found for reference in the examples directory in hashkat, with this one under the title *tutorial05_classic_barabasi*.
You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/examples/tutorial05_classic_barabasi/INFILE.yaml).

So let's make some modifications to our input file. As opposed to the random follow model simulations, where the number of entities within the network remained constant throughout

## Example - Non-Classic Barabasi



## Example - Other Twitter Preferential Models



## Next Steps
