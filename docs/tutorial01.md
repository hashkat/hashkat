[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

# Getting Started

## What is a Social Network?

A social network is an online system of connections made by users to interact
with one another. The purpose of #k@ is to simulate these social networks
through implementation of the Kinetic Monte Carlo Method.

## What is the Kinetic Monte Carlo (KMC) Method?



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

When running this program, you will see something similar to this
outputted to the screen:

Time | Users | Follows | Tweets | Retweets | Unfollows | R | Time (s)
--- | --- | --- | --- | --- | --- | --- | ---
9.04e+03 | 1.00e+04 | 9.00e+04 | 0.00e+00 | 0.00e+00(0.00e+00) | 0.00e+00 | 1.00e+01 | 4.86e-01

This gives a list of data at a particular point in your simulation.
**Time** corresponds to simulation time, **Users** corresponds to the number
of users or entities in the network at the point in the simulation, 
**Follows** show the number of follows that have occurred, **Tweets** displays
the number of tweets that have been made, **Retweets** shows the number of
retweets, **Unfollows** displays the number of unfollows, **R** is the
cumulative rate function, which is the sum of all the rates, the add rate,
tweet rate, follow rate, and retweet rate, in the system, and **Time (s)**
which is the length of real time that this part of the simulation has occurred.
