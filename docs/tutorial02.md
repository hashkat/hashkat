[hashkat.org](http://hashkat.org)

# Restarting a Network

A brief explanation on restarting a network simulation, this tutorial should take approximately 10 minutes to complete.

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

Though not necessary, you can also remove the other files created from running a network simulation by typing in the commands:

`rm DATA_vs_TIME`

##### To Move:

Create a new directory to store your old simulations, for example:

`mkdir ~/hashkat/MyOldSimulations`

because this is off the path, you may simply move the unwanted files & directories into 'MyOldSimulations' and they will be invisible to **hashkat**:

`mv ~/hashkat/network_stat.dat ~/hashkat/MyOldSimulations/`

`mv -r ~/hashkat/output/ ~/hashkat/output`

If you do this more than once, you will end up with a lot of output directories of the same name in 'MyOldSimulations'.  Therefore you may want to move and rename them.

##### To Rename:

You may also simply rename the unwanted files and directories, which will make them invisible to **hashkat**.  You may rename by copying them to a new name, then removing the old name.

To copy:

`cp network_state.dat unwanted_network_state.dat`

`cp -r output unwanted_output`

then remove the old file:

`rm network_state.dat`

`rm -r output`

#### Run New Simulation

You can now run your new simulation by typing in the command:

`./run.sh`

#### Stopping and Continuing a Simulation

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/-gQ52JW2s-Y" frameborder="0" allowfullscreen></iframe>
</center>

If for whatever reason you would like to stop a simulation midway through, without losing your data, simply press **Ctrl-c**. The simulation will automatically save the information it's collected to the 'network_state.dat' file.  You may analyze the data you have so far in the 'DATA_vs_TIME' file and 'output' directory. 

If you would like to continue this simulation from where you left off, simply type in:

`./run.sh`

again and the simulation will recommence running, if you have time left.  Note: you may increase the maximum simulation time in your 'INFILE.yaml' file and the network simulation will simply continue from where it previously left off.

It is possible to continue running the same simulation with a new configuration in 'INFILE.yaml' but this give complex output data and is discouraged.

#### Next Steps

With the completion of this second tutorial using **#k@**, you are now familiar with running, stopping, and continuing a new network simulation in any directory. 

Proceed to the next tutorials for a demonstration of the different follow models in **k@**, starting with the [random follow model](http://docs.hashkat.org/en/latest/tutorial03/).