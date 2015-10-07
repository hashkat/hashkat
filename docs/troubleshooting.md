[hashkat.org](http://hashkat.org)

# Troubleshooting

With **#k@** being such an interactive program, it is expected that one will receive errors in running their simulations from time to time. 

Here we will discuss some of the error messages and incorrect results you may come across while working with **#k@** and how to find a solution to them.

## Errors

When trying to run **#k@** after configuring the **INFILE.yaml** file, you may be unable to run your network simulation and receive one of the following error messages:

#### Running **#k@** in a different directory

`bash: ./run.sh: No such file or directory`

This error message appears if you run **#k@*** in a directory that doesn't contain the **run.sh** script. As discussed in [Tutorial 2](http://docs.hashkat.org/en/latest/tutorial02/), be sure to input the location of **run.sh** when running the script. 

`../run.sh` 

Tells the computer that **run.sh** is one directory up from your current working directory. 

`/run.sh` 

Tells the computer that **run.sh** is in a subdirectory of your current working directory.

#### Spelling or Missing Term Error

`
Exception occurred while reading 'INFILE.yaml-generated': yaml-cpp: error at line 90, column 17: invalid scalar
terminate called after throwing an instance of 'YAML::InvalidScalar'
  what():  yaml-cpp: error at line 90, column 17: invalid scalar
/home/mobile/hashkat/run.sh: line 80:  4026 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This error message is due to a spelling error that may have been accidentally made in **INFILE.yaml**, such as spelling 'true' or 'false' wrong or forgetting to give a value for a particular variable. We received this error message when we spelled 'false' as 'fAlse' under 'use_barabasi'. Simply fix the spelling mistake and rerun **#k@**.

#### Tab Error

`
terminate called after throwing an instance of 'YAML::BadDereference'
  what():  yaml-cpp: error at line 0, column 0: bad dereference
/home/mobile/hashkat/run.sh: line 80: 22283 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

This is due to accidentally indenting one of the variables using 'Tab' in **INFILE.yaml**. To fix this error, simply move the term back to its proper position in the file.

#### Incorrect Function Formatting

`
Exception occurred while reading 'INFILE.yaml-generated': yaml-cpp: error at line 124, column 8: 'y_intercept' was not found!
terminate called after throwing an instance of 'YAML::RepresentationException'
  what():  yaml-cpp: error at line 124, column 8: 'y_intercept' was not found!
/home/mobile/hashkat/run.sh: line 80:  4312 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

The above error message was shown when we configured a linear function incorrectly.  A similar message will show if there is improper format in the input for at least one of the functions present in **INFILE.yaml**, i.e. missing a parameter.

When using a constant function for your 'add', 'follow', or 'tweet' rates, you must have a 'value' or number for this rate (1 parameter). 

When using a linear function, you must have a 'y_intercept' AND 'slope' for this increasing or decreasing rate (2 parameters).  

Matching your input so that it coincides with the function you are implementing will fix this error.  

Note that if an error message is not thrown, it may be because **#k@** used a default value instead of your incorrect input.

#### Adding Too Many Follow Models, Preference Classes, Agent Types, Regions, or Ideologies

`
Number of ideologies must not be more than N_BIN_IDEOLOGIES (4)! Please adjust src/config_static.h.
terminate called after throwing an instance of 'char const*'
/home/mobile/hashkat/run.sh: line 92: 12264 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

You may receive a similar error if you attempt to configure more follow models, preference classes, agent types, regions, or ideologies in **INFILE.yaml** than have been built in the default **#k@** build.  In the above case, we have added too many ideologies. 

To fix this, you may either remove the excess entries from the **INFILE.yaml**, or rebuild **#k@** to permit the number you want.  

To rebuild **#k@** with a different number of, for example, ideologies, go to

`~/hashkat/src/config_static.h`

Look for **N_BIN** files such as

    N_BIN_PREFERENCE_CLASS = 2;
    N_BIN_AGENT_TYPES = 200;
    N_BIN_REGIONS = 3;
    N_BIN_IDEOLOGIES = 4;

These are the default values in the **#k@** build.  Revise the defaults to the values you want.

Then rebuild **#k@** by running **build.sh**. 

Note:  the number of regions in your network must EXACTLY match the quantity specified in 'config_static.h', so be sure to modify your 'INFILE.yaml' or the **N_BIN** in 'config_static.h' so they match.

After **#k@** is rebuilt, the particular 'ideology' may be configured in **INFILE.yaml**.

#### Not Enough Memory

`
terminate called after throwing an instance of 'std::bad_alloc'
  what():  std::bad_alloc
/home/mobile/hashkat/run.sh: line 80:  6088 Aborted                 (core dumped) "$HASHKAT/build/src/hashkat" $args
`

Simulations take memory, particularly with many agents, 'agent types' or over a long allocated time.  The above error message will appear if you exceed your available memory when running a network simulation. 

If this error message or one similar appears, it is recommended that you lower the number of agents in your network simulation by reducing the value of the 'initial_agents' and/or 'max_agents' in **INFILE.yaml** file.

## Unexpected Network Produced

Sometimes the simulation may run smoothly, but the network you've created is quite different from what you had in mind.  The following discusses some of the configuration errors that you may encounter when using **#k@**.

#### Twitter_Suggest Follow Model Network Not Properly Constructed

You may notice that when visualizing a 'twitter_suggest' follow model simulation that there seems to be no preferential attachment present in the network. The connections between agents seem to be random, and there is no clear distinction between the most popular and least popular agents. 

It is imperative in any preferential attachment model network simulation that the 'max follow rank threshold' be equal to or greater than the number of agents in the simulation and that the 'follow rank weight' is 1 for the 'minimum follow rank' of zero and that the 'max follow rank weight' is equal to the 'max follow rank threshold' + 1 (assuming the follow rank bins are spaced linearly in increments of 1). 

This allows agents with more followers to have a better chance of being followed than less popular agents. If the follow ranks are weighted incorrectly, more popular agents may not have a higher probability of being followed. If the 'follow rank max threshold' is a number less than the total number of agents in the simulation, then agents with a differing number of followers may eventually have an equal weight of being followed because they have reached the 'max follow rank threshold'. Ensuring that these follow ranks values are correct is essential for running an accurate preferential attachment follow model simulation.

The following visualization is that of an incorrect simulation we ran using the 'twitter_suggest' follow model:

<center>
<img src='../img/troubleshooting/incorrect_twitter_suggest.png'>
</center>

This network resembled a 'random' follow model simulation as opposed to a 'twitter_suggest' simulation. 

Looking in **INFILE.yaml**, we discovered that the 'follow rank max threshold' and 'weight' were much smaller than the total number of agents in the simulation. Changing these values to their correct amount, we ran the network simulation again and produced the following visualization:

<center>
<img src='../img/troubleshooting/correct_twitter_suggest.png'>
</center>

As you can see, this is now a preferential attachment follow model network, with the most popular agents in the centre of the visualization, and the less popular ones at the sides. 

#### No Retweets Present in Network When They Should Be

<center>
<img src='../img/troubleshooting/no_retweets_present.png'>
</center>

After running a network simulation that enables retweeting, you may find that there are no retweets actually present in the simulation at its completion. This could be due to agents having a 'tweet rate' of zero.  It could also be due to an error in the 'tweet observation probability density function' or decay rate, where perhaps tweets aren't relevant for a long enough time to be retweeted. The 'NoRetweetPref' could also be enabled instead of a preference class that allows retweeting, and/or the preference class that is enabled has a too small 'tweet_transmission' rate, leading to very few or no tweets being retweeted. 

Take a look over all these factors and run the network simulation again. 

If you find that there are still no retweets occurring in the network, increase the 'tweet_transmission' rate, 'tweet' rate, and maximum simulated and/or maximum real time to allow agents ample opportunity to retweet. 

#### Implementing *use_barabasi* For the Wrong Follow Model

A Barabasi configuration is usually only enabled for follow models that invoke preferential attachment, such as the 'twitter_suggest', 'preferential_agent', and the combination 'twitter' follow model. You can use the Barabasi configuration for other follow models, but it will impact your network and give you different results.

#### Simulation Running With an Unintended Variable Value

You may find your simulation ran with a value you did not expect.  When you check **INFILE.yaml** you discover you were missing a particular variable or its value yet the simulation ran. When a variable or its value are missing from **INFILE.yaml**, the default values will be used in the simulation.  The default values can be found here:

`~/hashkat/DEFAULT.yaml`

**DEFAULT.yaml** can also be found [here](https://github.com/hashkat/hashkat/blob/master/DEFAULT.yaml).

## Reporting an Issue

To report an issue, go to [Feedback](http://docs.hashkat.org/en/latest/feedback/). There you will find our email address as well as a link to a webpage where you can post any issues you may have. These issues will be addressed and remedied as soon as possible. 
