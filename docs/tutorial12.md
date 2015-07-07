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



## Next Steps

 With the completion of this tutorial, you have knowledge and experience with every different configuration you can implement into #k@. From here, we will move on to a discussion on interactive mode in #k@.
