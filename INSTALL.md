# Installation

The ***#k@*** project is compatible with both Mac OS X, Linux and Microsoft Windows.

We will go over the steps to install ***#k@*** for each particular operating system:
* [Mac OS X](macosx)
* [Linux](linux)

***

## Mac OS X

Here outlines the following steps on how to install ***#k@*** on Mac OS X:

* Download the latest build of ***#k@*** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build'.
* Enter a terminal and go into your **Downloads** directory/folder by typing in the following command and pressing **return**:

    `cd Downloads/`

* To see the list of files in your **Downloads** directory, type into the command line:

    `ls`

* Amongst your various other downloads you should see the following:

    `hashkat.2015.08.20.tar`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of ***#k@***!

* To unpack this tar file, type in the command:

    `tar -xf hashkat.2015.08.20.tar`

* Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

* For ease of access, move this directory into your 'home' directory by typing in the command:

    `mv ~/Downloads/hashkat ~/` 

* Now return to your 'home' directory by typing in the command:

    `cd`

* Let's now go into the **hashkat** directory by typing in the command:

    `cd hashkat/`

* Once in this directory, you can see the list of all the files and directories that compose the ***#k@*** project by again typing in the command:

    `ls`

* You will need to install the 'CMake' package on to your computer to build ***#k@***. You can install this package to your computer using 'Homebrew'. To install 'Homebrew' to your computer, simply type in the command:

    `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

    A prompt may appear asking you if you'd like to install the 'command line developer tools'. Click 'Install' and agree to the licensing agreement to install these tools. Once these tools have been installed, you can continue installing 'Homebrew' by pressing **return**.

* With 'Homebrew' installed, you an install the 'CMake' package by typing in the command:

    `brew install cmake`

* You must now do a build of ***#k@*** on your computer by typing in the command:

    `./build.sh -O`

* You can now run ***#k@*** in this **hashkat** directory as long as it has *INFILE.yaml* and *run.sh* within in it by typing in the command:

    `./run.sh`

* Once you have run ***#k@*** properly, remove all the network save and analysis files created from this test simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output/`

* [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install gnuplot, type in the command:

    `brew install gnuplot -x11`

* It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website to use for visualizations of collected data.

With ***#k@*** installed on your computer, please proceed to the following pages in the documentation to learn how to fully utilize this program.

## Linux

Here outlines the following steps on how to install ***#k@*** on Linux (these instructions assume you are using a Debian-based distribution such as Ubuntu, Linux Mint, or Debian):

* Download the latest build of ***#k@*** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build' and then clicking 'Save File' and then 'OK' on the prompt that appears.
* Enter a terminal and go into your **Downloads** directory/folder by typing in the following command and pressing **Enter**:

    `cd Downloads`

* To see the list of files in your **Downloads** directory, type into the command line:

    `ls`

* Amongst your various other downloads you should see the following:

    `hashkat.2015.08.20.tar.gz`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of ***#k@***!

* To unpack this tar file, type in the command:

    `tar xzvf hashkat.2015.08.20.tar.gz`

* Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

* For ease of access, move this directory into your 'home' directory by typing in the command:

    `mv ~/Downloads/hashkat  ~/`

* Now return to your 'home' directory by typing in the command:

    `cd`

* Let's now go into the hashkat directory by typing in the command:

    `cd hashkat`

* Once in this directory, you can see the list of all the files and directories that compose the ***#k@*** project by again typing in the command:

    `ls`

* Enter into the command line the following commands to install the 'CMake' package and the appropriate tools necessary to build ***#k@***:

    `sudo apt-get install cmake`

    `sudo apt-get install build-essential -y`

* You must now do a build of ***#k@*** on your computer by typing in the command:

    `./build.sh -O`

* You can now run ***#k@*** in this **hashkat** directory as long as it has *INFILE.yaml* and *run.sh* within in it by typing in the command:

    `./run.sh`

* Once you have run ***#k@*** properly, remove all the network save and analysis files created from this test simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output`

* [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install gnuplot, type in the command:

    `sudo apt-get install gnuplot-x11`

* It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website to use for visualizations of collected data.

With ***#k@*** installed on your computer, please proceed to the following pages in the documentation to learn how to fully utilize this program.

## Supplementary Tools

This is not necessary for the running of ***#k@***, but you can use the C++ compiler 'Clang' if you're having issues with the 'g++' compiler by doing the following:

* For Mac OS X, input the command:

    `brew install clang`

    For Linux, input the command:

    `sudo apt-get install clang`

* Add the following to your *.bashrc* file (for both Mac OS X and Linux):

    `export CC=clang`

    `export CXX=clang++`

You can now use 'Clang' as a C++ compiler when using ***#k@***.
