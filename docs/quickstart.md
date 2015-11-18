[hashkat.org](http://hashkat.org)

# Quick Start

**#k@** simulates an online social network such as Twitter(tm).  In terms of graph theory agents are 'nodes' and follows 'edges'. Agents are created with certain characteristics and added to the network.  Periodically, agents chose to follow other agents.  Periodically, agents signal (tweet) or retransmit received signals (retweet).  Follow behavior may depend on the content of signals received or characteristics of the tweeting agent.  

Quickstart is intended for users who want to dive in and learn as they go.

#### Required Programs

**#k@** is compatible with both Mac OS X and Linux. Required programs:

*  **Python 2.7**.  At present Python3 & above are not supported.
*  [CMake](http://www.cmake.org/) 
*  [Gnuplot](http://gnuplot.sourceforge.net/)
*  [Gephi](http://gephi.github.io/)
*  for Mac OS X, an installer such as [Homebrew](http://brew.sh/)
*  recommended:  [Networkx](https://networkx.github.io/)

**#k@** is run from the terminal in Ubuntu/Linux.

See also the detailed [installation](http://docs.hashkat.org/en/latest/installation/) instructions.   

#### Build & Configuration Files

**#k@** is built in C++, configured in Python, then run in C++.  

Navigate to the hashkat directory and build:

`./build.sh`

The configuration file is called:

`INFILE.yaml`
 
INFILE.yaml is an exact copy of DEFAULT.yaml.  To create INFILE.yaml type:

`cp DEFAULT.yaml INFILE.yaml`

INFILE.yaml contains many parameters, many of which are never changed.  The simplified configuration file is:

`quickstart_INFILE.yaml`

Simply configure quickstart_INFILE.yaml then rename:

`cp quickstart_INFILE.yaml INFILE.yaml`

Any parameters not included in INFILE.yaml will be pulled from DEFAULT.yaml at run time.  

Experienced users may change build parameters in **hashkat/src/config_static.h**.

#### Running Hashkat For First Time

To run **#k@** the first time you must build it by typing in the command:

`./build.sh -O`  

**-0** is 'dash capital oh'.

Once built, you may run **#k@** by typing in the command:

`./run.sh`

#### Running Hashkat For Second Time

To run **#k@** a second time, you need to first remove some files, and remove or rename the output directory:

* remove:  DATA_vs_TIME
* remove:  hashkat/network_state.dat
* remove:  INFILE.yaml-generated
* remove or rename:  output directory

The [commands](http://docs.hashkat.org/en/latest/commandline/) are:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output  (<-to remove | to rename->)  mv output new_name_output`

Once these files are removed, to run the same INFILE.yaml again (will give exact same output):

`./run.sh`

To run the same simulation with a different random seed (will give different output):  

`./run.sh --rand` 

To change the simulation, change INFILE.yaml.

#### Create Your Own Simulation

To create your own network, first configure the network for time and size:

*  initial_agent, max_agents, agent add_rate, max_time (simulated time) and max_real_time
*  add rate

The simulation will run until the smaller of max_time (simulated) or max_real_time elapses.  In a simple simulation the ratio of simulated time to real time can be 900,000:1.  In a complicated simulation the ratio is lower.

To test your configuration file (Python is white space & case sensitive), run it with max_real_time set at 5 minutes.  Remember to delete the output and other files before running the full simulation.  

Next configure how edges are created and destroyed:

*  true/false:  [follow based on hashtags, follow back](http://docs.hashkat.org/en/latest/tutorial09/), [stage1_unfollow (unfollow for relative tweet rate](http://docs.hashkat.org/en/latest/tutorial11/) 
*  rates: [unfollow (absolute) tweet rate](http://docs.hashkat.org/en/latest/tutorial11/), [hashtag use](http://docs.hashkat.org/en/latest/tutorial09/)
*  follow_models:  [random](http://docs.hashkat.org/en/latest/tutorial03/), [twitter_suggest (preferential_global)](http://docs.hashkat.org/en/latest/tutorial04/), [agent](http://docs.hashkat.org/en/latest/tutorial05/), [preferential_agent](http://docs.hashkat.org/en/latest/tutorial06/), [hashtag](http://docs.hashkat.org/en/latest/tutorial07/), [twitter (combination)](http://docs.hashkat.org/en/latest/tutorial08/)

Agents and their signals are coded based on:

*  Ideology: from 1 to 4
*  [Agent_types](http://docs.hashkat.org/en/latest/tutorial12/):  from 1 to 200
*  Preference_Classes (retweet rates by agent_type & tweet_type): from 1 to 2
*  Regions:  EXACTLY 3
*  Languages (hardcoded): English, English+French, French, Spanish
*  Tweet_types (hardcoded) : plain, ideological, musical, humorous  

Note:  There must be EXACTLY 3 Regions but all but one may have add_weight = 0. 

Parameter numbers are set in the **BIN** variables in file **hashkat/src/config_static.h**.  If you change the numbers you must re-build **#k@** for the changes to take effect.

All but one of any of the above may be effectively disabled by setting its add_weight to zero.  Put another way, at least one of Ideology, Agent_type, Preference_Class, Language, & Tweet_type must have an add_weight that is not zero.

Further information is available in [documentation](http://docs.hashkat.org/en/latest/).     `

#### Hashkat Output

The data collected by **#k@** during the simulation is sent to files stored in the output directory, which is created fresh with each run. Information in **#k@** is may be collected by 'ranks' (bins/buckets) or individually.  It may be collected for the whole run or by time period (ie simulated month).  

Output is usually by array number not name.  So languages, for example, will be output as {0,1,2,3} being {English, English+French, French, Spanish}.

Data (**.dat**) files  may be viewed raw with any text editor, or may be visualized. The [output directory](http://docs.hashkat.org/en/latest/output/) includes files for:

*  Agent_Type_info.dat - one for each agent_type
*  Categories_Distro.dat
*  indegree (#follows), outdegree (#followed by) and culmulative degree reports for each agent (each agent has own ID)
*  degree distribution by follow_model
*  statistics re follows, followbacks, hashtags, retweets
*  most popular tweet contentrs
*  many others

**#k@** also creates output files that may be viewed & analyzed with other programs.  Files ending **.gexf** are configured to be [visualized](http://docs.hashkat.org/en/latest/visualization/) with [Gephi](http://gephi.github.io/). 

[Networkx](https://networkx.github.io/) may be used to create a graph of your network.  To create a **Networkx** plot, enter Python from the terminal command line by typing:

`python`

then run:

`../visualize.py`

This will produce a plot of your network and save the plot to **graph.svg**.  Or, in Python use commands:

```python
import matplotlib.pyplot as plt`
import networkx as nx`
G = nx.read_edgelist('output/network.dat')`
pos = nx.spring_layout(G,iterations=75)`
nx.draw(G,pos)`
plt.show()`
```
or other **Networkx** commands.  See the [visualization tutorial](http://docs.hashkat.org/en/latest/visualization/) for more analysis tools.

#### Further Information

Implementation of **#k@** is described more fully in [Input](http://docs.hashkat.org/en/latest/input/) documentation and in detailed [tutorials](http://docs.hashkat.org/en/latest/tutorial01/) with video demonstrations.  A [large network](http://docs.hashkat.org/en/latest/large_network/) using 128 kb of RAM is demonstrated and other [complex](http://docs.hashkat.org/en/latest/tutorial13/) scenarios.  For further information or [feedback](https://github.com/hashkat/hashkat) do not hesitate to contact us.

Have fun.

