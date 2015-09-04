[hashkat.org](http://hashkat.org)

# Visualization

Here we shall outline ways in which you can visualize the networks you've created using ***#k@***.

## Gephi

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/GZYsnLApk24" frameborder="0" allowfullscreen></iframe>
</center>

You can visualize some of the data you have collected using [Gephi](http://gephi.github.io/). There are many other programs you can use for visualizations, and you are encouraged to try them, we're just going to focus on using Gephi however in this documentation. The network we visualized here was the one created in [**Tutorial 5**](http://docs.hashkat.org/en/latest/tutorial05/).

### How to Visualize a Network

You can download Gephi through there [website](http://gephi.github.io/). Once you have finished downloading Gephi, enter the program, and we will go over the steps necessary to visualize the networks you've created.

1. When entering the program, you will see a prompt similar to the one below appear on the screen. 

    <center>
    <img src='../img/visualization/gephi_welcome.png'>
    </center>

	Under 'New Project' click 'Open Graph File...' to access the appropriate output file created from your network simulation to be used in Gephi.


1. The file used to visualize your network and its most popular tweet are the *network.gexf* file and *retweet_viz.gexf* file respectively. These can be found in the **output** directory after running a simulation. Click on whichever *.gexf* file you prefer and press 'Open'.

    <center>
    <img src='../img/visualization/gephi_network_file.png'>
    </center>

    
1. After opening the graph file you will be visualizing, the following prompt will appear:
    
    <center>
    <img src='../img/visualization/gephi_import_report.png'>
    </center> 
    
    There is nothing that you have to change here so you can simply press 'OK', and the outline of your created network will appear:

    <center>
    <img src='../img/visualization/gephi_origin.png'>
    </center>
    
    It is important to note that in the top right hand corner of the screen you will see the following, though with most likely different numbers: 
    
    <center>
    <img src='../img/visualization/gephi_network_info.png'>
    </center>
    
    This details the number of nodes (agents) in the network and the total number of edges (connections/followings) within it.


1. To differentiate between the node types in your system, click the refresh button in the 'Partition' box on the left hand side of the screen and under '---Choose a partition parameter' click 'Label'. This will show each node type numbered by how they were ordered in the input file, their percentage out of the total number of agents, and their corresponding colour in the visualization. An image of this box is shown below:

    <center>
    <img src='../img/visualization/gephi_label_nodes.png'>
    </center>
    
    Click 'Apply' to implement this labelling in to your visualization, giivng you a visualization similar to the one below:

    <center>
    <img src='../img/visualization/gephi_basic_network.png'>
    </center>

    
1. To modify the layout of your network, go to the 'Layout' window on the left hand side of your page (shown below), choose the layout you would like to use, and press 'Run'.

<center>
<img src='../img/visualization/gephi_choose_layout.png'>
</center>

### Explore

Now that you have set up your visualization of your network simulation, you are encouraged to explore all the features of Gephi that you can use for your visualizations. Two network layouts that we primarily use in this documentation are the *Fruchterman Reingold* layout:

<center>
<img src='../img/visualization/gephi_fruchterman_reingold.png'>
</center>

and the *Yifan Hu* layout:

<center>
<img src='../img/visualization/gephi_yifan_hu.png'>
</center>

(**Note**: we used the *Contraction* layout in conjuntion with the *Yifan Hu* layout for the above example to fit everything onto the screen)

There are numerous other modifications and adjustments that can be made to your visualization. Ones which we use fairly often can be found at the bottom of your screen.

<center>
<img src='../img/visualization/gephi_modifications.png'>
</center>

By left clicking the light bulb button, you can change the background from white to black or vice-versa, and by right clicking it you can change the background color to whatever colour you'd like. By moving the edge weight scale, you can also adjust how emboldened the connection lines are in your visualization.

This just scratches the surface of all that you can do using Gephi. Try experimenting with some of its other features and see what configuration of your network visualization you like the best.

## Networkx

You can also visualize the networks you've created using [Networkx](https://networkx.github.io/). Networkx is a Python language software package that can be used to create, modify, and analyze networks. To install Networkx on to your computer, enter into the command line:

`pip install networkx`

You now have Networkx installed on your computer! To create a graph of your network after running a simulation, enter Python by typing in the command:

`python`

In Python, type in the following to create a graph of your network:

`import matplotlib.pyplot as plt`

`import networkx as nx`

`G = nx.read_edgelist('output/network.dat')`

`nx.draw(G)`

`plt.show()`

This will produce a plot similar to the one below:

<center>
<img src='../img/visualization/networkx_graph.png'>
</center>

Similar to our visualizations using Gephi, the red nodes in the above graph correspond to agents and the black edges in between them correspond to connections/followings between agents, though it is important to note that there is no way to distinguish between the different agent types via this method.

Though Gephi is of course the visualization tool primarily used in this software, you can visualize your networks using the above manner by running the script in the **hashkat** directory *visualize.py* by entering the command after running a network simulation:

`./visualize.py`

Running this script will not only produce a plot of your network using Networkx, but it will also save that plot to a file named *graph.svg*.

Though as previously mentioned we go over using Gephi in much greater detail in this documentation, you are encouraged to also analyze your networks using Networkx, to have a full handle on how to analyze the data you've collected by running ***#k@***.