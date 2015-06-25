[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# A Diverse Network of Agents

As you have clearly noticed working through these tutorials, #k@ offers the opportunity for users to create network simulations consisiting of a truly dynamic and varied set of agents. A multilingual, multiregional network consisting of agents of many different ideologies, preferences, personalities, and types is one that truly models the human race and is exactly what we strive to do here with #k@. In this tutorial, we will go over every aspect in which you can individualize agents in your network, to give you the freedom and means to create the most realistic network simulations possible.

## Ideology

As previously discussed, an agent's idelogy is the particular dogma that they follow. Ideologies can be added, removed, or renamed in the *ideologies* section of **INFILE.yaml**. The proportion of agents that have each particular ideology in a certain region can be modified in the *ideology_weights* of a particular region in the *regions* section of the input file. Agents can also follow other agents based on ideology in the hashtag follow model, where setting the hashtag follow option *care_about_idelogy* in the agent type section to true results in agents only following other agents that share the same ideology as them. 

## Language

As you would expect, an agent's language is the particular language that they speak. The language of an agent is determined in the *regions* section of **INFILE.yaml** under *language_weights*, where they can be added, removed, or renamed, as well as the proportion of a particular language that agents in that region speak can be weighted against other languages in that region. Agents will never be able to connect with other agents that speak a different language from them, since they are unable to understand one another.

## Region

'Region' outlines the particular geographical area that an agent lives in. *regions* can be named, added, or removed in the *regions* section of **INFILE.yaml**. They're *add_weight* determines the proportion of agents in a network simulation being from that region as opposed to others. They're *preference_class_weights*, *ideology_weights*, and *language_weights*, also weighs the proportion of agents that will have a particular preference class, ideology, or language respectively in the network as opposed to others within the region. 

## Preference Class - Tweet Transmission

An agent's tweet transmission is the probability that an agent will retweet a tweet of a particular type. This is defined in the *preference_classes* section of **INFILE.yaml**, and is therefore weighted in the *preference_class_weights* factor of a region in the *regions* section. The probability that an agent will retweet a tweet of the possible tweet types (plain/musical, different ideology, same ideology, humourous) is determined by its agent type. 

## Preference Class - Follow Reaction Probability

An agent's *follow_reaction_probability

## Agent - Add Weight

An agent's add weight is the proportion in which agents of a particular agent type are added into the network in comparison to those of other types.

## Agent - Follow Weight

An agent's follow weight is the proportion in which agents of a particular agent type are followed with respect to those of other agent types in the *agent* or *preferential_agent* follow models.

## Agent - Tweet Type



## Agent - Followback Probability



## Agent - Hashtag Follow Options



## Agent - Follow Rate



## Agent - Tweet Rate



## Next Steps

 