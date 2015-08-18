[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

The ***#k@*** project is compatible with both 'Mac OS X' and 'Linux'.

We will go over the steps to install these programs for each particular operating system below.

# Mac OS X

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/kzK3W0aydqM" frameborder="0" allowfullscreen></iframe>
</p>

Here outlines the following steps on how to install ***#k@*** on Mac OS X:

1. Download the latest build of ***#k@*** at [hashkat.org](http://hashkat.org).
1. Enter a terminal and go into your *Downloads* directory/folder by typing in the following command and pressing **Enter**:

    `cd Downloads`

1. To see the list of files in your *Downloads* directory, type into the command line:

    `ls`

1. Amongst your various other downloads you should see the following:

    `hashkat.2015.06.24.tar`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine,
it just means that you've downloaded a newer version of ***#k@***!

1. To unpack this tar file, type in the command:

    `tar -xf hashkat.2015.06.24.tar`

1. Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

1. For ease of access, move this directory into your 'home' directory by typing in the command:

    `mv ~/Downloads/hashkat/ ~/`	

1. Now return to you 'home' directory by typing in the command:

    `cd`

1. Let's now go into the *hashkat* directory by typing in the command:

    `cd hashkat`

1. Once in this directory, you can see the list of all the files and directories that compose the ***#k@*** project by again typing in the command:

    `ls`

1. You must now do a build of ***#k@*** on your computer by typing in the command:

    `build.sh`

    You may need to install additional tools on to your computer to build ***#k@***. If a prompt appears asking you to do so, please do. You may also need to install the 'CMake' package to your computer to build ***#k@***. If so, steps 12 and 13 outline how to install this package.

1. You may not be able to build ***#k@*** on your computer due to the lack of the 'CMake' package. You can install this package to your computer using 'Homebrew'.
To install 'Hmebrew' to your computer, simply type in the command:

    `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

1. With 'Homebrew', you can install 'CMake' into your computer by using the command:

    `brew install cmake`

    With this installed, you should be able to build ***#k@*** in the manner outlined in step 11.

1. You can now run ***#k@*** in this *hashkat* directory, as well as any other directory as long as it has *INFILE.yaml* within in it by typing in the command:

    `run.sh`

1. Once you have run ***#k@*** properly, remove all the network save and analysis files created from this simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install gnuplot, type in the command:

    `brew install gnuplot -x11`

1. It is also recommended to download and install ['Gephi'](http://gephi.github.io/) from their website to use for visualizations of collected data.

With ***#k@*** installed on your computer, please proceed to the following pages in the documentation to learn how to fully utilize this program.

# Linux

<p align = 'center'>
<iframe width="420" height="315" src="https://www.youtube.com/embed/ppI80vBbZqs" frameborder="0" allowfullscreen></iframe>
</p>

Here outlines the following steps on how to install ***#k@*** on Linux (these instructions assume you are using a Debian-based distribution such as Ubuntu, Linux Mint, or Debian):

1. Download the latest build of ***#k@*** at [hashkat.org](http://hashkat.org).
1. Enter a terminal and go into your *Downloads* directory/folder by typing in the following command and pressing **Enter**:

    `cd Downloads`

1. To see the list of files in your *Downloads* directory, type into the command line:

    `ls`

1. Amongst your various other downloads you should see the following:

    `hashkat.2015.06.24.tar.gz`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine, it just means that you've downloaded a newer version of ***#k@***!

1. To unpack this tar file, type in the command:

    `tar xzvf hashkat.2015.06.24.tar.gz`

1. Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

1. For ease of access, move this directory into your 'home' directory by typing in the command:

    `mv ~/Downloads/hashkat/ ~/`

1. Now return to your 'home' directory by typing in the command:

    `cd`

1. Let's now go into the hashkat directory by typing in the command:

    `cd hashkat`

1. Once in this directory, you can see the list of all the files and directories that compose the ***#k@*** project by again typing in the command:

    `ls`

1. You must now do a build of ***#k@*** on your computer by typing in the command:

    `build.sh`

    You may need to install the 'CMake' package and the appropriate build tools to your computer to build ***#k@***. If so, step 12 outlines how to install these.

1. The simulation may not run due to the lack of the 'CMake' package and the appropriate build tools. You can install these into your computer by typing in the commands:

    `sudo apt-get install cmake`

    `sudo apt-get install build-essential -y` 

    With this installed, you should be able to build ***#k@*** in the manner outlined in step 11.

1. You can now run ***#k@*** in this *hashkat* directory, as well as any other directory as long as it has *INFILE.yaml* within in it by typing in the command:

    `run.sh`

 1. Once you have run ***#k@*** properly, remove all the network save and analysis files created from this simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install gnuplot, type in the command:

    `sudo apt-get install gnuplot -x11`

1. It is also recommended to download and install ['Gephi'](http://gephi.github.io/) from their website to use for visualizations of collected data.

With ***#k@*** installed on your computer, please proceed to the following pages in the documentation to learn how to fully utilize this program.

# Supplementary Tools

This is not necessary for the running of ***#k@***, but you can use the C++ compiler 'Clang' if you're having issues with 'g++' by doing the following:

1. For Mac OS X, input the command:

    `brew install clang`

    For Linux, input the command:

    `sudo apt-get install clang`

1. Add the following to your *.bashrc* file:

    `export CC=clang`

    `export CXX=clang++`

You can now use 'Clang' as a C++ compiler when using ***#k@***.