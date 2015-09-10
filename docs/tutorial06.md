[hashkat.org](http://hashkat.org)

# The Preferential_Agent Follow Model

The *preferential_agent* follow model is exactly like the *agent* follow model, except that instead of following a random agent within a particular agent type, agents tend to follow agents within that agent type with the highest number of followers (in-degree). Let's try running a *preferential_agent* follow model simulation, using the input file created in [**Tutorial 5**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial05/INFILE.yaml) as a starting point. The input file that we will create in this tutorial can be found for reference in the **docs/tutorial_input_files** directory in **hashkat**, with this one under the title **tutorial06**. You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial06/INFILE.yaml). This tutorial should take approximately 10 minutes to complete.

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/_YHp19AxtMI" frameborder="0" allowfullscreen></iframe>
</center>

## Constructing The Network

Since we've already modified our input file for an *agent* follow model in the previous tutorial [here](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial05/INFILE.yaml), all we have to do is change the follow model in *INFILE.yaml* to *preferential_agent*, and ***#k@*** will do the rest.

## Running and Visualizing The Network

Running this simulation, we produced the following visualization:

<center>
<img src='../img/tutorial06/visualization.png'>
</center>

As you can see, not only do the *Celebrity* agents (red nodes) generally have much more connections than the *Standard* agents (blue nodes), but through the *twitter_suggest* follow model, there are particular agents within each agent type that have much more connections than other agents of the same type.

The cumulative-degree distribution for this network is shown below:

<center>
<img src='../img/tutorial06/cumulative-degree_distribution_month_000.svg'>
</center>

As we can see, the vast majority of agents in this network have a cumulative degree less than 20, while the other agents have differing cumulative degrees higher than this up to a value of almost 180.

The commands used to plot this graph in Gnuplot were:

`set style fill transparent solid 0.5 noborder`

`set title 'Cumulative-Degree Distribution'`

`set xlabel 'Cumulative-Degree'`

`set ylabel 'Normalized Cumulative-Degree Probability'`

`plot 'Celebrity_info.dat' u 1:4 lc rgb 'goldenrod' w filledcurves above y1=0 title 'Celebrity',`
`'Standard_info.dat' u 1:4 lc rgb 'blue' w filledcurves above y1=0 title 'Standard',`
`'cumulative-degree_distribution_month_000.dat' u 1:2 lc rgb 'dark-red' w filledcurves above y1=0`
`title 'Overall'`

## Next Steps

You have now familiarity with the *preferential_agent* follow model. Similar to the *agent* follow model, you are encouraged to continue experimenting with this follow model by adding more agent types into your simulation, such as bots and organizations, to create some compelling networks. You can alse implement *use_barabasi* into your simulation with an arbitrary amount of *barabasi_connections* to see what differences this causes in your network. When you are ready to move on, proceed to the next tutorial, where we deal with the [*hashtag* follow model](http://docs.hashkat.org/en/latest/tutorial07/).