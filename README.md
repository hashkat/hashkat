--- #KAT ---   Kinetic analysis of Twitter
===

This is open-source software for generating kinetic complex networks. The events that occur in the simulations are designed to mimic the events that occur in Twitter. These events include following, tweeting, retweeting, unfollowing, and entity creation. An entity in our similations represent users in the online social network of Twitter. 

         -----------------------------------------
        |  NOTE: you must have cmake, and pyyaml |
        |                                        |
        |    TO BEGIN SIMULATION: './run.sh'     |
         ----------------------------------------

Once the simulation begins, you will see the following output to the screen...





Starting simulation with seed '1'.

Number of Months = 0


Time		Users		Follows		Tweets		Retweets	Unfollows	R	Real Time Spent

0.15		101		0		0		0(0)	0		1.20		60.73ms	
600.94		712		219		180		8(19)	35		2.35		361.05ms	
926.55		1066		523		533		58(93)	105		3.79		475.45ms	
1138.89		1350		837		874		180(188)	196		5.53		486.18ms	
1286.79		1573		1154		1264		328(303)	285		7.23		500.89ms	
1399.80		1774		1457		1667		512(465)	384		9.53		555.75ms	
1488.59		1933		1733		2064		774(640)	461		12.61		545.74ms	
1556.92		2066		2011		2441		1075(843)	540		16.07		597.97ms	
1614.11		2195		2249		2816		1431(1069)	612		19.76		614.21ms	
1660.44		2295		2545		3146		1786(1293)	699		22.86		767.08ms	
1701.52		2424		2775		3522		2153(1549)	757		26.47		699.89ms	
1738.30		2534		3023		3878		2539(1790)	837		30.07		895.47ms	
1769.84		2642		3261		4172		2968(2035)	901		33.66		805.37ms	
1798.25		2729		3505		4503		3393(2297)	969		38.35		889.64ms	
^CWelcome to Simulator Interactive Mode.
Type exit() or quit() to (gracefully) finish the simulation.
> 


Afterpressing ctrl-c, the simulation (temporarily) terminated into the interpreter mode. For further statistics on the network formed, you can simply type 'exit()'. To continue the simulation, press 'ctrl-c' once again. As you can see from the periodic output, the simulation terminated with only 2729 users. All of the statistical printed to the screen can be found in the file 'DATA_vs_TIME'.  
