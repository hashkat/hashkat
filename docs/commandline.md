[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# Using the Command Line

A computer terminal allows a user to enter data into and display data out of a computer. The command line is where a user can issue commands into the computer from the terminal. Your computer terminal can be accessed very simply by searching for 'terminal' in you Mac OS X or Linux system.

Using the command line within your computer terminal is required extensively when using the **#k@** program. If this is your first time using the computer terminal, have no fear! Here we will discuss several fundamental commands that may be necessary for you to use in the **#k@** program, and that are also essential to know when working a computer terminal:

## Print Working Directory

To display to the screen the path of the working directory you are currently in, enter in the following command:

`pwd`

## List of Files and Directories

To output to the screen the list of files and directories within the directory you are currently in, simply enter into the command line:

`ls`

## Create a File

You can create a file using the command **touch**. Entering in the following command will create the file *newfile*:

`touch newfile`

## Make a Directory

You can create a directory using the **mkdir** command. Entering in the following command will create the directory *newdirectory*:

`mkdir newdirectory`

## Change Directory

You can change in to a different directory using the **cd** command. Simply typing in **cd** will move you into the 'home' diectory, while typing in **cd ..** will have you to move up one directory or move into the directory that the directory you were previously in was in. You can change into a specific directory using  **cd** in conjunction with the directory's name, though this directory must be present within the directory that you are currently in. Typing in the following will move you into the *Documents* directory:

`cd Documents`

## Remove a File

You can remove a file from the directory you are currently in using the **rm** command. Entering in the following command will remove the file *newfile*:

`rm newfile`

## Remove a Directory

You can remove a directory and all its contents from the directory you are currently in using the **rm -r** command. Entering in the following command will remove the directory *newdirectory*:

`rm -r newdirectory`

## Move

You can move a file or folder using the **mv** command. When moving a file or folder, you must type in its current location in your computer followed by the location where you would like to move it. Entering in the following command will move the file *newerfile* from your *Downloads* directory into your *Documents* directory:

`mv ~/Downloads/newerfile ~/Documents/`

Note that '~' symbolizes your 'home' directory.

## Copy a File

You can copy a file using the **cp** command, where you must enter in the file's current location as well as the location in which you would like its copy to be located in. Entering in the following command will make a copy of the file *newerfile* found in the *Documents* directory and place it in the 'home' directory:

`cp ~/Documents/newerfile ~/`

## Copy a Directory

You can copy a directory using the **cp -r** command in much the same way you copied a file. Entering in the following command will make a copy of the directoy *newerdirectory* found in the 'home' directory and place it in the *Documents* directory:

`cp -r ~/newerdirectory/ ~/Documents/`

## Enabling a Text Editor

You can modify the contents of a file using a text editor. There are numerous text editors available for you to use, but for this documentation, we will use the 'nano' text editor, simply because it is one of the more basic ones and a good text editor to start out with. You can use 'nano' to edit a file in your current working directory by simply typing in the command **nano** followed by the name of the file you wish to edit. Entering in the following command will edit the file *newerfile*:

`nano newerfile`

## Tips

To save time when using the command line, you can use **tab** to finish the name of a file or directory you are typing out and the **up** and **down** directional arrows to see and use previous commands that you have entered into the command line.

You can find more information on using the command line [here](http://cli.learncodethehardway.org/book/).
