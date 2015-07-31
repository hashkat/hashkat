[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Getting Started

An introduction to the #k@ project, this tutorial will take approximately
40 minutes to complete.

## What is a Social Network?

A social network is an online system of connections made by users to interact
with one another. The purpose of #k@ is to simulate these social networks
through implementation of the Kinetic Monte Carlo Method.

## What is the Kinetic Monte Carlo (KMC) Method?

The Kinetic Monte Carlo (KMC) Method carries out a simulation where the rate
of which time is increased by is influenced by the cummulative rate function
of the rates within the system.

For the #k@ project, we have **R**, which is our cummulative rate function,
which is the sum of the agent add rate, tweet rate, follow rate, and retweet rate.  
It should be noted that the tweet rate, follow rate, and retweet rate are
all multiplied by the number of agents in the network.
At a simulated time of 0, a random number, *r*<sub>1</sub>, is generated. Depending on this number's value, it will either cause an agent to be added into the network,
a random agent to tweet, a random agent to follow another random agent, or a
random agent to retweet a tweet from another agent. **R** will then change
accordingly. If *use_random_time_increment* is enabled. another random number *r*<sub>2</sub> will then be generated, and time will move forward by:

<p align = 'center'>
&Delta;<i>t</i> = -ln(<i>r</i><sub>2</sub>) / <b>R</b>
</p>

If this is not enabled, time will move forward by:

<p align = 'center'>
&Delta;<i>t</i> = 1 / <b>R</b>
</p>

All of the rate functions will then be recalibrated to their new values and
this cycle will repeat until either the maximum simulation time or real time
has been reached.

The following is a model of how this works in #k@. We initially have one agent in the network, with the agent add rate having been set to 1 agent per simulated minute and the agent tweet rate being set to 1 tweet per simulated minute (we've ignored the follow rate and retweet rate for this example). At this moment, there is an even chance that another agent will be added to the network or the initial agent will make a tweet. A random number is generated. If the random number is less than 0.5, another agent will be added, if it is greater or equal to 0.5, the initial agent will tweet. Time will then move forward based on the cumulative rate function and possibly another generated random number, again depending on whether or not *use_random_time_increment* is permitted.

<p align='center'>
<img src='../img/tutorial01/kmc_1.png'>
</p>

Let's say that an agent has been added to the network.

<p align='center'>
<img src='../img/tutorial01/kmc_2.png'>
</p>

With this addition to our network, the agent add rate will remain the same but the agent tweet rate will now double since we have double the number of agents from what we originally had in the network. With this change, an agent tweeting is twice as likely to happen in comparison to an agent being added in the KMC loop, though the expected rates for each agent will remain the same since the simulated time will move more slowly, since the change in time is inversely proportional to the steadily increasing cummulative rate function.

<p align='center'>
<img src='../img/tutorial01/kmc_3.png'>
</p>

Let's add an additional agent to the network.

<p align='center'>
<img src='../img/tutorial01/kmc_4.png'>
</p>

Just as before, the agent add rate will remain the same but the agent tweet rate will increase since we now have 3 times the number of agents we initially had, though the expected rates for each agent will remain the same.

<p align='center'>
<img src='../img/tutorial01/kmc_5.png'>
</p>

This cycle will continue until the end of the simulation. For more information on how this is run in #k@, please visit the **Developers** page.

## Running a Simple Network

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/i921pJBAPmM" frameborder="0" allowfullscreen></iframe>
</p>

(**Note**: The videos used in this documentation were created using a previous version of **#k@**. There are slight differences made between some of the commands issued and the files used in this video. The documentation itself is the most updated version of **#k@**.)

For this first tutorial, we're going to run a simple network using #k@.
If you look into the #k@ directory / folder, you will find several 
different files and directories. For now we are just going to look at
INFILE.yaml, which is the file we use to input how we would like to 
configure our simulated network.

In this file, you will see all the different configurations which you can
implement into your network. A detailed description of every single one
of these configurations can be found on the 'Input' page. For this tutorial,
it is only necessary to know that we have a constant number of agents 
or users in this network, 1000, and that the simulation will run for 100,000
simulated minutes and a maximum of 1 minute real time.

Each agent will have their own particular characteristics that will be changed and manipulated in the following tutorials. A summary of these characteristics is outlined below:

<p align='center'>
<img src='../img/tutorial01/agent_info.png'>
</p>

Run the simulation. If you experience any issues running the simulation,
you can reference the installation page for details on running a simulation
for the first time.

When running this program, you will see something similar to this
outputted to the screen:

| Time | Users | Follows | Tweets | Retweets | Unfollows | R | Time (s) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 9.93e+04 | 1.00e+03 | 1.00e+04 | 0.00e+00 | 0.00e+00(0.00e+00) | 0.00e+00 | 1.00e-01 | 2.25e+00 |

This gives a list of data at a particular point in your simulation.
**Time** corresponds to simulation time in minutes, **Users** corresponds to the number
of agents in the network at the point in the simulation, 
**Follows** show the number of follows that have occurred, **Tweets** displays
the number of tweets that have been made, **Retweets** shows the number of
retweets and in brackets beside it the number of active tweets (tweets that can be retweeted),
 **Unfollows** displays the number of unfollows, **R** is the
cumulative rate function, and **Time (s)**which is the length of real time
that this part of the simulation has occurred in seconds.

So as we can see from the above example, at a simulated time of 99,300 minutes,
there were 1,000 agents in the network, 10,000 follows, 0 tweets, 0
retweets, and 0 unfollows had occurred, the cumulative rate function was 0.1,
and the real time that had elapsed was 2.25 seconds.

Once the simulation has concluded, the amount of time that the total analysis
took will be displayed on the screen in milliseconds. 
Looking into the hashkat directory, you will see a 'DATA_vs_TIME' file, which
contains the number of follows, tweets, the cumulative function, etc for
various simulation times as discussed above. You will also see a
'network_state.dat' file which is where your simulation data will be saved to.
There is also the 'output' directory, which contains all the data you've
accummulated through simulation necessary for analysis. We shall discuss
these files below.


## Output of a Simple Network

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/cGzzACPLu8c" frameborder="0" allowfullscreen></iframe>
</p>

Going into the output directory and viewing its contents by inputting the commands

`cd output`

`ls`

one can see all the information collected from this simple network simulation. Let's
look at some of this data.
Going into our *main_stats.dat* file by typing in the command

`nano main_stats.dat`

we can the see the primary data compiled from our simulation shown below. 

<p align='center'>
<img src='../img/tutorial01/main_stats.png'>
</p>

We can also use gnuplot to plot the in-degree, out-degree, and cumulative-degree distributions for each simulated months for all the agents. An agent's in-degree is the number of agents following them, while its
out-degree is the number of agents its following, and the cumulative-degree is the sum of these two values. The agents in this simulation were configured to  have a follow rate of 0.0001, which basically means that in a simulation 100,000 simulated minutes long, each agent will follow around 10 users, and be followed by about 10 users. So we can therefore expect most agents to have an in-degree magnitude of 10, an out-degree magnitude of 10, and a cumlative-degree magnitude of 20. Let's plot and see!

To access gnuplot, type in the command:

`gnuplot`

To plot the in-degree, out-degree, and cumulative-degree distributions for *month002*, which contains the most up to date data, on the same graph with appropriate axis labels and a title, type in the following:

`set style fill transparent solid 0.5 noborder `

`set title 'Agent Degree Distributions'`

`set xlabel 'Degree'`

`set ylabel 'Degree Probability'`

`plot 'in-degree_distribution_month_002.dat' u 1:2 lc rgb 'red' with filledcurves title 'In-Degree', 'out-degree_distribution_month_002.dat' u 1:2 lc rgb 'goldenrod' with filledcurves title 'Out-Degree', 'cumulative-degree_distribution_month_002.dat' u 1:2 lc rgb 'blue' with filledcurves title 'Cumulative-Degree'`

Giving us:

<p align='center'>
<img src='../img/tutorial01/degree_distributions_month_002.svg'>
</p>

As expected, most agents have an in-degree of 10, an out-degree of 10, and a cumulative degree of 20.

Note that you can save your plots as .svg files by typing in the commands

`set term svg`

`set output 'filename.svg'`

when you first start up gnuplot and prior to actually plotting the data.

## Visualization

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/k4ULWpnRq8s" frameborder="0" allowfullscreen></iframe>
</p>

You can also use this data you collected to visualize your simulated network. Using [Gephi](http://gephi.github.io/), open the graph
file *network.gexf* found in the output directory of your simulation. Press 'OK' for the 'Import report' window that pops up,
and you will now see a rough outline of your network. Under the 'Partition' subheading on the left side of the page,
press the refresh symbol, and choose the partition parameter 'Label' and click 'Apply'. You are now free to choose a layout
for this network directly below the 'Apply' button you just pushed, and can run it for a few seconds. The following visualization
was made using the 'Fruchterman Reingold' and 'Clockwise Rotate' layout:

<p align='center'>
<img src='../img/tutorial01/visualization.png'>
</p>

We can see that these agents are randomly connected within this network simulation we created, with the highly connected agents at the centre
of the visualization and the agents with less connections on the sides.

## Next Steps

You have now completed your first tutorial using #k@, where you ran a simple network simulation, studied its output, and visualized it. 
Feel free to move on to the next tutorial, where we will discuss restarting a network. The input file used for this tutorial can be
found in the examples directory under tutorial01, so don't worry about changing the INFILE.yaml file in the hashkat directory, this is
encouraged! It is important to note that to get more refined graphs for your in-degree, out-degree, and cumulative-degree
distributions, you must have more agents present during your simulation. However, doing so will make it more difficult for
[Gephi](http://gephi.github.io/) to visualize your network, so take that into consideration when configuring your network
input file. 

Feel free to move on to the next tutorial, where we will discuss restarting a network.
