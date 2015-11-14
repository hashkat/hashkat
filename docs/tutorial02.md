[hashkat.org](http://hashkat.org)

# Restarting a Network

This is a brief explanation of how to restart a network simulation, or do a second simulation.  This tutorial should take 5 minutes to complete.

### Run a New Simulation

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/EmRJel-eGmw" frameborder="0" allowfullscreen></iframe>
</center> 

#### Remove Previous Simulation Data

At the end of [Tutorial 1](http://docs.hashkat.org/en/latest/tutorial01/), we had run a simple network simulation and studied its output. 

It is necessary to rename the output directory before running a new simulation or your data will be over-written and lost.


##### To Move:

Create a new directory, for example, 'My_Output', to store your old simulation output:

`mkdir ~/hashkat/My_Output`

You may simply move the unwanted files & directories into 'My_Output' and they will be invisible to **#k@**:

`mv ~/hashkat/network_stat.dat ~/hashkat/My_Output/`

`mv -r ~/hashkat/output/ ~/hashkat/My_Output/`

##### To Rename:

You may also simply rename the files and directories, which will make them invisible to **#k@**.  You may rename by using the **mv** command.

To rename:

`mv network_state.dat my_network_state.dat`

`mv output my_output`

#### Run New Simulation

You can now run your new simulation by typing in the command:

`./run.sh`

Alternatively, to run a simulation using a random seed, type the command:

`./run.sh --rand`

#### Stopping and Continuing a Simulation

If for whatever reason you would like to stop a simulation midway through, simply press **Ctrl-c**. Unfortunately, under current defaults, your data will be lost.  

#### Next Steps

With the completion of this second tutorial using **#k@**, you are now familiar with running and stopping network simulation in any directory. 

Proceed to the next tutorials for a demonstration of the different follow models in **k@**, starting with the [random follow model](http://docs.hashkat.org/en/latest/tutorial03/).