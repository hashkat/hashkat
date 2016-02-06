[hashkat.org](http://hashkat.org)

# Installation

The **#k@** project is compatible with both Mac OS X and Linux. We will go over the steps to install **#k@** for each particular operating system.

Required programs:

*  **Python 2.7**.  At present Python 3 is not supported.
*  [CMake](http://www.cmake.org/) 
*  for Mac OS X, an installer such as [Homebrew](http://brew.sh/)

Recommended programs:

*  [Gnuplot](http://gnuplot.sourceforge.net/)
*  [Gephi](http://gephi.github.io/).  Our documentation is in **Gephi 0.8**.
*  [Networkx](https://networkx.github.io/)


## Mac OS X

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/UrqxCDf0NxY" frameborder="0" allowfullscreen></iframe>
</center>


The following outlines the installation of **#k@** on Mac OS X:

1. Download the latest build of **#k@** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build'.

1. Enter a terminal (which can be accessed by searching for 'terminal' in your system) and go into your **Downloads** directory/folder by typing in the following command and pressing **return**:

    `cd Downloads/`

1. To see the list of files in your **Downloads** directory, type into the command line:

    `ls`

1. Amongst your various other downloads you should see the following:

    `hashkat.2015.08.20.tar`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of **#k@**.

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

1. You must now do a build of **#k@** on your computer.  You may be in the Cmake directory.  You will need to go to the hashkat directory by typing in the commands:

    `cd`

    `cd hashkat`

    `./build.sh -O`  

    **build.sh** is followed by 'capital oh'.  The "O" stands for "optimized build" and is optional.

1. You can now run **#k@** in this **hashkat** directory by typing the command:

    `./run.sh`

1. In **hashkat** you need to remove (or move) the network_state and output files between simulations or an error will be thrown.  

   You may remove the files by typing the following commands:

    `rm network_state.dat`

    `rm -r output/`

   To move the data instead:

    `mv network_state.dat /output/network_stat.dat`

    `mv output output_new_name`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install **Gnuplot**, type in the command:

    `brew install gnuplot -x11`

    **gnuplot** is followed by 'ex one one'

1. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website to use for visualizations of collected data.  For simulations with **max_agents** <= 10000, **#k@** automatically produces files with a **.gexf** suffix, suitable for Gephi visualization.  Our tutorials are illustrated with **Gephi 0.8**.


## Linux

<center>
<iframe width="560" height="315" src="https://www.youtube.com/embed/xuQIqFKXI9w" frameborder="0" allowfullscreen></iframe>
</center>

The following steps outline how to install **#k@** on Linux (these instructions assume you are using a Debian-based distribution such as Ubuntu, Linux Mint, or Debian):

1. Download the latest build of **#k@** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build' and then clicking 'Save File' and then 'OK' on the prompt that appears.

1. Enter a terminal (which can be accessed by searching for 'terminal' in your system) and go into your **Downloads** directory/folder by typing in the following command and pressing **Enter**:

    `cd Downloads`

1. To see the list of files in your **Downloads** directory, type into the command line:

    `ls`

1. Amongst your various other downloads you should see the following:

    `hashkat.2015.08.20.tar.gz`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of **#k@**!

1. To unpack this tar file, type in the command:

    `tar -xzvf hashkat.2015.08.20.tar.gz`

   This command creates a hashkat directory within the Downloads directory.

1. Once this is completed, list the files in the Downloads directory and you will see amongst them:

    `hashkat`

1. For ease of access, move this directory into your 'home' directory ('mv old_location new_location') by typing in the command:

    `mv ~/Downloads/hashkat  ~/`

1. To return to your 'home' directory type in the command:

    `cd ~`

1. Now navigate to the directory where you have stored the Cmake tar file.

1. To install the [CMake](http://www.cmake.org/) package, type the following commands:

    `tar -xzvf cmake.version.tar.gz`

    `sudo apt-get install cmake`

    `sudo apt-get install build-essential -y`

    If asked for sudo permission, enter your Linux password.

1. You must now do a build of **#k@** on your computer.  You need to change back to the hashkat directory first:

    `cd ~`

    `cd hashkat`

    `./build.sh -O`

    **build.sh** is followed by 'capital oh'.  The "O" stands for "optimized build" and is optional.

1. You can now run **hashkat** by typing in the command:

    `./run.sh`

1. In **hashkat** you need to move or remove the network_state and output files between simulations or an error will be thrown.  

   You may remove the files created from the simulation by typing the following commands:

    `rm network_state.dat`

    `rm -r output`

   To move the files:

    `mv network_state.dat /output/network_stat.dat`

    `mv output output_with_description`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install **Gnuplot**, from the terminal type in the command:

    `sudo apt-get install gnuplot-x11`

    **gnuplot** is followed by 'dash ex one one' with no space.  

    With Gnuplot installed, to access it, simply type:

    `plot` 

    at the beginning of the line in the console.  You should be in the same directory as the data files you want to access.

1. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website to use for visualizations of collected data.  For simulations with **max_agents** <= 10000, **#k@** automatically produces files with a **.gexf** suffix, suitable for Gephi visualization.  Our tutorials are illustrated with **Gephi 0.8**.
  If **max_agents <= 10000** **#k@** automatically produces files with a **.gexf** suffix, suitable for visualization with **Gephi**.  
   


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