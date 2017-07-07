# #k@ (hashkat) : Kinetic Analysis of Twitter-like networks

Build Status: [![Build Status](https://travis-ci.org/hashkat/hashkat.svg?branch=master)](https://travis-ci.org/hashkat/hashkat)

***

### Description
`#k@` is a dynamic network simulation tool designed to model the growth of and information propagation through an online social network. It is an agent-based, Kinetic Monte Carlo engine capable of simulating online networks such as Facebook, Twitter, LinkedIn, etc.

`#k@` incorporates all elements of online social networks including multiple user profiles (e.g. standard users, organizations, celebrities, and bots), user messaging, trending topics, and advertising. Agents within the network make decisions (e.g. follow, unfollow, broadcast, and rebroadcast) based on a variety of user defined constraints on signal propagation to model language, region, ideology, musical interests, and humour.

`#k@` allows for simulation of a realistic online social network, enabling users to test hypotheses for growth mechanisms and scenarios for information propagation. As it solves the forward problem, `#k@` can be used with Big Data analytics tools to test data collection protocols and ensure inverse model validity.

`#k@` is a fully cross-platform tool: it works natively on `Windows 10`, `OS X`, and `Linux` platforms.

[Download the latest version of Hashkat](https://github.com/hashkat/hashkat/archive/master.zip)

***

### How do I quickly build `#k@` and run `#k@` simulations ?

Build Requirements:
* `CMake`
* C++ Compiler (for instance: `gcc`/`g++` etc.)

Run Requirements:
* `Python 2.x`

Data Visualization Requirements (optional):
* Gnuplot
* Gephi
* Networkx

***

1. Clone the `hashkat` reositry locally<br>`git clone https://github.com/hashkat/hashkat`
2. Go inside the `hashkat` folder (using `cd hashkat`) after you are sure of all the requirements being fulfilled.
3. Build `hashkat` using `./build.sh -O`
4. Create simulations using the `DEFAULT.yaml` variable values by running the command `./run.sh`

A sample script can be seen as:
```
jigyasa@spin:~/temp$ git clone https://github.com/hashkat/hashkat
...
Checking connectivity... done.
jigyasa@spin:~/temp$ cd hashkat/
jigyasa@spin:~/temp/hashkat$ ./build.sh -O
.....
[100%] Built target hashkat
jigyasa@spin:~/temp/hashkat$ ./run.sh
.....
Simulation Completed: desired simulation time reached
Analysis complete!
```

For elaborate runs, refer [docs.hashkat.org](http://docs.hashkat.org/en/latest/) and for visualizing the output, refer [docs.hashkat.org/visualization](http://docs.hashkat.org/en/latest/visualization/)

***
### Documentation
`#k@` has built-in documentation web pages. Find detailed information, instructions and tutorials at [docs.hashkat.org](http://docs.hashkat.org/en/latest/)
<br>Documentation Status: [![Documentation Status](https://readthedocs.org/projects/hashkat/badge/?version=latest)](https://readthedocs.org/projects/hashkat/?badge=latest)
<br>Check out the [`hashkat/docs`](https://github.com/hashkat/hashkat/tree/master/docs) directory for the source code of the documentation.

***

### Contributing to `#k@`

`#k@` is a community project. We appreciate your enhancement requests as issues and bug-fixes. 
<br>Feel free to open new ones or fix the existing ones ([hashkat/issues](https://github.com/hashkat/hashkat/issues)).

***

### The Road ahead....
`#K@` community welcomes all to be a part of it and help us enhance our product. The [ROADMAP](https://github.com/hashkat/hashkat/blob/master/ROADMAP.md) is a list of things we wish to fulfill in the near future.

***

### LICENSE

`#K@` is a free software: you can redistribute it and/or modify it under the terms of the `GNU General Public License` as published by the `Free Software Foundation`, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the `GNU General Public License` for more details.

You should have received a copy of the [GNU General Public License v3](https://github.com/hashkat/hashkat/blob/master/LICENSE.md) along with this program. If not, see [gnu.org/licenses](http://www.gnu.org/licenses/).

![GNU GPL](https://www.gnu.org/graphics/gplv3-88x31.png)
