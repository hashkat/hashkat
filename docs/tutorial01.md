[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Getting Started

An introduction to the ***#k@*** project, this tutorial will take approximately
40 minutes to complete.

## What is a Social Network?

A social network is a system consisting of connections made by agents to interact
with one another. The goal of ***#k@*** is to simulate these social networks
through implementation of the Kinetic Monte Carlo Method.

## What is the Kinetic Monte Carlo (KMC) Method?

The Kinetic Monte Carlo (KMC) Method carries out a simulation where the change in simulation time is influenced by the cumulative rate function of the rates within the system.

For ***#k@***, we have **R**, the cummulative rate function,
which is the sum of the agent add rate, tweet rate, follow rate, and retweet rate.  
It should be noted that the tweet rate, follow rate, and retweet rate are
all multiplied by the number of agents in the network.
At a simulated time of 0, a random number, *r*<sub>1</sub>, is generated. Depending on this number's value, it will either cause an agent to be added into the network,
a random agent to tweet, a random agent to follow another random agent, or a
random agent to retweet a tweet from another agent. **R** will then change
accordingly. If 'use_random_time_increment' is enabled, another random number *r*<sub>2</sub> will then be generated, and time will move forward by:

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

The following is a model of how this works in ***#k@***. We initially have one agent in the network, with the agent add rate having been set to 1 agent per simulated minute and the agent tweet rate being set to 1 tweet per simulated minute (we've ignored the follow rate and retweet rate for this example). At this moment, there is an even chance that another agent will be added to the network or the initial agent will make a tweet. A random number is generated. If the random number is less than 0.5, another agent will be added, if it is greater or equal to 0.5, the initial agent will tweet. Time will then move forward based on the cumulative rate function and possibly another generated random number, again depending on whether or not 'use_random_time_increment' is permitted.

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

This cycle will continue until the end of the simulation. For more information on how this is run in ***#k@***, please visit the [**Developers**](http://docs.hashkat.org/en/latest/developers/) page.

## Running a Simple Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/jY7m0VDZPjM" frameborder="0" allowfullscreen></iframe>
</center>

For this first tutorial, we're going to run a simple network using ***#k@***.
If you look into the **hashkat** directory / folder, you will find several 
different files and directories. For now we are just going to look at
*INFILE.yaml*, which is the file we use to input how we would like to 
configure our simulated network.

In this file, you will see all the different configurations which you can
implement into your network. We will discuss these parameters throughout these tutorials, but a detailed description of every single one
of these configurations can be found on the [**Input**](http://docs.hashkat.org/en/latest/input/) page. For this tutorial,
it is only necessary to know that we have a constant number of agents 
or users in this network, 1000, and that the simulation will run for 100,000
simulated minutes and a maximum of 1 minute real time.

Each agent will have their own particular characteristics that will be changed and manipulated in the following tutorials. A summary of these characteristics is outlined below:

<p align='center'>
<img src='../img/tutorial01/agent_info.png'>
</p>

Run the simulation by typing in the command:

`./run.sh`

If you experience any issues running the simulation, you can reference the [**Installation**](http://docs.hashkat.org/en/latest/installation/) page for details on running a simulation for the first time.

When running this program, you will see something similar to this outputted to the screen:

| Simulation Time (min) | Users | Follows | Tweets | Retweets | Unfollows | R | Real Time (s) |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 9.93e+04 | 1.00e+03 | 1.00e+04 | 0.00e+00 | 0.00e+00(0.00e+00) | 0.00e+00 | 1.00e-01 | 2.25e+00 |

This gives a list of data at a particular point in your simulation.
**Simulation Time (min)** corresponds to simulation time in minutes, **Users** corresponds to the number
of agents in the network at that particular point in the simulation, 
**Follows** show the number of follows that have occurred, **Tweets** displays
the number of tweets that have been made, **Retweets** shows the number of
retweets and in brackets beside it the number of active tweets (tweets that can be retweeted),
 **Unfollows** displays the number of unfollows that have occurred, **R** is the
cumulative rate function, and **Real Time (s)** is the length of real time that has occurred in seconds.

So as we can see from the above example, at a simulated time of 99,300 simulated minutes,
there were 1,000 agents in the network, 10,000 follows, 0 tweets, 0
retweets, and 0 unfollows had occurred, the cumulative rate function was 0.1,
and the real time that had elapsed was 2.25 seconds.

Once the simulation has concluded, the amount of time that the total analysis
took will be displayed on the screen in milliseconds.

Looking into the hashkat directory, you will see a *DATA_vs_TIME* file, which
contains the chart displayed above at several different simulation times, a
*network_state.dat* file which is where your simulation data will be saved to, and the **output** directory, which contains all the data you've
accummulated through simulation necessary for analysis. We shall discuss these analysis files below.

## Output of a Simple Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/83lnl6UXstQ" frameborder="0" allowfullscreen></iframe>
</center>

Going into the **output** directory and viewing its contents by inputting the commands

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

We can also use 'gnuplot' to plot the in-degree, out-degree, and cumulative-degree distributions for each simulated month for all the agents. An agent's in-degree is the number of agents following them, while its
out-degree is the number of agents it's following, and the cumulative-degree is the sum of these two values. The agents in this simulation were configured to  have a follow rate of 0.0001, which basically means that in a simulation 100,000 simulated minutes long, each agent will follow around 10 other agents, and be followed by about 10 agents. So we can therefore expect most agents to have an in-degree magnitude of 10, an out-degree magnitude of 10, and a cumlative-degree magnitude of 20. Let's plot and see!

To access 'gnuplot', type in the command:

`gnuplot`

To plot the in-degree, out-degree, and cumulative-degree distributions for 'month002', which contains the most up to date data, on the same graph with appropriate axis labels and a title, type in the following:

`set style fill transparent solid 0.5 noborder `

`set title 'Agent Degree Distributions'`

`set xlabel 'Degree'`

`set ylabel 'Normalized Degree Probability'`

`plot 'in-degree_distribution_month_002.dat' u 1:2 lc rgb 'red' with filledcurves title 'In-Degree',`
`'out-degree_distribution_month_002.dat' u 1:2 lc rgb 'goldenrod' with filledcurves title 'Out-Degree',`
`'cumulative-degree_distribution_month_002.dat' u 1:2 lc rgb 'blue' with filledcurves`
`title 'Cumulative-Degree'`

Giving us:

<p align='center'>
<img src='../img/tutorial01/degree_distributions_month_002.svg'>
</p>

As expected, most agents have an in-degree of 10, an out-degree of 10, and a cumulative degree of 20.

Note that you can save your plots by typing in the command:

`set output <filename>`

when you first start up 'gnuplot' and prior to actually plotting the data.

## Visualizing a Simple Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/XLLcYKKzI2A" frameborder="0" allowfullscreen></iframe>
</center>

You can also use this data you collected to visualize your simulated network. We will discuss how to do so using ['Gephi'](http://gephi.github.io/) here, but you can also find a more in-depth walkthrough on visualizaing networks on the [Visualization](http://docs.hashkat.org/en/latest/visualization/) page. Entering 'Gephi', open the graph
file *network.gexf* found in the **output** directory of your simulation. Press 'OK' for the 'Import report' window that pops up,
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

You have now completed your first tutorial using ***#k@***, where you ran a simple network simulation, studied its output, and visualized it. The input file used for this tutorial can be
found in the **docs/tutorial_input_files** directory under **tutorial01** as well as [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial01/INFILE.yaml), so don't worry about changing the *INFILE.yaml* file in the **hashkat** directory, this is
encouraged! It is important to note that to get more refined graphs for your in-degree, out-degree, and cumulative-degree
distributions, you must have more agents present during your simulation. However, doing so will make it more difficult for
'Gephi' to visualize your network, so take that into consideration when configuring your network
input file. 

Feel free to move on to the next tutorial, where we will discuss [restarting a network](http://docs.hashkat.org/en/latest/tutorial02/).