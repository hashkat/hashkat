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

1. Download the latest build of #k@ at [hashkat.org](http://hashkat.org).
2. Enter a terminal and go into your downloads directory by typing in the following command and pressing **Enter**:

    `cd Downloads`

3. To see the list of files in your Downloads directory, type into the command line:

    `ls`

4. Amongst your various other downloads you should see the following:

    `hashkat.2015.05.01.tar.gz`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine,
it just means that you've downloaded a newer version of #k@.

5. To unpack this tar file, type in the command:

    `tar xzvf hashkat.2015.05.01.tar.gz`

6. Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

7. For ease of access, move this directory into your home directory by typing in the command:

    `mv ~/Downloads/hashkat/ ~/`	

8. Now return to you home directory by typing in the command:

    `cd`

9. Let's now go into the hashkat directory by typing in the command:

    `cd hashkat`

10. Once in this directory, you can see the list of all the files and directories that compose the #k@ project.
11. In order to run simulations on hashkat, you must first edit the .bashrc file. You can enter this file
by typing in the command:

    `nano ~/.bashrc`

12. Once inside this file, assuming you have hashkat at the top of your home directory, you must type in:

    `export HASHKAT=~/hashkat`

    `export PATH=$PATH:$HASHKAT/build/src`

    `export PATH=$PATH:$HASHKAT`

13. Exit this file by pressing **ctrl-x**. If you are asked whether or not changes should be saved, type in **y** to say 'yes'.

14. Once you have exited .bashrc, reload this file by typing in the command:
    
    `source ~/.bashrc`

15. You can now run hashkat in this hashkat directory, as well as any other directory as long as it has **INFILE.yaml**
within in it.

16. You can test the build by typing in the command:

    `hashkat_pre.py`

    And then typing in the following command to run the simulation:

    `hashkat`

    The program should run without errors, however, if it does not it is most likely just due to the lack of several packages
on your computer. Steps 17 and 18 outline how to install these packages.

17. The simulation may not run due to the lack of several necessary packages. You can install packages to your computer using homebrew.
To install homebrew to your computer, simply type in the command:

    `ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

18. With Homebrew, you can install these packages into your computer in the following manner:

    * **cmake** - `brew install cmake`
    * **python** - `brew install python`
    * **yaml** - `pip install pyyaml`
    * **scipy** - `pip install scipy`
    * **numpy** - `pip install numpy`

19. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website
to use for visualization of collected data.

## Linux

1. Download the latest build of #k@ at [hashkat.org](http://hashkat.org).
2. Enter a terminal and go into your downloads directory by typing in the following command and pressing **Enter**:

    `cd Downloads`

3. To see the list of files in your Downloads directory, type into the command line:

    `ls`

4. Amongst your various other downloads you should see the following:

    `hashkat.2015.05.01.tar.gz`

    Please note that the date in the file you downloaded may be different from the one above. That's totally fine,
it just means that you've downloaded a newer version of #k@.

5. To unpack this tar file, type in the command:

    `tar xzvf hashkat.2015.05.01.tar.gz`

6. Once this is completed, look into the list of files in this directory and you will see amongst them:

    `hashkat`

7. For ease of access, move this directory into your home directory by typing in the command:

    `mv ~/Downloads/hashkat/ ~/`

8. Now return to you home directory by typing in the command:

    `cd`

9. Let's now go into the hashkat directory by typing in the command:

    `cd hashkat`

10. Once in this directory, you can see the list of all the files and directories that compose the #k@ project.
11. In order to run simulations on hashkat, you must first edit the .bashrc file. You can enter this file
by typing in the command:

    `nano ~/.bashrc`

12. Once inside this file, assuming you have hashkat at the top of your home directory, you must type in:

    `export HASHKAT=~/hashkat`

    `export PATH=$PATH:$HASHKAT/build/src`

    `export PATH=$PATH:$HASHKAT`

13. Exit this file by pressing **ctrl-x**. If you are asked whether or not changes should be saved, type in **y** to say 'yes'.

14. Once you have exited .bashrc, reload this file by typing in the command:

    `source ~/.bashrc`

15. You can now run hashkat in this hashkat directory, as well as any other directory as long as it has **INFILE.yaml**
within in it.

16. You can test the build by typing in the command:

    `hashkat_pre.py`

    And then typing in the following command to run the simulation:

    `hashkat`

    The program should run without errors, however, if it does not it is most likely just due to the lack of several packages
on your computer. Step 17 outlines how to install these packages.

17. The simulation may not run due to the lack of several necessary packages. You can install these packages into your computer in the following manner:

    * **cmake** - `sudo apt-get install cmake`
    * **python** - `sudo apt-get install python`
    * **yaml** - `pip install pyyaml`
    * **scipy** - `pip install scipy`
    * **numpy** - `pip install numpy`

18. It is also recommended to download and install [Gephi](http://gephi.github.io/) from their website
to use for visualization of collected data.


