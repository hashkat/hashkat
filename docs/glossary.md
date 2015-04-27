[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

#Glossary

Upon cloning the repository, you will notice there are files called “INFILE.yaml” and “DEFAULT.yaml”; these files allow one to adjust certain parameters to obtain desired results from the simulation, and all of the different parameters is described below.

analysis: (type: n/a) This parameter should never be adjusted, when the software looks for the parameters listed under the analysis heading, it first looks to this variable. This defines to so called analysis section of the input file.

initial entities: (type: integer) This parameter is the number of initial entities in the simulation. The entity type and characterizations are determined before any time will pass in the simulation.

max entities: (type: integer) This parameter is the maximum number of entities (users) that will be allowed in the simulation. Once the maximum number of users has been reached in the simulation, the program will no longer add users but will not terminate.

max time: (type: integer) This parameter is the maximum time allowed in the simulation. The units of time in the simulation is minutes. The simulation will terminate once the maximum amount of time has been reached. Conveniently, you can put ‘minute’, ‘hour’, ‘day’, or ‘year’, and the simulation will know the value in minutes.

max real time: (type: integer) This parameter is the maximum time allowed in real life. Similarly to the max time parameter, the simulation will terminate once the maximum amount of time has been reached. Also, you can put ‘minute’, ‘hour’, ‘day’, or ‘year’, and the simulation will know the value in minutes.

enable interactive mode: (type: boolean) While the simulation is running, you can press ‘ctrl-c’. If this variable is set to true, an interpreter will be activated and you can call functions in the interpreter. If you press the ‘tab’ key, you can go through the built in functions. The interpreter was designed to allow one to manipulate the network. If you are in the interpreter, you can press ‘ctrl-c’ and the simulation will resume. If you type ‘exit()’ while in the interpreter the simulation will terminate.

use barabasi: (type: boolean) If true, a certain simulation method will be implemented that results in a network that models a scale-free network with a scale-free exponent of 3. This method is very similar to the ‘barabasi.game’ in the igraph package found in R or Python. The follow model (follow model) in the simulation should be set to ‘preferential’ to achieve desired result. The simulation method is as follows; an entity is added into the network, and then follows another entity based on the degree of the entities in the current network. A higher degree results in a higher probability of being followed. If set to false, the simulation will run normally.

use random time increment: (type: boolean) If true, the time in the simulation will be incremented at a non-constant rate; the increment of time is determined by

where u is a random number in the interval 0 < u ≤ 1, and R tot is the sum of the rates for the simulation. On average, the value of − ln(u) is unity, and therefore you can increment time by 1/R tot ; this is how time is incremented if set to false.

use followback: (type: boolean) If set to true, there may be a follow back prior to a follow. When an entity A follows entity B, if entity B’s followback probability (which can be found in the entities section of the input file) is non-zero, then entity B can follow entity A without moving forward in time. This is why it is called a ‘flawed followback’; in reality there would be some time prior to the initial follow where the followback would occur.
