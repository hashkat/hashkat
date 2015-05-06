[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Getting Started

An introduction to the #k@ project, this tutorial will take approximately
30 minutes to complete.

## What is a Social Network?

A social network is an online system of connections made by users to interact
with one another. The purpose of #k@ is to simulate these social networks
through implementation of the Kinetic Monte Carlo Method.

## What is the Kinetic Monte Carlo (KMC) Method?

The Kinetic Monte Carlo (KMC) Method carries out a simulation where the rate
of which time is increased by is influenced by the cummulative rate function
of the rates within the system.

For the #k@ project, we have **R**, which is our cummulative rate function,
which is the sum of the user add rate, tweet rate, follow rate, retweet rate.  
It should be noted that the tweet rate, follow rate, and retweet rate are
all multiplied by the number of user in the network.
At a simulated time of 0, a random number is generated. Depending on this
number's value, it will either cause a user to be added into the network,
a random user to tweet, a random user to follow another random user, or a
random user to retweet a tweet from another user. **R** will then change
accordingly.  Another random number *u* will then be generated,
and time will move forward by:

&Delta;t = -ln(u) / **R**

All of the rate functions will then be recalibrated to their new values and
this cycle will repeat until either the maximum simulation time or real time
has been reached.

## Running a Simple Network

<iframe width="560" height="315" src="https://www.youtube.com/embed/-NQmVRzHX44" frameborder="0" allowfullscreen></iframe>

For this first tutorial, we're going to run a simple network using #k@.
If you look into the #k@ directory / folder, you will find several 
different files and directories. For now we are just going to look at
INFILE.yaml, which is the file we use to input how we would like to 
configure our simulated network.

In this file, you will see all the different configurations which you can
implement into your network. A detailed description of every single one
of these configurations can be found on the 'Input' page. For this tutorial,
it is only necessary to know that we have a constant number of entities 
or users in this network, 1000, and that the simulation will run for 100,000
simulated seconds and a maximum of 1 minute real time.

Run the simulation. If you experience any issues running the simulation,
you can reference the installation page for details on running a simulation
for the first time.

When running this program, you will see something similar to this
outputted to the screen:

Time | Users | Follows | Tweets | Retweets | Unfollows | R | Time (s)
--- | --- | --- | --- | --- | --- | --- | ---
9.93e+04 | 1.00e+03 | 1.00e+04 | 0.00e+00 | 0.00e+00(0.00e+00) | 0.00e+00 | 1.00e+01 | 2.23e+00

This gives a list of data at a particular point in your simulation.
**Time** corresponds to simulation time in seconds, **Users** corresponds to the number
of users or entities in the network at the point in the simulation, 
**Follows** show the number of follows that have occurred, **Tweets** displays
the number of tweets that have been made, **Retweets** shows the number of
retweets and in brackets beside it the number of active tweets (tweets that can be retweeted),
 **Unfollows** displays the number of unfollows, **R** is the
cumulative rate function, and **Time (s)**which is the length of real time
that this part of the simulation has occurred in seconds.

So as we can see from the above example, at a simulated time of 99,300 seconds,
there were 1,000 users in the network, 10,000 follows, 0 tweets, 0
retweets, and 0 unfollows had occurred, the cumulative rate function was 10,
and the real time that had elapsed was 2.23 seconds.

Once the simulation has concluded, the amount of time that the total analysis
took will be displayed on the screen in milliseconds, just below a chart
similar to this:

| | 0 | 1 | 2
--- | --- | --- | ---
**0** | 1.00e+02% | 0.00e+00% | 0.00e+00%
**1** | 0.00e+00% | 1.00e+02% | 0.00e+00% 
**2** | nan% | nan% | nan% 

Though this will be elaborated on in greater detail later on, what this
entails is the connections between regions. In the first row, we have the
different regions, regions 0, region 1, and region 2. In the second row,
we have the percentage of follows by users in region 0 that follow users in
other regions. As we can see from the above example, 100% of the follows
by users in region 0 follow users from region 0, 0% of those follows
follow users in region 1, and 0% of those follows follow users in region 2.
Similar conclusions can be made for the following rows. *nan* stands for not
a number. Since users from region 2 were not added into the simulation, there
is not a number for the percentage of their follows that follow users from
other regions, and 0.00% of the follows from any region are following them. 
The reasoning for why entities only follow other entities from their own region
can be found in INFILE.yaml. In the regions section of the file, you can see all the
the possible regions in this network. Since we have set users from region 0, Ontario,
to only speak English, and users from region 1, Quebec, to only speak Frence, it makes
sense that entitis from different regions won't connect with one another since they can't
understand each other. 

Looking into the hashkat directory, you will see a 'DATA_vs_TIME' file, which
contains the number of follows, tweets, the cumulative function, etc for
various simulation times as discussed above. You will also see a
'network_state.sav' file which is where your simulation data will be saved to.
There is also the 'output' directory, which contains all the data you've
accummulated through simulation necessary for analysis. We shall discuss
these files below.

## Output of a Simple Network

Going into the output directory and viewing its contents by inputting the commands

`cd output`

`ls`

one can see all the information collected from this simple network simulation. Let's
look at some of this data.
Going into our *main_stats.dat* file by typing in the command

`nano main_stats.dat`

we can the see the primary data compiled from our simulation. We can also use gnuplot to
plot the in-degree, out-degree, and cumulative-degree distributions for each simulated months
for all the entities. An entity's in-degree is the number of entities following them, while its
out-degree is the number of entities its following, and the cumulative-degree is the sum of these
two values. Let's plot!

To access gnuplot, type in the command:

`gnuplot`

To plot the in-degree distributions for *month002*, which contains the most up to date data, with appropriate axis labels and a title,
type in the following:

`set title 'In-Degree Distribution'`

`set xlabel 'k'`

`set ylabel 'P(k)'`

`plot 'in-degree_distribution_month_002.dat' title ''`

Giving us:

![In Degree Distribution](/img/tutorial01/in-degree_distribution_month_002.svg =1x  "In Degree Distribution")

where *k* is the number of degrees or followers an entity has, and *P(k)* is the probability that an entity would have that many
degrees/followers.

Repeating these steps in a similar fashion for the ou-degree an d cumulative-degree distributions gives us:

![Out Degree Distribution](/img/tutorial01/out-degree_distribution_month_002.svg =1x  "Out Degree Distribution")

![Cumulative Degree Distribution](/img/tutorial01/cumulative-degree_distribution_month_002.svg =1x  "Cumulative Degree Distribution")

Note that you can save your plots as .svg files by typing in the commands

`set term svg`

when you first start up gnuplot and prior to actually plot the data.
