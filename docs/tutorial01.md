[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

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
random user to retweet a tweet from another user.**R** will then change
accordingly.  Another random number *x* will then be generated,
and time will move forward by:

&Delta;t = ln(1/x) / **R**

All of the rate functions will then be recalibrated to their new values and
this cycle will repeat until either the maximum simulation time or real time
has been reached.

##Running a Simple Network

<iframe width="560" height="315" src="https://www.youtube.com/embed/-NQmVRzHX44" frameborder="0" allowfullscreen></iframe>

For this first tutorial, we're going to run a simple network using #k@.
If you look into the #k@ directory / folder, you will find several 
different files and directories. For now we are just going to look at
INFILE.yaml, which is the file we use to input how we would like to 
configure our simulated network.

In this file, you will see all the different configurations whih you can
implement into your network. A detailed description of every single one
of these configurations can be found on the 'Input' page. For this tutorial,
it is only necessary to know that we have a constant number of entities 
or users in this network, 10,000, and that the simulation will run for 10,000
simulated minutes and a maximum of 1 minute real time.

To run this simulation, . You can press 'ctrl-c' at any time to stop the
simulation and save your data. Running the simulation again will result in
it continuing from the point where it halted.

When running this program, you will see something similar to this
outputted to the screen:

Time | Users | Follows | Tweets | Retweets | Unfollows | R | Time (s)
--- | --- | --- | --- | --- | --- | --- | ---
9.04e+03 | 1.00e+04 | 9.00e+04 | 0.00e+00 | 0.00e+00(0.00e+00) | 0.00e+00 | 1.00e+01 | 4.86e-01

This gives a list of data at a particular point in your simulation.
**Time** corresponds to simulation timein minutes, **Users** corresponds to the number
of users or entities in the network at the point in the simulation, 
**Follows** show the number of follows that have occurred, **Tweets** displays
the number of tweets that have been made, **Retweets** shows the number of
retweets, **Unfollows** displays the number of unfollows, **R** is the
cumulative rate function, and **Time (s)**which is the length of real time
that this part of the simulation has occurred.

So as we can see from the above example, at a simulated time of 9,040 minutes,
there were 10,000 users in the network, 90,000 follows, 0 tweets, 0
retweets, and 0 unfollows had occurred, the cumulative rate function was 10,
and the real time that had elapsed was 0.486 seconds.

Once the simulation has concluded, the amount of time that the total analysis
took will be displayed on the screen in milliseconds, just below a chart
similar to this:

| | 0 | 1 | 2
--- | --- | --- | ---
**0** | 5.01e+01% | 4.99e+01% | 0.00e+00%
**1** | 4.99e+01% | 5.01e+01% | 0.00e+00% 
**2** | nan% | nan% | nan% 

Though this will be elaborated on in greater detail later on, what this
entails is the connections between regions. In the first row, we have the
different regions, regions 0, region 1, and region 2. In the second row,
we have the percentage of follows by users in region 0 that follow users in
other regions. As we can see from the above example, 50.1% of the follows
by users in region 0 follow users from region 0, 49.9% of those follows
follow users in region 1, and 0.00% of those follows follow users in region 2.
Similar conclusions can be made for the following rows. *nan* stands for not
a number. Since users from region 2 were not added into the simulation, there
is not a number for the percentage of their follows that follow users from
other regions, and 0.00% of the follows from any region are following them. 

Looking into the hashkat directory, you will see a 'DATA_vs_TIME' file, which
contains the number of follows, tweets, the cumulative function, etc for
various simulation times as discussed above. You will also see a
'network_state.sav' file which is where your simulation data will be saved to.
There is also the 'output' directory, which contains all the data you've
accummulated through simulation necessary for analysis. We shall discuss
these files below.

##Output of a Simple Network
