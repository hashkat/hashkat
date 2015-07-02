[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Visualization

Here we shall outline ways in which you can visualize some of that data you have collected using [Gephi](http://gephi.github.io/). There are many other programs you can use for visualization, and you are encouraged to try them, we're just going to focus using Gephi however in this documentation.

## How to Visualize a Network

You can download Gephi through there [website](http://gephi.github.io/). Once you have finished downloading Gephi, enter the program, and we will go over the steps necessary to visualize the networks you've created.

1. When entering the program, you will see a prompt similar to the one below appear on the screen. 

	![Gephi - Welcome](/img/visualization/gephi_welcome.png "Gephi - Welcome")

    Under 'New Project' click 'Open Graph File...' to access the appropriate output file created from your network simulation to be used on Gephi.

2. The file used to visualize your network and its most popular tweet are the *network.gexf* file and *retweet_viz.gexf* file respectively. These can be found in the output directory after running a simulation. Click on whichever **.gexf** file you prefer and press 'Open'.

    ![Gephi - Open](/img/visualization/gephi_network_file.png "Gephi - Open")

3. After opening the graph file you will be visualizing, the following prompt will appear:

    ![Gephi - Import Report](/img/visualization/gephi_import_report.png "Gephi - Import Report")

    There is nothing that you have to change here so you can simply press 'OK', and the outline of your created network will appear:

    ![Gephi - Origin](/img/visualization/gephi_origin.png "Gephi - Origin")

    It is important to note that in the top right hand corner of the screen you will see the following, though with most likely different numbers: 
    
    ![Gephi - Network Info](/img/visualization/gephi_label_nodes.png "Gephi - Network Info")

    This details the number of nodes (agents) in the network and the total number of edges (connections/followings) within it.

4. To differentiate between the node types in your system, click the refresh button in the 'Partition' box on the left hand side of the screen and under '---Choose a partition parameter' click 'Label'. This will show each node type, their percentage out of the total number of agents, and their corresponding colour in the visualization. An image of this box is shown below:

    ![Gephi - Label Nodes](/img/visualization/gephi_label_nodes.png "Gephi - Label Nodes")

    Click 'Apply' to implement this labelling in to your visualization, giivng you a visualization similar to the one below:

    ![Gephi - Basic Network](/img/visualization/gephi_basic_network.png "Gephi - Basic Network")

5. To modify the layout of your network, go to the 'Layout' window on the left hand side of your page (shown below), choose the layout you would like to use, and press 'Run'.

![Gephi - Choose Layout](/img/visualization/gephi_choose_layout.png "Gephi - Choose Layout")

## Explore

Now that you have set up your visualization of your network simulation, you are encouraged to explore all the features of **Gephi** that you can use for your visualizations. Two network layouts that we primarily use in this documentation are the *Fruchterman Reingold* layout:

![Gephi - Fruchterman Reingold](/img/visualization/gephi_fruchterman_reingold.png "Gephi - Fruchterman Reingold")

and the *Yifan Hu* layout:

![Gephi - Yifan Hu](/img/visualization/gephi_yifan_hu.png "Gephi - Yifan Hu")

(Note: we used the *Contraction* layout in conjuntion with the *Yifan Hu* layout for the above example to fit everything onto the screen)

There are numerous other modifications and adjustments that can be made to your visualization. Ones which we use fairly often can be found at the bottom of your screen.

![Gephi - Modifications](/img/visualization/gephi_modifications.png "Gephi - Modifications")

By left clicking the light bulb button, you can change the background from white to black or vice-versa, and by right clicking it you can change the background color to whatever colour you'd like. By moving the edge weight scale, you can also adjust how emboldened the connection lines are in your visualization.

This just scratches the surface of all that you can do using **Gephi**. Try experimenting with some of its other features and see what configuration of your network visualization you like the best