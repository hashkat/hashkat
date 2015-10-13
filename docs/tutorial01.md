[hashkat.org](http://hashkat.org)

# Getting Started

An introduction to **#k@**, this tutorial will take approximately 20 minutes to complete.

#### What is a Social Network?

A network as seen through graph theory is a set of users or agents (nodes) and connections between them (edges).  

In the real world, a social network is a set of connections made by agents acting voluntarily, for recreational purposes.  Unlike, for example, a network set up to connect government offices, or all outlets of General Motors (tm), which can be considered top-down designed networks, (also with proprietary content), social networks are created bottom up, by agents who are NOT obligated to participate or produce signals.  Social networks are fluid and unique to the agent.
  
Within the social network, connected agents receive signals (content) from each other, either in one direction only (Twitter) or bidirectionally (Facebook).  For purpose of these tutorials, language appropriate to Twitter will be used.

The goal of **#k@** is to simulate the growth of social networks and track how signals propagate through the network.  

#### What is **#k@**?

In the **#k@** simulation, the network is set up with a certain number of initial agents and with an **add_rate** of new agents to the network. The network also has a **decay_rate** for signals, or time beyond which signals will no longer be retransmitted.

The agents are set up to be of different types, with different preferences.  The agents are configured for:

1.  The type of Agent (Standard, Celebrity, Organization or Bot).

2.  The Agent's characteristics in terms of Region, Language, and Ideology.

3.  The type of signal the Agent is likely to generate (tweet).  Signal types are General, Political, Humorous, Musical.  The signal type affects the signal's scope of transmission.

4.  The type of signal the Agent is likely to retransmit (retweet).

5.  The events that will cause the Agent to follow another Agent (follow-model)(edge creation).

6.  The events that will cause the Agent to unfollow another Agent (edge deletion).

7.  Rates (probabilities) are set for each type of behavior.  For example, Agent-type A may 50% likely to generate a political tweet and 50% likely to generate a musical tweet.  Agent-type B may only generate and retweet humorous tweets.  Agent A may tweet once a day, Agent B may tweet 10 times a day. 

8.  A simulated time and real time are set for the simulation to run.  

The simulation commences.  

At each increment in time agents chose:  whom to follow (and receive signals from);  whether or not to generate a signal (tweet); the content of the signal;  whether or not to retransmit a signal (retweet); and whether or not to cease following another agent (unfollow).

At the end of the allotted time, the simulation ceases.  Output files are generated and stored in **~/hashkat/output/**.  

The output files describe multiple characteristics of the network. The output files may be visualized through third party programs such as [Gnuplot](http://gnuplot.sourceforge.net/), [Gephi](http://gephi.github.io/) or 
[Networkx](https://networkx.github.io/). 


#### What is the Kinetic Monte Carlo (KMC) Method?


The [Kinetic Monte Carlo (KMC)](https://en.wikipedia.org/wiki/Kinetic_Monte_Carlo) algorithm modifies the time increment of the simulation to be dependent on the cumulative rate function **R** (the sum of the rates within the system).

For **#k@**, the cumulative rate function **R** is the sum of the network 'add' rate, and the different agent's 'tweet','follow', and 'retweet' rates. Note:  the 'tweet', 'follow', and 'retweet' rates are multiplied by the number of agents in the network.


**R = add_rate + (number of agents) x (tweet_rate + follow_rate + retweet_rate)**

or more specifically, for **n** number of agents_types in the system, the individual agent_type 'tweet', 'follow' and 'retweet' rates are summed.


**R = add_rate + (tweet_rate<sub>1</sub> + follow_rate<sub>1</sub> + retweet_rate<sub>1</sub>) + ... + (tweet_rate<sub>n</sub> + follow_rate<sub>n</sub> + retweet_rate<sub>n</sub>)**

The default build of **#k@** permits 200 different agent_types (n = 200).  However, you may wish modify the proportions of each agent type, i.e. to have 1000 agents of type 1 (T<sub>1</sub> = 1000), but only 10 of type 2, for a 100:1 ration of A:B.  Then the **R** function becomes:

**R = add_rate + T<sub>1</sub>(tweet_rate<sub>1</sub> + follow_rate<sub>1</sub> + retweet_rate<sub>1</sub>)+...+T<sub>n</sub>(tweet_rate<sub>n</sub> + follow_rate<sub>n</sub> + retweet_rate<sub>n</sub>)**

At simulated time of 0, a random number **r<sub>1</sub>** is generated. This random number is used to determine all choices at that point in time in the system.  For Agent A, there is a 50:50 chance of a Political or Musical tweet.  If **r<sub>1</sub>** equals, say, 0.40, Agent A will tweet politically, if **r<sub>1</sub>** is 0.60, Agent B will tweet with musical content.

The culmulative **R** will then change to reflect choices that have been made.  

If **use_random_time_increment** in **INFILE.yaml** is enabled, another random number **r<sub>2</sub>** is generated, and time will move forward by:

<center>
&Delta;<i>t</i> = -log(<i>r</i><sub>2</sub>) / <b>R</b>
</center>

If it is not enabled, time will move forward by:

<center>
&Delta;<i>t</i> = 1 / <b>R</b>
</center>

All of the rate functions will then be recalibrated to their new values and this cycle will repeat.

#### How to Run **#k@**

**#k@** is written in C++, a compiled (built) language.  First one builds the **#k@** program.  The build defines the **number** of Regions, Ideologies, Agent-types, and Preferences the agents may have in your simulated network.  

Then one configures the network, to give Agents their **particular** Region, Language, tweet Preferences, and rates etc.  Configuration is done in the file **~/hashkat/INFILE.yaml**.

For now, we will use the default build, and simply demonstrate how to modify configuration.  

Note:  The tutorial files are stored separately and do not affect the main build and configuration files of **#k@**.


#### Running a Simple Network


<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/jY7m0VDZPjM" frameborder="0" allowfullscreen></iframe>
</center>

For this first tutorial, we're going to run a simple network using **#k@**.  This network is unrealistic and is used for demonstration purposed only.

If you look into the **hashkat** directory, you will find several different files and directories. For now we are just going to look at **INFILE.yaml**, which is the file we use to configure our simulated network.

We will discuss the configuration parameters throughout these tutorials.  A detailed description of the parameters can be found on the [**Input**](http://docs.hashkat.org/en/latest/input/) page. 

For this tutorial, we will have a constant (unchanging) number of agents (n = 1000), one agent type, and the simulation will run for 100,000 simulated minutes and a maximum of 1 minute real time.  The **tweet** rate was set to 0.0001, while  **retweet**, and **unfollow** rates are set to zero.  Therefore:  **R = 0 + (1000)(0.0001 + 0 + 0) = 0.1**

Run the simulation by typing in the command:

`./run.sh`

You may refer to [**Installation**](http://docs.hashkat.org/en/latest/installation/) for more information on running a simulation for the first time.

When running this program, you will see something similar to this on the screen:

|<sub>Simulation Time (min)</sub>|<sub>Users</sub>|<sub>Follows</sub>|<sub>Tweets</sub>|<sub>Retweets</sub>|<sub>Unfollows</sub>|<sub>R</sub>|<sub>Real Time (s)</sub>|
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|<sub>9.93e+04</sub>|<sub>1.00e+03</sub>|<sub>1.00e+04</sub>|<sub>0.00e+00</sub>|<sub>0.00e+00(0.00e+00)</sub>|<sub>0.00e+00</sub>|<sub>1.00e-01</sub>|<sub>2.25e+00</sub>|


This gives a list of data at particular points in your simulation.  **Retweets** shows the number of retweets and in brackets beside it the number of **active** tweets.  'Active' tweets have not yet 'decayed' and are still eligible for retweeting. 



So as we can see, at a simulated time of 99,300 simulated minutes, there were 1,000 agents in the network, 10,000 follows, 0 tweets, 0 retweets, and 0 unfollows.  The cumulative rate function was 0.1, and the real time that elapsed was 2.25 seconds.

Once the simulation is concluded, the real time elapsed of the total analysis will be displayed on the screen in milliseconds.

Looking into the hashkat directory, you will see a **DATA_vs_TIME** file, which contains the chart displayed above at several different simulation times, a **network_state.dat** file which is where your simulation data will be saved to, and the **output** directory, which contains all the data you've accummulated during the simulation. 
#### Output of a Simple Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/83lnl6UXstQ" frameborder="0" allowfullscreen></iframe>
</center>

To go into the **output** directory and view its contents use the commands

`cd output`

`ls`

This lists all the files of information collected from the simulation. Let's look at some of this data. Go into the **main_stats.dat** file by typing in the command

`nano main_stats.dat`

**Nano** will display the results of **main_stats.dat** as text. We can see the **in-degree distribution** (an Agent's followers), **out-degree distribution** (whom an Agent follows), and **cumulative-degree distribution** (both an Agent's followers & following) for ALL the agents, by simulated month. 

The agents in this simulation were configured to have a follow rate of 0.0001 follows per simulated minute, so for a simulation of 100,000 simulated minutes, we would expect each agent to follow around 10 other agents, and be followed by about 10 agents. Therefore, we can expect most agents to have an in-degree value of 10, an out-degree value of 10, and a cumlative-degree value of 20. Let's plot and see!


<center>
<img src='~/hashkat/docs/img/tutorial01/main_stats.png'>
</center>

To plot the results, we may use [gnuplot](http://gnuplot.sourceforge.net/).  To access gnuplot, type in the command:

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

<center>
<img src='../img/tutorial01/degree_distributions_month_002.svg'>
</center>

As expected, most agents have an in-degree of about 10, an out-degree of about 10, and a cumulative degree of about 20.

Note that you can save your plots by typing in the command:

`set output <filename>`

when you first start up Gnuplot and prior to actually plotting the data.

#### Visualizing a Simple Network

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/XLLcYKKzI2A" frameborder="0" allowfullscreen></iframe>
</center>

You can also use the data you collected to visualize your simulated network. We will discuss how to do so using [Gephi](http://gephi.github.io/) here.  A more in-depth discussion can be found on the [Visualization](http://docs.hashkat.org/en/latest/visualization/) page. 

Entering Gephi, open the graph file **network.gexf** found in the **output** directory of your simulation. Press 'OK' for the 'Import report' window that pops up, and you will now see a rough outline of your network. Under the 'Partition' subheading on the left side of the page, press the 'refresh' symbol, and choose the partition parameter 'Label' and click 'Apply'. You are now free to choose a layout for this network directly below the 'Apply' button you just pushed, and can run it for a few seconds. The following visualization was made using the 'Fruchterman Reingold' and 'Clockwise Rotate' layout:

<center>
<img src='../img/tutorial01/visualization.png'>
</center>

We can see that these agents (the red nodes) are randomly connected within this network simulation we created, with the highly connected agents at the centre of the visualization and the agents with less connections on the sides.

#### Next Steps

You have now completed your first tutorial using **#k@**, where you ran a simple network simulation, studied its output, and visualized it. 

The input file used for this tutorial can be found in the **hashkat** directory in the **docs/tutorial_input_files** directory under **tutorial01** as well as [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial01/INFILE.yaml), so the main configuration file, **~/hashkat/INFILE.yaml** has not yet been changed.

Feel free to move on to the next tutorial, where we will discuss [restarting a network](http://docs.hashkat.org/en/latest/tutorial02/).