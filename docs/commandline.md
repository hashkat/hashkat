[hashkat.org](http://hashkat.org)

# Using the Command Line

A computer terminal allows the user to enter data into and display data out of a computer. The command line is where a user can issue commands into the computer from the terminal. Your computer terminal can be accessed very simply by searching for 'terminal' in your Mac OS X or Linux system.  The commands typed into 'terminal' are case sensitive.

Using the command line within the computer terminal is required extensively when using the **#k@** program. If this is your first time using the computer terminal, have no fear! Here we will discuss several fundamental commands that may be necessary for you to use in the **#k@** program, and that are also essential to know when working in a computer terminal.

#### Path

A terminal used a hierarchical structure, starting at the **home** directory and branching 'downward'.  The path is the file's address within the system.  

Let us assume the **home** directory contains two directories (or folders), **directory_A** and **directory_B**, and each directory has a sub-directory, **sub_directory_A** and **sub_directory_B**, and each sub-directory has one file in it, called **file_A** and **file_B**. 
Then the path of **file_B** is:

`~/directory_B/sub_directory_B/file_B`

#### Home symbol (~)

Note that **~** is the symbol for the **home** directory.

#### Print Working Directory

To display to the screen the path of the directory you are currently working in, enter the following command:

`pwd`

#### List of Files and Directories

To output to the screen a list of files and directories within the current working directory, simply enter the command:

`ls`

#### Change Directory

One navigates to a different directory using the **cd** command. Typing the following will move you to the 'home' directory:

`cd`

To open a directory one level down from the directory you are in, type **cd directory_name**. 

Typing the following will move you into **directory_A** from the **home** directory:

`cd directory_A`

Typing the following will move you to the directory one level up:

`cd..`

If you wish to navigate to a directory not in your current path (i.e. from **sub_directory_B** to **sub_directory_A**) type the path:

`cd ~/directory_A/sub_directory_A`

#### Create a File

You can create a file using the command **touch**. Entering the following command will create the file **new_file**:

`touch new_file`

#### Create a Directory

A directory is created using the **mkdir** command. Entering the following command will create a new directory **directory_C**:

`mkdir directory_C`

#### Remove a File

One removes a file from the current working directory using the **rm** command. 

`rm unwanted_file`

#### Remove a Directory

One removes a directory and all its contents with the **rm -r** command. The following command will remove the directory **unwanted_directory**:

`rm -r unwanted_directory`

#### Move

One may move a file with the **mv** command.  To move a directory use the **mv -r** command.  The command is **mv present_location new_location**.  The two locations are separated by a 'space'. In the end, the file will be in the new location and NOT in the former location.

Let's try it out!

Using **directory_C** in the **home** directory, let's create **file_C** in **directory_C** then move it to **sub_directory_A**:

`cd`

`cd directory_C`

`touch file_C`

`mv ~/directory_C/file_C ~/directory_A/sub_directory_A/`

To see if you have been successful, you may go to the old and new directories and list their contents. 

#### Copy a File

A file is copied using the **cp** command.  As with **move**, to **copy** you enter the file's current location followed by the desired location. Let's copy **file_B** into **sub_directory_A**.  In the end the file copied will be in BOTH locations:

`cp ~/directory_B/sub_directory_B/file_B  ~/directory_A/sub_directory_A/`

#### Copy a Directory

A directory is copied using the **cp -r** command in the same way one copies a file.  

`cp -r ~/directory_C/ ~/directory_B/`

#### Open a .tar File

To open a **tar** file in Mac OS X, type the command:

`tar -xf filename.tar`

To open a **tar** file in a Linux system, type the command:

`tar xzvf filename.tar.gz`

#### Install a file

For Mac OS X we recommend installing [Homebrew](http://brew.sh/) to install programs. Once **Homebrew** is installed, the command is:

`brew install filename`

For debian Linux machines such as Ubuntu, files may be installed by typing:

`sudo apt-get install filename`

#### Sudo

**sudo** is a Linux command that tells the terminal to make a change that requires administrator approval. In most cases, you (the user) are the administrator. The terminal will prompt you for your **sudo** password if required. Enter it and proceed.  

#### Enabling a Text Editor

One may modify the contents of a file using a text editor. There are numerous text editors available for use, but if it's your first time using a text editor, we recommend using the [nano](http://www.nano-editor.org/) text editor, simply because it is one of the more basic text editors. You can use **nano** to edit a file in your current working directory by simply typing in the command **nano** followed by the name of the file you wish to edit.  

Entering in the following command will allow you to edit **file_A** (assuming you are in the directory where **file_A** is located):

`nano file_A`

#### Gnuplot

After **Gnuplot** is installed, to set parameters for your plot, i.e. title, type:

`set title desired_title`

Then to the data, simply type at the beginning of the line **plot**, for example:

`plot desired_file_name` 

You should be in the same directory as the data file you want to access.  The exact commands are listed in the tutorials.

#### Tips

To save time when using the command line, press **tab** to finish the name you are typing, or use the **up** or **down** directional arrows to use previous commands into the command line.

You can find more information on using the command line [here](http://cli.learncodethehardway.org/book/).