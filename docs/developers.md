[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# How Does #k@ work?

The code map below gives a brief summary of how the #k@ program runs.

![Code Map](/img/code_map.jpg "Code Map")

As we can see, after configuring the **INFILE.yaml** input file, the input file is read into Python via **INFILE.py**. Python then produces a generated input file in the form of **INFILE.yaml-generated** for C++ to read in the **main.cpp** file. From there either all the information in the input file is parsed into the **Analyzer** struct via **INFILE.yaml-generated** being parsed into **config_dynamic.cpp** or the following could occur:

For the latest build of #k@, see the github repository at [github.com/hashkat/hashkat](https://github.com/hashkat/hashkat)
