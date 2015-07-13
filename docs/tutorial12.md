[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">July 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# A Diverse Network of Agents

As you have clearly noticed working through these tutorials, #k@ offers the opportunity for users to create network simulations consisiting of a truly dynamic and varied set of agents. A multilingual, multiregional network consisting of agents of many different ideologies, preferences, personalities, and types is one that truly models the human race and is exactly what we strive to do here with #k@.

In this tutorial, we will go over every aspect in which you can individualize agents in your network, to give you the freedom and means to create the most realistic network simulations possible. We will also use what we learn in this tutorial to simulate a diverse network of several different agents and see which agent type ends up being the most popular. This tutorial should take about 45 minutes to complete.

## What Makes an Agent Unique?

### Ideology

As previously discussed, an agent's idelogy is the particular dogma that they follow. Ideologies can be added, removed, or renamed in the *ideologies* section of **INFILE.yaml**. The proportion of agents that have each particular ideology in a certain region can be modified in the *ideology_weights* of a particular region in the *regions* section of the input file. Agents can also follow other agents based on ideology in the hashtag follow model, where setting the hashtag follow option *care_about_idelogy* in the agent type section to true results in agents only following other agents that share the same ideology as them. 

### Language

As you would expect, an agent's language is the particular language that they speak. The language of an agent is determined in the *regions* section of **INFILE.yaml** under *language_weights*, where they can be added, removed, or renamed, as well as the proportion of a particular language that agents in that region speak can be weighted against other languages in that region. Agents will never be able to connect with other agents that speak a different language from them, since they are unable to understand one another.

### Region

'Region' outlines the particular geographical area that an agent lives in. *regions* can be named, added, or removed in the *regions* section of **INFILE.yaml**. They're *add_weight* determines the proportion of agents in a network simulation being from that region as opposed to others. They're *preference_class_weights*, *ideology_weights*, and *language_weights*, also weighs the proportion of agents that will have a particular preference class, ideology, or language respectively in the network as opposed to others within the region. 

### Preference Class - Tweet Transmission

An agent's tweet transmission is the probability that an agent will retweet a tweet of a particular type. This is defined in the *preference_classes* section of **INFILE.yaml**, and is therefore weighted in the *preference_class_weights* factor of a region in the *regions* section. The probability that an agent will retweet a tweet of the possible tweet types (plain/musical, different ideology, same ideology, humourous) is determined by its agent type. 

### Preference Class - Follow Reaction Probability

An agent's *follow_reaction_probability* is the probability that an agent, after seeing a tweet be retweeted by someone they follow, will follow the speaker of the original tweet as opposed to just retweeting the tweet.

### Agent - Add Weight

Defined in the *agents* section of **INFILE.yaml**, an agent's add weight is the proportion in which agents of a particular agent type are added into the network in comparison to those of other types.

### Agent - Follow Weight

Defined in the *agents* section of **INFILE.yaml**, an agent's follow weight is the proportion in which agents of a particular agent type are followed with respect to those of other agent types in the *agent* or *preferential_agent* follow models.

### Agent - Tweet Type

Defined in the *agents* section of **INFILE.yaml**, *tweet_type* outlines the weight in which agents of a particular agent type make tweets categorized into *ideological*, *plain*, *musical*, or *humourous* based on their content. 

### Agent - Followback Probability

Defined in the *agents* section of **INFILE.yaml**, an agent's *followback_probability* is the probability in which agents of a particular agent type follow other agents that have followed them. This value has been found to be about 44% or 0.44 in experiment. In order for *followback_probability* to be enabled, *use_followback* must be set to *true* in the *section* of **INFILE.yaml**.

### Agent - Hashtag Follow Options

Defined in the *agents* section of **INFILE.yaml**, the *hashtag_follow_options* determine who agents follow in the *hashtag* follow model. Whether they follow an another agent only if they share the same region and/or ideology as them is based on whether or not you put *true* for the *care_about_region* and/or *care_about_ideology* variable respectively.

### Agent - Follow Rate

Defined in the *agents* section of **INFILE.yaml**, an agent's follow rate dictates the amount of follows an agent of that particular agent type does per simulated minute. The function of this rate can be either constant or linear. For a constant follow rate, the function must be set to *constant* and it must have a value of 0 or greater assigned to it. For a linear follow rate, the function must be set to *linear* and there must be a *y_intercept* and *slope* instead of a *value*.

### Agent - Tweet Rate

Defined in the *agents* section of **INFILE.yaml**, an agent's tweet rate dictates the amount of tweets an agent makes of the particular agent type per ismulated minute. The function of this rate can be either constant or linear. For a constant tweet rate, the function must be set to *constant* and it must have a value of 0 or greater assigned to it. For a linear tweet rate, the function must be set to *linear* and there must be a *y_intercept* and *slope* instead of a *value*.

## Popularity Contest

Now that we have seen all the factors that make agents unique, we are going to create a network simulation that consists of these diverse agents, and see which agent type out of the ones we created are the most popular.

For this exercise, we will actually not go through how to configure **INFILE.yaml** to create the desired network with you. You've had enough experience with it that you should be able to do this on your own. We're just going to outline the network that we would like. If you're having any problems with this, the desired input file can be found for reference in the tutorials directory in hashkat, with this one under the title *tutorial12*. You can also view the desired input file you will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/tutorials/tutorial12/INFILE.yaml). Now, on to creating the network.

For this network simulation, we would like to have the following:

* a constant number of 1000 agents
* a maximum simulation time of 1000 simulated minutes
* a maximum real time of 30 minutes
* followback implemented
* the twitter follow model implemented, with the twitter_suggest and the hashtag follow model evenly weighted and the other follow models being excluded from this simulation
* No unfollowing allowed
* Hashtag presence in tweets 50% of the time
* 4 ideologies: 'Red', 'Blue', 'Green', and 'Orange'
* agents being located in either Ontario or Quebec, with the ratio of Ontarians to Quebecers being 3:2
* Ontarians having a *NoRetweetPref* preference class, with 4 evenly wieghted ideologies and the ratio of English to French speaking Ontarians being 1:1
* Quebecers having the *StandardPref* preference class, with only 3 ideologies ('Red', 'Blue', 'Green') evenly weighted, with the ratio of English speaking to French speaking Quebecers being 1:1
* for the *StandardPref* preference class, the tweet transmission rates for the agent types present in this network are 0.1 for 'plain'/'musical' tweets, 0.0 for 'different_ideology' tweets, 0.2 'same_ideology' tweets, and 0.4 for 'humourous' tweets, with the 'follow_reaction_prob' being 0.3
* the *NoRetweetPref* preference class will remain the same, with 0 for all

We would also like you to use only the following agent types with their respective characteristics:



Make sure that your tweet, retweet, and follow ranks have also been updated accordingly to properly account for this network. As you've probably noticed, we've set the max real time to 30 minutes instead of the regular 1 minute. The simulation should definitely not run for this long, we just want to allow enough time for the simulation to run to completion. When running this simulation on our computer, it took around 16 minutes real time to complete due to the complexity of the network. You do not have to run your simulation for this long if you wish. You can change the maximum real time to a time of your choosing or press **Ctrl-c** at any time during the simulation to pause it. Once you have configured **INFILE.yaml** to account for all this, run and visualize your network simulation.

Running the network simulation to completion, we produced the following network visualization:

<p align='center'>
<img src='../img/tutorial12/visualization.png'>
</p>

In the above visualization, the green nodes correspond to the 'Standard-Chatty' agents, the purple nodes correspond to the 'Standard-Friendly' agents, the blue nodes correspond to the 'Standard-Musical' agents, the yellow nodes correspond to 'Standard-Funny' agents, and the red nodes correspond to the 'Standard-Political' agents. As we can see, our social network has been divided into networks based on language, with one network corresponding to English-speaking agents and the other corresponding to French-speaking agents. Judging from this visualization, there seems to be no dominant agent type based on connectivity. The 'Standard-Chatty' agents and 'Standard-Musical agents' seems to be slightly less popular, as evident by their more abundant presence on the sides of each network simulation, while the 'Standard-Friendly', 'Standard-Funny', and 'Standard-Political' agents seem to be slightly more popular due to there more ample presence near the centre of the networks.

In the end, there really seems to be no true 'most popular' agent type. In a social network of agents with such a variety of interests and personalities, it can be expected that every agent type has something that draws other agents into following them. In this social network we have simulated, there are agents of every type with similar attributes that are both very popular and not as much.

## Next Steps

 With the completion of this tutorial, you now have knowledge and experience with every different agent configuration you can implement into #k@. From here, we will move on to a discussion on interactive mode in #k@, where you can make changes to your network mid-simulation.