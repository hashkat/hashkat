[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">August 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

The ***#k@*** project is compatible with both 'Mac OS X' and 'Linux'.

We will go over the steps to install these programs for each particular operating system below.

# Mac OS X

Here outlines the following steps on how to install ***#k@*** on Mac OS X:

1. Download the latest build of ***#k@*** at [hashkat.org](http://hashkat.org) by clicking on 'Latest Build'.
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

1. You will need to install several packages and tools on to your computer to build ***#k@***. You can install some of these packages to your computer using 'Homebrew'. To install 'Homebrew' to your computer, simply type in the command:

    `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

    The type in the following commands to install the necessary packages and tools (make sure to click 'Install' in the prompt that appears after entering the first command):

    `xcode-select --install`

    `brew install cmake`

1. You must now do a build of ***#k@*** on your computer by typing in the command:

    `./build.sh -O`

    You may need to install additional tools on to your computer to build ***#k@***. If a prompt appears asking you to do so, please do and then use the above command again.

1. You can now run ***#k@*** in this *hashkat* directory as long as it has *INFILE.yaml* within in it by typing in the command:

    `./run.sh`

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

1. Enter into the command line the following commands to install the 'CMake' package and the appropriate tools necessary to build ***#k@***:

    `sudo apt-get install cmake`

    `sudo apt-get install build-essential -y`

1. You must now do a build of ***#k@*** on your computer by typing in the command:

    `./build.sh -O`

1. You can now run ***#k@*** in this *hashkat* directory as long as it has *INFILE.yaml* within in it by typing in the command:

    `./run.sh`

 1. Once you have run ***#k@*** properly, remove all the network save and analysis files created from this simulation by typing in the following commands:

    `rm DATA_vs_TIME`

    `rm INFILE.yaml-generated`

    `rm network_state.dat`

    `rm -r output`

1. [Gnuplot](http://gnuplot.sourceforge.net/) is the primary graphing utility used in this documentation. To install gnuplot, type in the command:

    `sudo apt-get install gnuplot-x11`

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
