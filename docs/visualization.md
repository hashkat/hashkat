[hashkat.org](http://hashkat.org)

# Visualization

Here we outline ways in which you can visualize the data you've collected from running **#k@**.

## Gephi

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/GZYsnLApk24" frameborder="0" allowfullscreen></iframe>
</center> <br /> 
You can visualize some of the data you have collected using [Gephi](http://gephi.github.io/), the visualization tool primarily used in this documentation. 

The network we visualize here is the one created in [Tutorial 5](http://docs.hashkat.org/en/latest/tutorial05/).

#### How to Visualize a Network

Download [Gephi](http://gephi.github.io/) then enter the program and start a **new file**. 

These are the steps necessary to visualize the networks you've created:


1. When entering the program, you will see a prompt similar to the one below appear on the screen. 

    <center>
    <img src='../img/visualization/gephi_welcome.png'>
    </center>   

    Under 'New Project' click 'Open Graph File' to access the appropriate output file created from your network simulation to be used in Gephi.


2. The information to visualize your network is the **network.gexf** file.  The most popular tweet on your network is in the **retweet_viz.gexf**. These can be found in the **output** directory after running a simulation. Click on the file you prefer and press 'Open'.

    <center>
    <img src='../img/visualization/gephi_network_file.png'>
    </center>   

    
3. After opening the graph file (**.gexf**), the following prompt will appear.

    <center>
    <img src='../img/visualization/gephi_import_report.png'>
    </center> 

    There is nothing that you have to change here so you may simply press 'OK' and the outline of your created network will appear.

    <center>
    <img src="../img/visualization/gephi_origin.png"  />
    </center> 
 
    It is important to note that in the top right hand corner of the screen you will see the following, though with most likely different numbers:

    <center>
    <img src="../img/visualization/gephi_network_info.png">
    </center> 
  
    This details the number of nodes (agents) in the network and the total number of edges (connections/followings) within it.


4. To differentiate between the node types in your system, click the refresh button in the 'Partition' box on the left hand side of the screen and under '---Choose a partition parameter' click 'Label'. This will show each node type numbered by how they were ordered in the input file, their percentage out of the total number of agents, and their corresponding colour in the visualization. An image of this box is shown below
  
    <center>
    <img src="../img/visualization/gephi_label_nodes.png">
    </center> 
 
    Click 'Apply' to implement this labelling in to your visualization, giving you a visualization similar to the one below:

    <center>
    <img src="../img/visualization/gephi_basic_network.png">
    </center> 

    
5. To modify the layout of your network, go to the 'Layout' window on the left hand side of your page (shown below), choose the layout you would like to use, and press 'Run':
 
    <center>
    <img src='../img/visualization/gephi_choose_layout.png'>
    </center>


#### Explore

Now that you have set up your visualization of your network simulation, you are encouraged to explore all the features of Gephi that you can use for your visualizations. Two network layouts that we primarily use in this documentation are the 'Fruchterman Reingold' layout:

<center>
<img src='../img/visualization/gephi_fruchterman_reingold.png'>
</center>

and the 'Yifan Hu' layout:

<center>
<img src='../img/visualization/gephi_yifan_hu.png'>
</center>

(Note: we used the 'Contraction' layout in conjuntion with the 'Yifan Hu' layout for the above to fit everything onto the screen)

There are numerous other modifications and adjustments that can be made to your visualization. Ones which we use fairly often can be found at the bottom of your screen.

<center>
<img src='../img/visualization/gephi_modifications.png'>
</center>

By left clicking the light bulb button, you can change the background from white to black or vice-versa, and by right clicking it you can change the background color to whatever colour you'd like. By moving the edge weight scale, you can also adjust how emboldened the connection lines are in your visualization.

This just scratches the surface of all that you can do using Gephi. Try experimenting with some of its other features and see which configuration of your network visualization you like the best.

## Networkx

You can also visualize the networks you've created using [Networkx](https://networkx.github.io/). Networkx is a Python language software package that can be used to create, modify, and analyze networks. To install Networkx on to your computer, enter into the command line:

`pip install networkx`

#### Plotting Your Network

To create a graph of your network after running a simulation using Networkx, enter Python by typing in the command:

`python`

In Python, type in the following to create a graph of your network:

```python
import matplotlib.pyplot as plt`
```

```python
import networkx as nx`
```

```python
G = nx.read_edgelist('output/network.dat')`
```

```python
pos = nx.spring_layout(G,iterations=75)`
```

```python
nx.draw(G,pos)`
```

```python
plt.show()`
```

This will produce a plot similar to the one below:

<center>
<img src='../img/visualization/networkx_graph.png'>
</center>
<br />

Similar to our visualizations using Gephi, the red nodes in the above graph correspond to agents and the black edges in between them correspond to connections/followings between agents. It is important to note however that there is no way to distinguish between the different agent types via this method.

#### Analyzing Your Network

You may use Networkx to analyze the network you've created as well. Here we will outline some of the various functions and algorithms you can input into Python after plotting your graph to collect some information on your network. More information on [Networkx](https://networkx.github.io/documentation/latest/) can be found at their website [here](https://networkx.github.io/documentation/latest/).

##### Number of Nodes
 
To ascertain the number of nodes present in your network, type the command:

```python
nx.number_of_nodes(G)
```

#### Number of Edges

To find the number of edges in your network, entering the command:

```python
nx.number_of_edges(G)
```

#### Most Popular Agent

To find the most connected agent and its corresponding 'cumulative-degree', enter the commands:

```python
from operator import itemgetter
```

```python
sorted(G.degree_iter(),key=itemgetter(1),reverse=True)[0:1])
```

This will print to the screen (though most likely with different values):

```python
[(u'938', 35)]
```

In this case, '938' corresponds to the ID of the most connected agent and '35' corresponds to the cumulative-degree of this agent.

#### Most Common Cumulative-Degree

You can find the most common cumulative-degree of agents in the network and the number of agents with that cumulative-degree by entering the commands:

```python
from operator import itemgetter
```

```python
max(enumerate(nx.degree_histogram(G)), key=itemgetter(1)))
```

This will print to the screen (though most likely with different values):

```python
(20, 90)
```

where '20' corresponds to the most common cumulative-degree and '90' corresponds to the number of agents with that cumulative-degree.

#### Graph Density

A dense graph is one in which the number of edges in a network are very close to their maximum value. A network's graph density can have a maximum value of 1 and a minimum value of 0. You can calculate the graph density of your network by typing in the command:

```python
nx.density(G)
```

#### Average Shortest Path Length

The average shortest path length is the shortest number of edges on average between any two nodes in the network. You can calculate this value for your network by inputting the command:

```python
nx.average_shortest_path_length(G))
```

#### visualize.py

You can visualize and analyze your networks in the above manner by running the script in the '/~hashkat/visualize.py', by entering in the command after running a network simulation:

`../visualize.py`

Running this script will not only produce a plot of your network using Networkx, but it will also save that plot to a file named **graph.svg**. This script will also run the analysis functions and algorithms discussed above on your network. 

#### Explore

Though we discuss using Gephi in much greater detail in this documentation, you are encouraged to also analyze your networks using [Networkx](https://networkx.github.io/). You are encouraged to explore all of Networkx's features and functionalities to understand all the ways in which you can analyze the data you've collected from running **#k@**.
