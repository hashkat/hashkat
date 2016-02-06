[hashkat.org](http://hashkat.org)

# Quick Start

**#k@** simulates an online social network such as Twitter(tm).  In terms of graph theory agents are 'nodes' and follows 'edges'. Agents are created with certain characteristics and added to the network.  Periodically, agents chose to follow other agents.  Periodically, agents signal (tweet) or retransmit received signals (retweet).  Follow behavior may depend on the content of signals received or characteristics of the tweeting agent.  

Quickstart is intended for users who want to dive in and learn as they go.

**#k@** is built in C++, configured in Python 2.7, then run in C++.  **#k@** is compatible with both Mac OS X and Linux. At present Python 3 & above are not supported.

Recommended programs:

*  [CMake](http://www.cmake.org/) 
*  for Mac OS X, an installer such as [Homebrew](http://brew.sh/)
*  [Gnuplot](http://gnuplot.sourceforge.net/)
*  [Gephi](http://gephi.github.io/). Our documentation is in **version 0.8**
*  [Networkx](https://networkx.github.io/)

#### Running Hashkat in Ubuntu/Linux

**#k@** is run from the terminal in Ubuntu/Linux.

See also the detailed [installation](http://docs.hashkat.org/en/latest/installation/) instructions.   

Once in the hashkat directory, build the program.

`./build.sh`  or
`./build.sh -O`  

**O** is 'capital Oh'.

Then configure your desired simulation in Hashkat by by creating a copy of the DEFAULT.yaml file and call it **INFILE.yaml** (exactly).

`cp DEFAULT.yaml INFILE.yaml`

INFILE.yaml contains many parameters, many of which are never changed.  A simplified configuration file is called quickstart_INFILE.yaml.  You may simply configure quickstart_INFILE.yaml then rename it:

`cp quickstart_INFILE.yaml INFILE.yaml`

Any parameters not included in INFILE.yaml will be pulled from DEFAULT.yaml at run time.  

#### Create Your Own Simulation

This information is elaborated in the excellent **Tutorials** and also in the description of **Input**.

Hashkat is a graph creation program mimicking a social network.  The program needs to be configured for:

* number of agents & run time
* agent (node) characteristics such as tweet & follow preferences & rates 
* edge creation & destruction (how agents will follow & unfollow each other)
* region creation, or the language, ideology, and re-tweet behavior of the regional population

To create your own network, first configure the network for time and number of agents:

*  initial_agents
*  max_agents 
*  max_time (simulated minutes) 
*  max_real_time (real minutes)
*  (agent) add_rate

The simulation will run until the lesser of max_time or max_real_time elapses.  The ratio of simulated time to real time can be 500,000:1 or higher.

Next configure how edges (follows) are created and destroyed (refer to tutorials, glossary or input for more information):

*  true/false:  [follow based on hashtags, follow back](http://docs.hashkat.org/en/latest/tutorial09/), [stage1_unfollow (unfollow for relative tweet rate](http://docs.hashkat.org/en/latest/tutorial11/) 
*  rates: [unfollow (absolute) tweet rate](http://docs.hashkat.org/en/latest/tutorial11/), [hashtag use](http://docs.hashkat.org/en/latest/tutorial09/)
*  follow_models:  [random](http://docs.hashkat.org/en/latest/tutorial03/), [twitter_suggest (preferential_global)](http://docs.hashkat.org/en/latest/tutorial04/), [agent](http://docs.hashkat.org/en/latest/tutorial05/), [preferential_agent](http://docs.hashkat.org/en/latest/tutorial06/), [hashtag](http://docs.hashkat.org/en/latest/tutorial07/), [twitter (combination)](http://docs.hashkat.org/en/latest/tutorial08/)

Next configure nodes, or Agents, and their signalling (tweeting) behavior:

*  [Agent_types](http://docs.hashkat.org/en/latest/tutorial12/):  from 1 to 200
*  Tweet_types (hardcoded) : plain, ideological, musical, humorous  

Finally configure the population characteristics of regions:

*  Regions:  3 (exactly)
*  Ideology: from 1 to 4 ideologies are permitted in the default build
*  Languages (hardcoded): English, English+French, French, Spanish
*  Preference_Classes (retweet rates) by agent_type & tweet_type: from 1 to 2 

Although the default build specifies 4 ideologies, 3 regions, 2 preference classes (retweet profiles) and up to 200 agent types, only one of each needs to be configured.  The others may be effectively disabled by setting their add_weight to zero.  

12 short Tutorials have been created to guide you through the process.     

Advanced users may change the numbers of Ideologies, Agent Types, Preference Classes and Regions permitted in the **BIN** variables in file **hashkat/src/config_static.h**.  If you change the numbers you must re-build **#k@** for the changes to take effect.`

#### Hashkat Output

The data collected by **#k@** during the simulation is sent to files stored in the output directory, which is created fresh with each run. Information in **#k@** is may be collected by 'ranks' (bins/buckets) or individually.  It may be collected for the whole run or by time period (ie simulated month).  

Output is usually by array number not name.  So languages, for example, will be output as {0,1,2,3} being {English, English+French, French, Spanish}.

Data (**.dat**) files  may be viewed raw with any text editor, or may be visualized. The [output directory](http://docs.hashkat.org/en/latest/output/) includes files for:

*  Agent_Type_info.dat - one for each agent_type
*  Categories_Distro.dat
*  indegree (#follows), outdegree (#followed by) and culmulative degree (in + out) reports for each agent (each agent has own ID)
*  degree distribution by follow_model
*  statistics re follows, followbacks, hashtags, retweets
*  most popular tweet contents

**#k@** also creates output files that may be viewed & analyzed with other programs provided the number of agents is under 10,000.  

Files ending **.gexf** are configured to be [visualized](http://docs.hashkat.org/en/latest/visualization/) with [Gephi](http://gephi.github.io/). 

[Networkx](https://networkx.github.io/) may be used to create a graph of your network with a script pre-prepared in Hashkat.  To create a **Networkx** plot, enter Python from the terminal command line by typing:

`python`

then run:

`./visualize.py`

This will produce a plot of your network and save the plot to **graph.svg**.  

See the [visualization tutorial](http://docs.hashkat.org/en/latest/visualization/) for more analysis tools.

#### Running Hashkat For First Time

Once Hashkat has been built, and INFILE.yaml is created to your satisfaction, you may run **#k@** by typing in the command:

`./run.sh`

#### Running Hashkat For Second Time

To run **#k@** a second time, you need to clear some files out of the workspace, either by deleting or renaming them.

The files that need to be moved, renamed or deleted are:

* hashkat/network_state.dat
* hashkat/output/

#### To Remove

The [commands](http://docs.hashkat.org/en/latest/commandline/) to remove are:

    `rm network_state.dat`

    `rm -r output`
	
#### To Move & Rename

I prefer to move the file generated by the run to the output directory, then rename the output directory so I may look at the data later.  I do this with the commands:
	
	` mv network_state.dat \output\network_state.dat
	
	` cp \output\ \output_newname\`

Once these files are removed, to run the same INFILE.yaml again (will give exact same output):

`./run.sh`

To run the same simulation with a different random seed (will give different output):  

`./run.sh --rand` 

To change the simulation, change the configuration in INFILE.yaml.

#### Tips

To test your configuration file (Python is white space & case sensitive), run it with max_real_time set at 1 minute to start.  If everything goes well, you may run it for longer.

Remember to delete the output and other files before running the simulation again for a longer time.   

#### Further Information

Implementation of **#k@** is described more fully in [Input](http://docs.hashkat.org/en/latest/input/) documentation and in detailed [tutorials](http://docs.hashkat.org/en/latest/tutorial01/) with video demonstrations.  A [large network](http://docs.hashkat.org/en/latest/large_network/) using 128 kb of RAM is demonstrated and other [complex](http://docs.hashkat.org/en/latest/tutorial13/) scenarios.  For further information or [feedback](https://github.com/hashkat/hashkat) do not hesitate to contact us.

Have fun!

