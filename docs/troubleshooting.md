[hashkat.org](http://hashkat.org)

# Troubleshooting

With ***#k@*** being such an interactive program, it is expected that one will receive errors in running their simulations from time to time. Here we will discuss some of the error messages and errors you may come across while working with ***#k@*** and how to find a solution to them.

## Error Messages

When trying to run ***#k@*** after configuring the *INFILE.yaml* file, you may be unable to run your network simulation and recieve one of the following error messages:

### Running ***#k@*** in a different directory

`bash: ./run.sh: No such file or directory`

This error message appears if you run ***#k@*** in a directory that doesn't contain the *run.sh* script. As discussed in [**Tutorial 2**](http://docs.hashkat.org/en/latest/tutorial02/), be sure to input the location of *run.sh* when running the script. Typing in '../' prior to 'run.sh' in the command line tells the computer that *run.sh* is one directory up from your current working directory. Typing in a directory name and a '/' prior to 'run.sh' in the command line tells the computer that *run.sh* is in a subdirectory of your current working directory.

### Spelling or Missing Term Error

`
Exception occurred while reading 'INFILE.yaml-generated': yaml-cpp: error at line 90, column 17: invalid scalar
terminate called after throwing an instance of 'YAML::InvalidScalar'
  what():  yaml-cpp: error at line 90, column 17: invalid scalar
/home/mobile/hashkat/run.sh: line 80:  4026 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This error message is due to a spelling error that may have been accidentally made in *INFILE.yaml*, such as spelling *true* or *false* wrong or forgetting to give a value for a particular variable. We received this error message when we spelt *false* as *fAlse* under *use_barabasi*. This error can be simply remedied by fixing the spelling mistake and rerunning ***#k@***.

### Indentation Error

You may receive a long error message ending with something similar to the following:

`
terminate called after throwing an instance of 'YAML::BadDereference'
  what():  yaml-cpp: error at line 0, column 0: bad dereference
/home/mobile/hashkat/run.sh: line 80: 22283 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This is due to accidentally indenting one of the variables using **Tab** in *INFILE.yaml*. To fix this error, simply move the term back to its proper position in the file.

### Incorrect Function Formatting

`
Exception occurred while reading 'INFILE.yaml-generated': yaml-cpp: error at line 124, column 8: 'y_intercept' was not found!
terminate called after throwing an instance of 'YAML::RepresentationException'
  what():  yaml-cpp: error at line 124, column 8: 'y_intercept' was not found!
/home/mobile/hashkat/run.sh: line 80:  4312 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This error message is due to improper formatting for at least one of the functions present in your *INFILE.yaml* file. When using a constant function for your add rate, follow rate, or tweet rate, you must have a *value* for this constant rate. When using a linear function, you must have a *y_intercept* and *slope* for this increasing or decreasing rate. If these corresponding variables are not present for each particular function, then you will receive an error similar to the one above, which was caused by having a linear add rate with a corresponding *value* instead of a *y_intercept* and *slope*. Simply changing your function configuration so that it coincides with the function you are implementing will fix this error.

### Adding Too Many Follow Models, Preference Classes, Agent Types, Regions, or Ideologies

`
Number of ideologies must not be more than N_BIN_IDEOLOGIES (4)! Please adjust src/config_static.h.
terminate called after throwing an instance of 'char const*'
/home/mobile/hashkat/run.sh: line 92: 12264 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

You may recieve an error similar to the one above for adding too many follow models, preference classes, agent types, regions, or ideologies into your simulation (in the above case, we have added too many ideologies). If you recieve this error, you may either remove the additional follow model, preference class, agent type, region, or ideology from the *INFILE.yaml* file, or increase the value of the quantity of the parameter in question in the *config_static.h* file in *src* and rebuild ***#k@*** by running *build.sh*. It is important to note that you will also receive an error if the number of regions in your network does not EXACTLY match the quantity specified in *config_static.h*, so be sure to modify your input file or *config_static.h* file accordingly and rebuild ***#k@*** if this situation arises.

### Not Having Enough Memory

`
terminate called after throwing an instance of 'std::bad_alloc'
  what():  std::bad_alloc
/home/mobile/hashkat/run.sh: line 80:  6088 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

You may be unable to run certain large network simulations due to your computer not having enough memory to run a simulation of a network of such size. This will be evident by the above error message appearing when you try to run a network simulation. If this error message or one similar to it ever appears, it is recommended that you lower the number of agents in your network simulation by reducing the value of the *initial_agents* and/or *max_agents* in the *INFILE.yaml* file.

## Unexpected Network Produced

Sometimes when running a network simulation, the simulation may run smoothly, but the network you've created is quite different from what you had in mind. This is usually due to an error being made in *INFILE.yaml* that does not prevent the simulation from running, but gives you odd results. The following discusses some of the errors that you may encounter when using ***#k@***, and what the root cause of that problem is.

### Twitter_Suggest Follow Model Network Not Constructed As Such

You may notice that when visualizing a *twitter_suggest* follow model network simulation, there seems to be no preferential attachment present in the network. The connections between agents seem to be random, and there is no clear distinction between the most popular and least popular agents. This is most likely due to an error in the follow ranks section of *INFILE.yaml*. It is imperative in any preferential attachment model network simulation that the max follow rank threshold be equal to or greater than the number of agents in the simulation and that the follow rank weight is 1 for the minimum follow rank of zero and that the max follow rank weight is equal to the max follow rank threshold increased by one (assuming the follow rank bins are spaced linearly in increments of 1). This allows agents with more followers to have a better chance of being followed than less popular agents. If the follow ranks are weighted incorrectly, more popular agents may not have a higher probability of being followed. If the follow rank max threshold is a number less than the total number of agents in the simulation, than agents with a differing number of followers may eventually have an equal weight of being followed because they have reached the max follow rank threshold. Ensuring that these follow ranks values are correct is essential for running an accurate preferential attachment follow model simulation.

The following visualization is that of a network we ran using the *twitter_suggest* follow model:

<center>
<img src='../img/troubleshooting/incorrect_twitter_suggest.png'>
</center>

As you can clearly see, this network clearly resembled a *random* follow model simulation as opposed to a *twitter_suggest* follow model simulation. Looking into the *INFILE.yaml* file, we discovered that the follow rank max threshold and weight were much smaller than the total number of agents in the simulation. Changing these values to their correct amount, we ran the network simulation again and produced the following visualization:

<center>
<img src='../img/troubleshooting/correct_twitter_suggest.png'>
</center>

As you can see, this is clearly now a preferential attachment follow model network, with the most popular agents in the centre of the visualization, and the less popular ones at the sides. 

### No Retweets Present in Network When They Should Be

<center>
<img src='../img/troubleshooting/no_retweets_present.png'>
</center>

Running a network simulation that enables retweeting, you may find that there are no retweets actually present in the simulation at its completion. This could be due to agents having a tweet rate of zero, since if there are no tweets in the simulation, there can't possibly be any retweets. This could also be due to an error in the tweet observation probability density function, where perhaps tweets aren't relevant for a long enough time to be retweeted. The *NoRetweetPref* could also be enabled instead of a preference class that allows retweeting, and/or the preference class that is enabled has miniscule tweet transmission rates, leading to very few or no tweets being retweeted. Take a look over all these factors, and ensure that they are set properly before running the network simulation again. If you find that there are still no retweets occurring in the network, increase the tweet transmission rates, tweet rates, maximum simulated time, and/or maximum real time variables to allow agents ample opportunity to retweet tweets. 

### Implementing *use_barabasi* For the Wrong Follow Model

Though a Barabasi configuration can be implemented into every follow model network simulation, it is usually only enabled for follow models that invoke preferential attachment, such as the *twitter_suggest*, *preferential_agent*, and possibly the *twitter* folow model. You can use the Barabasi configuration for other follow models if you so please, but keep in mind the impact it will have on your network, and how different your results may be for a network simulation that does not implement this configuration.

### Simulation Running Even Despite Missing Variable

You may find when checking your *INFILE.yaml* file after running a network simulation that you were missing a particular variable and its corresponding value in your input file. However, this seems to have not stopped your simulation from running. The reason for this is that whenever a variable and its corresponding value are missing from *INFILE.yaml*, the value assigned to that variable in *DEFAULT.yaml* will be used in the network simulation in its place. THE *DEFAULT.yaml* file can of course be found in the **hashkat** directory as well as [here](https://github.com/hashkat/hashkat/blob/master/DEFAULT.yaml).

## Reporting an Issue

Check out our [**Feedback**](http://docs.hashkat.org/en/latest/feedback/) page if you have any issues using ***#k@*** that are not discussed above. There you can find our email address where you can contact us as well as a link to a webpage where you can post any issues you may have. These issues will be addressed and remedied as soon as possible. 