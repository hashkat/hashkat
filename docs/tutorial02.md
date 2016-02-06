[hashkat.org](http://hashkat.org)

# Restarting a Network

This is a brief explanation of how to restart a network simulation, or do a second simulation.  This tutorial should take 5 minutes to complete.

### Run a New Simulation

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/EmRJel-eGmw" frameborder="0" allowfullscreen></iframe>
</center> 

#### Remove Previous Simulation Data

At the end of [Tutorial 1](http://docs.hashkat.org/en/latest/tutorial01/), we had run a simple network simulation and studied its output. 

It is necessary to discard (or rename) the results of the old simulation to run a new simulation.  The results of the old simulation are found in:

`network_state.dat`

`~/hashkat/output/`

Here we show you how to REMOVE the files, which is permanent.  You may also move the files to a storage directory, and/or rename the unwanted files and directories.

##### To Remove:

To remove the old output:

`rm network_state.dat`

`rm -r output`

##### To Move:

You may simply MOVE the files into the output directory, then rename the output directory:
 
`mv network_state.dat /output/network_state.dat`

`mv output new_output`

###Run New Simulation

You can now run your new simulation from the same seed, type the command:

`./run.sh`

Alternatively, to run a simulation using a random seed, type the command:

`./run.sh --rand`

For this first tutorial, the first command is sufficient.

#### Stopping and Continuing a Simulation

If for whatever reason you would like to stop a simulation midway through, without losing your data, simply press **Ctrl-c**. The simulation will automatically save the information it's collected to the **network_state.dat** file.  

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/-gQ52JW2s-Y" frameborder="0" allowfullscreen></iframe>
</center>

If for whatever reason you would like to stop a simulation midway through, without losing your data, simply press **Ctrl-c**. The simulation will automatically save the information it's collected to the **network_state.dat** file.  

You may analyze the data you have so far which includes the **DATA_vs_TIME** file in the **output** directory. 

If you would like to continue the simulation from where you left off, simply type in:

`./run.sh`

Note:  to continue a simulation, do NOT use the random seed **./run.sh --rand** command.

The simulation will recommence running if you have time left.  Note: you may increase the maximum simulation time in your 'INFILE.yaml' file and the network simulation will simply continue from where it previously left off.

It is possible to continue running the same simulation with a new configuration of 'INFILE.yaml', but this gives complex output data and is discouraged.

#### Next Steps

With the completion of this second tutorial using **#k@**, you are now familiar with running, stopping, and continuing a new network simulation in any directory. 

Proceed to the next tutorials for a demonstration of the different follow models in **k@**, starting with the [random follow model](http://docs.hashkat.org/en/latest/tutorial03/).