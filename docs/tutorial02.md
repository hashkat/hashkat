[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Restarting a Network

A brief explanation on how to restart a network, this tutorial should take approximately 10 minutes to complete.

## Running a New Simulation

At the end of Tutorial 1, we had run a simple network simulation and studied its output. You now have the knowledge to run your own simple networks and analysis of those networks.
However, you will find that after modifying INFILE.yaml to your own specifications, and typing the commands to run the simulation:

`hashkat_pre.py`

`hashkat`

you will find that the simulation is not running properly. Though it looks like it's about to run, no new simulation is created, and no more data is collected. Inside the output
directory you will find that nothing has changed from your previous simulation.

Have no fear! This is simply because you have not removed the data you collected from the previous simulation from #k@. The manner in which to do this is simple, though be warned,
once removing this data you will lose it forever. Remove the save file that your previous simulaiton information was saved to by typing in the command:

`rm network_state.dat`

and remove the old contents of your output directory with the command:

`rm output/*`

You can now run your new simulation by typing in the commands:

`hashkat_pre.py`

`hashkat` 

## Stopping and Continuing a Simulation

If for whatever reason you would like to stop a simulation midway through running, without losing any of your data, simply press **ctrl-c**. The simulation will automatically
save the information it's collected to the *network_state.dat* file, and you can analyze the data you have so far in the *DATA_vs_TIME* file and *output* directory. If you would like
to continue this simulation from where you left off, simply type in:

`hashkat`

again and the simulation will recommence running.

<iframe width="420" height="315" src="https://www.youtube.com/embed/EomkuFDMQ30" frameborder="0" allowfullscreen></iframe>

## Next Steps

With the completion of this second tutorial using #k@, you are now familiar with restarting the
basic networks you learned how to create in Tutorial 1. Proceed to the next tutorial for an introduction
with using the various different follow models offered by #k@, starting with a lesson on the 
Random Follow Model.
