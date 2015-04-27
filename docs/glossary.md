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


