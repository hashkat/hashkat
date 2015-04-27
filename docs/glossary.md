[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">April 2015 - This site is currently under construction. Please return in the next few months for further updates. </span>

#Glossary

Upon cloning the repository, you will notice there are files called “INFILE.yaml” and “DEFAULT.yaml”; these files allow one to adjust certain parameters to obtain desired results from the simulation, and all of the different parameters is described below.

####analysis
**(type: n/a)** This parameter should never be adjusted, when the software looks for the parameters listed under the analysis heading, it first looks to this variable. This defines to so called analysis section of the input file.

####initial_entities
**(type: integer)** This parameter is the number of initial entities in the simulation. The entity type and characterizations are determined before any time will pass in the simulation.

####max_entities
**(type: integer)** This parameter is the maximum number of entities (users) that will be allowed in the simulation. Once the maximum number of users has been reached in the simulation, the program will no longer add users but will not terminate.

####max_time
**(type: integer)** This parameter is the maximum time allowed in the simulation. The units of time in the simulation is minutes. The simulation will terminate once the maximum amount of time has been reached. 
Conveniently, you can put ‘minute’, ‘hour’, ‘day’, or ‘year’, and the simulation will know the value in minutes.

####max_real_time
**(type: integer)** This parameter is the maximum time allowed in real life. Similarly to the max time parameter, the simulation will terminate once the maximum amount of time has been reached.
Also, you can put ‘minute’, ‘hour’, ‘day’, or ‘year’, and the simulation will know the value in minutes.

####enable_interactive_mode
**(type: boolean)** While the simulation is running, you can press ‘ctrl-c’. If this variable is set to true, an interpreter will be activated and you can call functions in the interpreter.
If you press the ‘tab’ key, you can go through the built in functions. The interpreter was designed to allow one to manipulate the network. If you are in the interpreter, you can press ‘ctrl-c’ and the simulation will resume.
If you type ‘exit()’ while in the interpreter the simulation will terminate.

####use_barabasi
**(type: boolean)** If true, a certain simulation method will be implemented that results in a network that models a scale-free network with a scale-free exponent of 3.
This method is very similar to the ‘barabasi.game’ in the igraph package found in R or Python. The follow model (follow model) in the simulation should be set to ‘preferential’ to achieve desired result.
The simulation method is as follows; an entity is added into the network, and then follows another entity based on the degree of the entities in the current network. A higher degree results in a higher probability of being followed.
If set to false, the simulation will run normally.

####use_random_time_increment
**(type: boolean)** If true, the time in the simulation will be incremented at a non-constant rate; the increment of time is determined by

where u is a random number in the interval 0 < u ≤ 1, and R tot is the sum of the rates for the simulation. On average, the value of − ln(u) is unity, and therefore you can increment time by 1/R tot ; this is how time is incremented if set to false.

####use_followback
**(type: boolean)** If set to true, there may be a follow back prior to a follow. When an entity A follows entity B, if entity B’s followback probability (which can be found in the entities section of the input file) is non-zero,
then entity B can follow entity A without moving forward in time. This is why it is called a ‘flawed followback’; in reality there would be some time prior to the initial follow where the followback would occur.

####follow model
**(type: string)** Currently, there are 6 follow methods implemented in the software; ‘random’, ‘entity’, ‘preferential’, ‘preferential-entity’, ‘hashtag’, and ‘twitter’; to use a follow model simply do

follow_model: random

The ‘random’ follow method causes entities to follow other entities randomly; if the number of entities is set to constant and this method is on, you will achieve an Erd ̈ os-R ́enyi degree distribution (Poisson distribution).
The ‘entity’ follow method causes entities to follow other entities based on title alone; the titles are set in the entities section of the input file. The probabilities to to follow each entity can also be set in the weights portion of the entities section.
The probabilities are normalized, so the probabilities should be set with respect to one another.
The ‘preferential’ follow method follows the preferential attachment model outlined by Barabasi and Albert. The basics behind the method is that the degree of the entity determines the probability of following the entity;
the greater the degree the greater the probability of the entity being followed. The thresholds and weights variables outlined in the follow ranks section determines how the degree of the entities are binned and the corresponding weights for each bin.
If you want to achieve the preferential attachment method similar to the use barabasi method you can set the thresholds to increment by 1,
with linear spacing from 0 to the max number of entities and set the weights to increment by 1 with linear spacing from 1 to the max number of entities.
The ‘preferential-entity’ follow method is just the ‘preferential’ follow method nested in the ‘entity’ follow method. Firstly, a certain entity type will be selected, then based on the degrees of the entities within the certain entity type,
an entity will be selected to follow.
The ‘hashtag’ follow method is a mechanism introduced to follow other entities based on hashtags. If the use hashtag probability parameter is non-zero, then entities will attach hashtags to their tweets.
Depending on the ideology and location of the entity, they are then placed into a dynamic array. If another user wants to follow via hashtag, they look to these dynamic arrays to find a specific entity that relates to
their hashtag preferences. These preferences can be set in the hashtag follow options section.
The ‘twitter’ follow model is a model that incorporates all of the above follow mechanisms. The weights for each mechanism can be set for each follow method. See model weights for more information.

####model_weights
**(type: n/a)** In the model weights, you can set the probability of calling each follow method. You can use arbitrary units to set the weighting for each follow method because the weights are summed and then normalized to find the probability
of calling each method. As an example, if you would like to call the follow methods equally you would set it as so:

model_weights{random: 0.20, preferential: 0.20, entity: 0.20, preferential entity: 0.20, hashtag: 0.20}

####stage1_unfollow
**(type: boolean)** If set to true, entities can be flagged when followed based on their tweet rates. If the tweet rate of the newly followed entity is greater than twice the average tweet rate of the pre-existent entities you follow,
than the entity is placed into an array. This array is looked to when the unfollow function is called. This algorithm is supposed to encapsulate an unfollow method on Twitter.
In Twitter if your feed is being dominated by a user you may become annoyed and want to unfollow that user. This is the process that occurs in this unfollow method.

####unfollow_tweet_rate
**(type: double)** This tweet rate is not associated with the stage1 unfollow method described above. This is a more simplified unfollow algorithm and can be considered the stage 0 unfollow method.
When an entity tweets, we look to see how many tweets they have put out since the entity was created. If the tweet rate of the entity exceeds the value set here, then the entity will be randomly unfollowed by one of its followers.

####use_hashtag_probability
**(type: double)** This is a probability (0.0-1.0) that determines how often entities attach hashtags into their tweets. If set to 0.0, then none of the tweets will have hashtags.
If set to 0.5, then half of the tweets will have hashtags. If 1.0, then all of the tweets will have hashtags.

####rates
**(type: n/a)** This parameter does not need to be changed nor removed. Used to point to the global add rate for the simulation. This defines the global rates part of the input file.

####add
**(type: n/a)** This parameter allows one to adjust the rate at which users are being added into the network. To have different numbers of entities, you can adjust the add weights in the entities section of the input file.

####function
**(type: string)** The function for the add rate can either be constant or linear; other options can be coded into the software if necessary. If function is set to ‘constant’, then you must also declare the constant value by setting the ‘value’
parameter to a real constant > 0. An example of of constant add rate is as follows;

add: {function: constant, value: 1.0}

This will set the add rate to 1 entity per minute. If function is set to ‘linear’, then you must set 2 other variables, the ‘y intercept’ and ‘slope’ variables. An example of a linearly increasing add rate is as follows;

add: {function: linear, y intercept: 1.0, slope: 0.1}

This will create an add user rate of the form

R add = 1.0 + n months · 0.1

where n months is the number of months that have passed in the simulation. The number of months is determined by a constant approximate month of 30 · 60 · 24 minutes. This constant value can be changed in the software to achieve desired result.

####output
**(type: n/a)** This parameter should not be removed, when the simulation has completed, functions that do further analysis on the network can be controlled here, a few built in functions are shown below. This defines the output portion of the input file.

####save_network_on_timeout
**(type: boolean)** If set to true, all of the information of the network is sent into the file declared for the save file when the simulation is terminated.

####load_network_on_startup
**(type: boolean)** If set to true, all of the information of the network that was sent into the file declared for the save file will be loaded from when the simulation begins.

####ignore_load_config_check
**(type: boolean)** If you try to load a network from a save file that does not have the same parameters as the input file and this parameter is set to false, then you will receive a message on the screen and the simulation will not start.
This message will tell you that you must set this parameter to true. Once you do this you can load the saved network and continue simulating that network with different parameters. An example is to generate a random graph and save it.
Then you can load this network with different parameters generating a different network.

####save_file
**(type: string)** This is the file where all of the information of the network will be stored if save network on timeout is set to true. The file name can be anything you like.
Also if load network on startup is set to true, the simulation will look for this file to load the existing network information.

####stdout_basic
**(type: boolean)** If set to true, the number of months in the simulation will be outputted to the screen. Once all of the analysis in complete in the simulation, a message will also be printed to the screen.

####stdout_summary
**(type: boolean)** If set to true, the amount of time, number of entities, number of follows, number of tweets, number of retweets, total rate, and real time spent between each successive output is outputted to the screen.

####visualize
**(type: boolean)** If set to true, the information from the network is outputted to 2 files, “network.dat” and “network.gexf”. The “network.dat” file consists of two columns; the first column is a list the entity ID’s in order, and the second column is the
entity ID’s for who the first column is following. For example if entity ID 1 is following entity ID 13, 14, and 19, then it would be outputted like:

The structure of this file can be referred to as an edge list. This file can easily be read into Python or R to perform any further analysis. The “network.gexf” file is an xml file that can be used to visualize the network in a program called Gephi.

####entity_stats
**(type: boolean)** If set to true, additional calculations will be done to find the relationships between different entity groups and degree distributions for each entity.
For every entity set in the entities section of the input file, there will be a file created with the information mentioned previously. The output files created will take the form “name info.dat” where the name is the name created for the entity.

####degree_distributions
**(type: boolean)** If set to true, the cumulative, in-degree, and out-degree distributions will be outputted to “cumulative distribution.dat”, “in-degree distribution.dat”, and “out-degree distribution.dat”.
The data in the files can be easily plotted with Gnuplot.

####tweet analysis
**(type: boolean)** If set to true, further analysis will be done to create a distribution similar to a degree distribution.
The tweet and retweet distributions come from how often the entities tweet in the network; they tell you the probability that a given entity would have tweeted or retweeted n times, and n is first column in the data files produced.
The files created are titled “tweet distro.dat” and “retweets distro.dat.”

####retweet_visualization
**(type: boolean)** If set to true, there will be a Gephi file called “retweet_viz.gexf” that is produced after the simulation terminates. This file can be found in the ‘output’ directory. The graph produced is a visualization of the most retweeted
tweet in the network.

####main_statistics
**(type: boolean)** If set to true, there will be a text file entitled “main_stats.dat” that is generated after the simulation has finished. This file can be found in the ‘output’ directory. It provides some general statistics for the simulation performed.

####tweet_ranks
**(type: n/a)** This variable must stay in the input file, it allows the code to determine the thresholds for the tweets in the simulation. The entities in the simulation will be categorized in the thresholds provided.
For example if a threshold is 10, then the entities with 10 or less tweets will be stored an a list. Once a entity in this list tweets more than 10 times, the entity will be moved into a bin with a higher threshold.

####retweet ranks
**(type: n/a)** This has the same functionality as the tweet ranks, except it is for retweets. See above description about tweet ranks for more information.

####follow_ranks
**(type: n/a)** This has the same functionality as the tweet and retweet ranks, except it is for the number of followers an entity has. The major difference between the follow
ranks and the tweet/retweet ranks is that the thresholds have weights associated with them for follow ranks. If you adjust these weights correctly, you can achieve the preferential attachment model.
The descriptions for how you can set your thresholds for tweets, retweets, and follows, as well as how to set the weights for follows is below.

####thresholds
**(type: n/a)** The thresholds parameter always take 4 variables; bin spacing, min, max, and increment.

####bin_spacing
**(type: string)** The strings that can be used for the bin spacing variable are linear, quadratic, and cubic; other bin spaces can be implemented if necessary.
If linear, the bins for grouping entities will be spaced based on the increment, if quadratic, the bins for grouping entities will be spaced based on the increment squared, and if cubic, the bins for grouping entities will be spaced based
on the increment cubed.

####min
**(type: integer)** This variable sets the minimum threshold for grouping entities. If the entities have a value less than the minimum threshold, then no binning will occur.

####max
**(type: integer)** This variable sets the maximum threshold for grouping entities. If the entities exceed the maximum value, they will be grouped in a bin where the threshold can not be exceeded.

####increment
**(type: integer)** This variable sets the separation between adjacent thresholds for grouping users. If bin spacing is linear, then the separation between bins is the increment, if quadratic, the separation between bins is the increment
squared, if cubic, the separation between bins is the increment cubed.

An example of linear thresholds is

thresholds: {bin spacing: linear, min: 10, max: 100, increment: 10}

This will create thresholds 10, 20, 30,..., 100; the entities will be grouped according to
these thresholds.

####weights
**(type: n/a)** This follows the same procedure as thresholds. Instead of defining the thresholds for grouping entities, it defines the probability of selecting one of the groups of entities.
