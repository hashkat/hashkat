[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

The #k@ project is compatible with both Mac (OS X) and Linux. To run this project on either operating system, you will need the following:

* cmake (probably available through your friendly neighbourhood package manager)
* python
* yaml (try 'pip install pyyaml')
* scipy
* numpy
* networkx (for analysis)
* matplotlib (for analysis)

sudo port install cmake
sudo brew install cmake
sudo pip install pyyaml
sudo pip install networkx

We will go over the steps to install these programs for each particular operating system below.

# Source Based

## Mac (OS X)

Here outlines the following steps on how to install #k@ on Mac (OS X):

1. Download the latest build of #k@ at [hashkat.org](http://hashkat.org)
2. Enter a terminal and go into your downloads directory by typing in the following command and pressing **Enter**

    `cd Downloads`

3. To see the list of files in your Downloads directory, type into the command line

    `ls`

4. Amongst your various other downloads you should see the following

    `hashKAT.2015.03.25.tar.gz`

5. To unpack this tar file, type in the command

    `tar xzvf`

6. Once this is completed, look into the list of files in this directory and you will see amongst them

    `hashKAT`

7. Go into this directory by typing in the command

    `cd hashKAT`

8. Once in this directory, you can see the list of all the files and directories that compose the #k@ project.
9. To run the simulation, you must do the following:

    * First enter the .bashrc file. This can be done by typing in the command

        `nano ~/.bashrc`

    * Once inside this file, assuming you have hashkat at the top of your home directory, you must type in:

        `export HASHKAT=~/hashkat`

        `export PATH=$PATH:$HASHKAT/build/src`

        `export PATH=$PATH:$HASHKAT`

    * Exit this file, and then type in the command
    
        `source ~/.bashrc`

    * You can now run hashkat in any directory, as long as it has **INFILE.yaml** within in it
    * Whenever you'd like to run the simulation, you must first prepare it by typing in the command

        `hashkat_pre.py`

    * Then to run it type in the command

        `hashkat`

    * You can press **ctrl-c** at any time to stop the simulation and save your data. Running the simulation again will result in it continuing from the point where it halted.

10. The simulation may not run due to the lack of several necessary packages. You can install packages to your computer using homebrew.
Simply go to [Homebrew](http://brew.sh/)'s website, copy the command you see on the home page, and run it on your computer.
11. With Homebrew, you can install these packages into your computer in the following manner:

    * cmake - `brew install cmake`
    * python - `brew install python`
    * yaml - `pip install pyyaml`
    * scipy - `pip install scipy`
    * numpy - `pip install numpy`

12. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website
to use for visualization of collected data.

## Linux

Here outlines the following steps to install #k@ on Linux:

1. Download the latest build of #k@ at [hashkat.org](http://hashkat.org)
2. Enter a terminal and go into your downloads directory by typing in the following command and pressing **Enter**

    `cd Downloads`

3. To see the list of files in your Downloads directory, type into the command line

    `ls`

4. Amongst your various other downloads you should se the following

    `hashKAT.2015.03.25.tar.gz`

5. To unpack this tar file, type in the command

    `tar xzvf`

6. Once this is completed, look into the list of files in this directory and you will see amngst them

    `hashKAT`

7. Go into this directory by typing in the command

    `cd hashKAT`

8. Once in this directory, you can see the list of all the files and directories that compose the #k@ project.
9. To run the simulation, you must do the following:       

    * First enter the .bashrc file. This can be done by typing in the command 

        `nano ~/.bashrc`

    * Once inside this file, assuming you have hashkat at the top of your home directory, you must type in:

        `export HASHKAT=~/hashkat`

        `export PATH=$PATH:$HASHKAT/build/src`

        `export PATH=$PATH:$HASHKAT`

    * Exit this file, and then type in the command

        `source ~/.bashrc`

    * You can now run hashkat in any directory, as long as it as **INFILE.yaml** within in it
    * Whenever you'd like to run the simulation, you must first prepare it by typing in the command

        `hashkat_pre.py`

    * Then to run it type in the command

        `hashkat`


    * You can press **ctrl-c** at any time to stop the simulation and save your data. Running the simulation again will result in it continuing from the point where it halted.

10. The simulation may not run due to the lack of several necessary packages.
These packages can be installed into your computer in the following manner:

    * cmake - `sudo apt-get install cmake`
    * python - `sudo apt-get install python`
    * yaml - `pip install pyyaml`
    * scipy - `pip install scipy`
    * numpy - `pip install numpy`

11. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website
to use for visualization of collected data.
