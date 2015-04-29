[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

The #k@ project is compatible with both Mac (OS X) and Linux. To run this project on either operating system, you will need the following:

* cmake (probably available through your friendly neighbourhood package manager)
* python
* yaml (try 'pip install pyyaml')
* scipy
* numpy
* networkx (for analysis)
* matplotlib (for analysis)

We will go over the steps to install these programs for each particular operating system below.

# Source Based

## Mac (OS X)

Here outlines the following steps on how to install #k@ on Mac (OS X):

1. Download the latest build ot #k@

sudo port install cmake
sudo brew install cmake
sudo pip install pyyaml
sudo pip install networkx

## Linux

Here outlines the following steps to install #k@ on Linux:

1. Download the latest build of #k# at [hashkat.org](http://hashkat.org)
2. Enter a terminal and go into your downloads directory by typing in the following command and pressing 'Enter' - `cd Downloads`
3. To see the list of files in your Downloads directory, type into the command line - `ls`
4. Amongst your various other downloads you should se the following - 'hashKAT.2015.03.25.tar.gz`
5. To unpack this tar file, type in the command - `tar xzvf`
6. Once this is completed, look into the list of files in this directory and you will see amngst them - `hashKAT`
7. Go into this directory by typing in the command - `cd hashKAT`
8. Once in this directory, you can see the list of all the files and directories that compose the #k@ project.
