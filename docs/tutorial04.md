[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Twitter Preferential Follow Model

Going through several exercises on the different configurations of the twitter preferential follow model, this tutorial should take approximately 1 hour to complete

Based on Albert-Laszlo Barabasi's research, a twitter preferential follow model is one in which entities tend to follow users with the highest degree, aka highest number of followers.
The twitter preferential method is influenced heavily by the [Barabasi-Albert Model](http://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model), generating networks
where entities with the greater number of followers have a higher probability of being followed.
This particular follow model can be implemented using three different types of configurations of this method, each of which is outlined below.

## Example - Classic Barabasi

The Classic Barabasi configuration is one in which entities that are added to the network make one conection with another entity and no other, unless manipulated to do so by allowing follow back or
following through retweets within your simulation.

Let's try running a Classic Barabasi twitter preferential follow model simulation, with our starting point being the INFILE.yaml we used in the previous tutorial.
As always, all the files that we will use in this simulation can be found for reference in the examples directory in hashkat, with this one under the title *tutorial05_classic_barabasi*.
You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/examples/tutorial05_classic_barabasi/INFILE.yaml).

So let's make some modifications to our input file. As opposed to the random follow model simulations, where the number of entities within the network remained constant throughout the simulation,
we are going to have the number of entiteies within the simulation increase over time, by setting our *initial_entities* to 10, and our *max_entities* to 3000. We're also going to have the simulation
run for a little longer by setting  the *max_time* to 10,000. Most importantly, we're going to set *use_barabasi* to *true* causing the simulation to implement the
Barabasi configuration. *barabasi_connections* specifies the number of connections an entity makes when entering the simulation, so for this Classic Barabasi example, we're going to set this value to 1.
Since we are running a twitter preferential follow model, we are going to set the *follow_model* as such. In the *rates* section of the input file, we are now adding entities to the network throughout the simulation,
so we will change the add rate value to 1.0, so that one entity will be added to the network per simulated minute.

As mentioned in the previous tutorial, the follow ranks are essential to the twitter preferential follow model. Entities are placed into bins based on their degree or the number of followers they have.
All the entities in the bin with threshold 0 have 0 followers, while all the entities in bin 200 have 200 followers.The weight of each bin threshold determines the probability that an entity from this bin
will be randomly chosen to be followed in comparison to entities in other bins. The bin thresholds are linearly spacecd in increments of 1 and have a minimum value of 0 and a maximum value equal to the maximum
number of entities within the simulation. The weights of these bins are also linearly spaced in increments of 1, and the minimum bin threshold has a weight of 1 and the maximum bin threshold has a weight
equal to the max number of entities plus one. So the weighted probability that an entity with 10 followers will be chosen in comparison with an entity with 100 followers is 11 to 101 respectively. Therefore,
entities with a greater number of followers have a better chance of being followed by other entities. It is very important that the max follow rank threshold be equal to or greater than the number of max entities
within the simulation. If this number is less than that, your network simulation may give inaccurate results and the total number of followers the most popular entity has may be impossible to determine. Since the
minimum threshold follow rank has a weight of 1, the maximum threshold follow rank musht have a weight equal to its value increased by one. 

To better demonstrate the results of a twitter preferential follow model, we are only going to use
one entity type for the simulation. Setting the add weights of the *Celebrity*, *Bot*, and *Organization* entity types will remove their presence from the network. Though it doesn't really matter, we have also
changed the *Standard* entity type's add weight to 100, to show that 100% of the entity will be of that type, though as long as this value is greater than zero it is inconsequential since all the other
entity types have an add weight of 0. We have also set the *Standard* entity type's follow rate to 0.0, so that the only manner in which entities are connecting with each other is through the *barabasi_connections*
they are assigned to make and any follow backs or following through retweets that occur.

Let's now run this simulation. You can plot the log-log graph of the *cumulative-degree_distribution_month_000.dat* in gnuplot, by following the same plotting steps outlined in Tutorial 1 but when plotting tying in
the command:

`plot 'cumulative-degree_distribution_month_000.dat' u 3:4 title ''`

This gave us the following graph:

![Cumulative Degree Distribution](/img/tutorial05_classic_barabasi/cumulative-degree_distribution_month_000.svg =1x  "Cumulative Degree Distribution")

We can see that this produces a linear plot with a negative slope, which makes sense since we would expect entities with a small degree to be great number, while entities with a very large degree to
be very small in number.

A visualization of this network is shown below:

![Visualization](/img/tutorial05_classic_barabasi/visualization.png =1x  "Visualization")

As we can see, we have the much more highly connected entities at the centre of the visualization, and the entities lower in degree on the sides. Looking closely in the visualization, we can see that some entities
are not connected to anyone. How could this be? Every entity was supposed to make at least one connection in the simulation through implementation of the Clasic Barabasi configuration. The reason for this was that
though every entity definitely attempted to make at least one connection, the entity they may have chosen to follow may have happened to speak a different language than them, resulting in them being unable to connect.
Though easy to forget, factors such as this are integral to the shape of your network simulation, and must always be considered.

## Example - Non-Classic Barabasi

The Non-Classic Barabasi configuration is exactly the same as the classic configuration except that the number of connections entities make when entering the simulation is a number greater than 1.

For this example, we will use the exact same INFILE.yaml as used above, but our *barabasi_connections* will have a value of 2 instead of 1. This simulation will also take longer to complete than the previous case,
so we shall set our *max_real_time* to a day to ensure that our simulation runs for the full length of simulated time. Though the simulation will take nowhere close to this long, it is good practice to
have your *max_real_time* always set to a high number, assuming that you have access to the computer running this simulation for that full length of time of course, to prevent your simulations from ever
cutting short of their allotted simulation run time. The network simulations that will be run in these tutorials will have a *max_real_time* of one day from now on. 
The files used in this simulation can be found for reference in the examples directory in hashkat under the title *tutorial05_nonclassic_barabasi*.
You can also view the input file used for this example [here](https://github.com/hashkat/hashkat/blob/master/examples/tutorial05_nonclassic_barabasi/INFILE.yaml).

Running this network simulation, we produced the visualization shown below:



## Example - Other Twitter Preferential Models

We shall now run a twitter preferential follow model network simulation with out implementing the Barabasi configuration. The INFILE.yaml file that we will use in this simulation will be the
exact same as the one used in the Classic Barabasi example, except that *use_barabasi* will be turned off of course, and the *Standard* entity types will now have a follow rate of 0.001. As explained above,
the max_real time for this simulation will also be one day.
The files that were used in this example can be found for reference in the examples directory in hashkat under the title *tutorial05_other*.
You can also view the input file used for this example [here](https://github.com/hashkat/hashkat/blob/master/examples/tutorial05_other/INFILE.yaml).

## Next Steps

We have now worked with a few configurations of the twitter preferential follow method. Though we did not implement entity types with a follow rate when runing Barabasi configurations,
you are encouraged to try doing so, as well running simulations with more entity types, more entities, less regions, etc. The more practice you get using #k@, the better skilled you will be at
producing your ideal network simulations.

Proceed to the next tutorial, where we try using the entity follow model.
