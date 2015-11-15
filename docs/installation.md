[hashkat.org](http://hashkat.org)

# Installation

The **#k@** project is compatible with both Mac OS X and Linux. We will go over the steps to install **#k@** for each particular operating system.

Required programs:

*  **Python 2.7**.    At present Python3 is not supported.
*  [CMake](http://www.cmake.org/) 
*  [Gnuplot](http://gnuplot.sourceforge.net/)
*  [Gephi](http://gephi.github.io/)
*  for Mac OS X, an installer such as [Homebrew](http://brew.sh/)

## Mac OS X

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/UrqxCDf0NxY" frameborder="0" allowfullscreen></iframe>
</center>


Here outlines the following steps on how to install **#k@** on Mac OS X:

1. Download the latest build of **#k@** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build'.

1. Enter a terminal (which can be accessed by searching for 'terminal' in your system) and go into your **Downloads** directory/folder by typing in the following command and pressing **return**:

    `cd Downloads/`

1. To see the list of files in your **Downloads** directory, type into the command line:

    `ls`

1. Amongst your various other downloads you should see the following:

    `hashkat.2015.08.20.tar`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of ***#k@***!

1. To unpack this tar file, type in the command:

    `tar -xf hashkat.2015.08.20.tar`

1. Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

1. For ease of access, move this directory into your 'home' directory by typing in the command:

    `mv ~/Downloads/hashkat ~/`	

1. Now return to your 'home' directory by typing in the command:

    `cd`

1. Let's now go into the **hashkat** directory by typing in the command:

    `cd hashkat/`

1. Once in this directory, you can see the list of all the files and directories that compose the **#k@** project by again typing in the command:

    `ls`

1. You will need to install the [CMake](http://www.cmake.org/) package on to your computer to build **#k@**. You can install this package to your computer using [Homebrew](http://brew.sh/). To install Homebrew to your computer, simply type in the command:

    `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

    A prompt may appear asking you if you'd like to install the 'command line developer tools'. Click 'Install' and agree to the licensing agreement to install these tools. Once these tools have been installed, you can continue installing Homebrew by pressing **return**.

1. With **Homebrew** installed, you an install the **CMake** package by typing in the command:

    `brew install cmake`

1. You must now do a build of **#k@** on your computer by typing in the command:

    `./build.sh -O`  

   **build.sh** is followed by 'space dash capital oh'.

1. You can now run **#k@** in this **hashkat** directory as long as it has *INFILE.yaml* and *run.sh* within in it by typing in the command:

    `./run.sh`

1. Once you have run **#k@** properly, remove all the network save and analysis files created from this test simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output/`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install **Gnuplot**, type in the command:

    `brew install gnuplot -x11`

   **gnuplot** is followed by 'space dash ex one one'

1. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website to use for visualizations of collected data.

With **#k@** installed on your computer, please proceed to the following pages in the documentation to learn how to fully utilize this program.

## Linux

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/xuQIqFKXI9w" frameborder="0" allowfullscreen></iframe>
</center>

Here outlines the following steps on how to install **#k@** on Linux (these instructions assume you are using a Debian-based distribution such as Ubuntu, Linux Mint, or Debian):

1. Download the latest build of **#k@** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build' and then clicking 'Save File' and then 'OK' on the prompt that appears.

1. Enter a terminal (which can be accessed by searching for 'terminal' in your system) and go into your **Downloads** directory/folder by typing in the following command and pressing **Enter**:

    `cd Downloads`

1. To see the list of files in your **Downloads** directory, type into the command line:

    `ls`

1. Amongst your various other downloads you should see the following:

    `hashkat.2015.08.20.tar.gz`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of ***#k@***!

1. To unpack this tar file, type in the command:

    `tar -xzvf hashkat.2015.08.20.tar.gz`

1. Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

1. For ease of access, move this directory into your 'home' directory by typing in the command:

    `mv ~/Downloads/hashkat  ~/`

1. Now return to your 'home' directory by typing in the command:

    `cd`

1. Let's now go into the hashkat directory by typing in the command:

    `cd hashkat`

1. Once in this directory, you can see the list of all the files and directories that compose the **#k@** project by again typing in the command:

    `ls`

1. Enter into the command line the following commands to install the [CMake](http://www.cmake.org/) package and the appropriate tools necessary to build ***#k@***:

    `sudo apt-get install cmake`

    `sudo apt-get install build-essential -y`

    if asked for sudo permission, enter your Linux password.

1. You must now do a build of **#k@** on your computer by typing in the command:

    `./build.sh -O`

   **build.sh** is followed by 'space dash capital oh'.

1. You can now run **#k@** in this **hashkat** directory as long as it has **INFILE.yaml** and **run.sh** within in it by typing in the command:

    `./run.sh`

1. Once you have run **#k@** properly, remove all the network save and analysis files created from this test simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install **Gnuplot**, type in the command:

    `sudo apt-get install gnuplot-x11`

    **gnuplot** is followed by 'dash ex one one' with no space.  

    With Gnuplot installed, to access it, simply type:

    `plot` 

    at the beginning of the line in the console.  You should be in the same directory as the data files you want to access.

1. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website to use for visualizations of collected data.  **#k@** automatically produces files with a **.gexf** suffix, suitable for **Gephi**.  To access them with **Gephi**, got into Gephi from the desktop and navigate, within **Gephi**, to the desired file.  
   
   On my Ubuntu laptop to start **Gephi**, using a mouse, I go into my home directory, click on **Gephi**, go to the **bin** file, then click on the icon that is labelled simply 'gephi'.

With **#k@** installed on your computer, please proceed to the following pages to learn how to fully utilize this program.

## Supplementary Tools

This is not necessary for the running of **#k@**, but you can use the C++ compiler **Clang** if you're having issues with the 'g++' compiler by doing the following:

1. For Mac OS X, input the command:

    `brew install clang`

    For Linux, input the command:

    `sudo apt-get install clang`

1. Add the following to your *.bashrc* file (for both Mac OS X and Linux):

    `export CC=clang`

    `export CXX=clang++`

You can now use **Clang** as a C++ compiler when using **#k@**.