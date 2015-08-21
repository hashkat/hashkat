[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Restarting, Stopping a Simulation and Running it in a Different Directory

A brief explanation on how to restart a network, this tutorial should take approximately 10 minutes to complete.

## Running a New Simulation

At the end of [**Tutorial 1**](http://docs.hashkat.org/en/latest/tutorial01/), we had run a simple network simulation and studied its output. You now have the knowledge to run your own simple networks and analysis of those networks.
However, you will find that after modifying *INFILE.yaml* to your own specifications, and typing the command to run the simulation:

`./run.sh`

you will find that the simulation is not running properly. Though it looks like it's about to run, no new simulation is created, and no more data is collected. Inside the **output**
directory you will find that nothing has changed from your previous simulation.

Have no fear! As briefly discussed on the [**Installation**](http://docs.hashkat.org/en/latest/installation/) page, this is simply because you have not removed the data you collected from the previous simulation from ***#k@***. The manner in which to do this is simple, though be warned,
once removing this data you will lose it forever. Remove the save file that your previous simulation information was saved to by typing in the command:

`rm network_state.dat`

and remove the old contents of your output directory with the command:

`rm -r output`

Though not necessary, you can also remove the other files created from running a network simulation by typing in the commands:

`rm DATA_vs_TIME`

`rm INFILE.yaml-generated`

These two files are always overwritten when running a new network simulation, so removing them is not necessary. This is just so you are aware that you can remove these files if need be.

You can now run your new simulation by typing in the command:

`./run.sh`

## Stopping and Continuing a Simulation

If for whatever reason you would like to stop a simulation midway through running, without losing any of your data, simply press **Ctrl-c**. The simulation will automatically
save the information it's collected to the *network_state.dat* file, and you can analyze the data you have so far in the *DATA_vs_TIME* file and **output** directory. If you would like
to continue this simulation from where you left off, simply type in:

`./run.sh`

again and the simulation will recommence running.

## Running ***#k@*** in a Different Directory

In all of these tutorials, we shall run ***#k@*** in the **hashkat** directory, where the only files necessary to run a simulation,an *INFILE.yaml* file and *run.sh*, are present. However, you may want to run ***#@*** in a different directory, perhaps in one of the tutorial directories in **docs/tutorial_input_files**. Typing in the command:

`./run.sh`

in one of these directories, will not run ***#k@***, even if there is *INFILE.yaml* present in this directory. The reason for this is that *run.sh* is not present in this directory, therefore, you cannot run that script in this manner. The only way to do this is to give a command that shows the location of *run.sh* in your network. We mentioned on the [**Using the Command Line**](http://docs.hashkat.org/en/latest/commandline/) page that inputting the command:

`cd ..`

will have you move up one directory. Having './' before inputting the script name *run.sh* tells the computer that this script is present in this current directory. Having '../' before inputting the script name *run.sh* tells the computer that this script is present one directory up from your current location. Inputting a directory name before inputting the script name *run.sh* such as 'hashkat/' tells the computer that this script is present in the subdirectory **hashkat**.

Let's try it out! Remove all the files you need in order to run a new simulation as discussed above, and change to your 'home' directory by typing in the command:

`cd`

Copy the input file into your home directory by typing in the command (assuming you have the **hashkat** directory as a subdirectory of your 'home' directory):

`cp ~/hashkat/INFILE.yaml ~/`

Let's now run ***#k@*** by typing in the command:

`hashkat/run.sh`

You have now run ***#k@*** in your home directory (feel free to remove all the files you copied and created for this simulation from your 'home' directory).

Let's now run ***#k@*** in a subdirectory of the **hashkat** directory. Change to the **tutorial01** directory in **docs/tutorial_input_files** in **hashkat** by typing in the command (assuming you are still in your 'home' directory and **hashkat** is an immediate subdirectory of your 'home' directory):

`cd hashkat/docs/tutorial_input_files/tutorial01`

Within this directory, you can see we just have an *INFILE.yaml* file, the input file we used in [**Tutorial 1**](https://github.com/hashkat/hashkat/blob/master/docs/tutorial_input_files/tutorial01/INFILE.yaml). Let's run ***#k@*** in this directory using this input file by typing in the command:

`../../../run.sh`

You have now run ***#k@*** in a subdirectory of the **hashkat** directory.

## Next Steps

With the completion of this second tutorial using ***#k@***, you are now familiar with restarting, stopping, running in different directories the
basic networks you learned how to create in [**Tutorial 1**](http://docs.hashkat.org/en/latest/tutorial01/). Proceed to the next tutorial for an introduction on using the various different follow models offered by ***k@***, starting with a lesson on the [Random Follow Model](http://docs.hashkat.org/en/latest/tutorial03/).
