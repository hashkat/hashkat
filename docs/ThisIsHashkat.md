<center>
<img src='../img/thisishashkat/title_pages_1.png'>
</center>
<center>
<img src='../img/thisishashkat/A_social_network.png'>
</center>

## Graph to Social Network

Social networks are graphs where the nodes are to represent humans,‭ ‬or entities capable of choice‭ (‬agents‭)‬,‭ ‬and the edges are created voluntarily by agents according to the agent’s criterion.‭  

Within the social networks,‭ signals‭ ‬are sent‭, received, and then possibly retransmitted.‭  ‬In the Hashkat model,‭ ‬nodes are called‭ “‬agents‭”‬,‭ ‬edges‭ “‬follows‭”‬,‭ ‬signals “‬tweets‭”‬,‭ ‬and retransmitted signals “‬retweets‭”‬.

Hashkat is a light weight,‭ ‬open source social network generator,‭ ‬which can generate graphs of‭ ‬10,000‭ ‬agents in less than‭ ‬2‭ ‬minutes on an ordinary laptop.

To‭  ‬transform a typical graph into a social network,‭  ‬agents need to be conceived in‭ ‬with‭ “‬human‭” ‬parameters,‭ ‬and methods of agent‭ ‬“choice‭” ‬need‭ ‬to be created.‭  ‬Finally,‭ ‬the fundamental element of humans needs to be built in,‭ ‬diversity.

<center>
<img src='../img/thisishashkat/B_demo_network_agent_follow.png'>
</center>

## Agents

The building block of the social network is the agent.  

<center>
<img src='../img/thisishashkat/C_agents.jpg'>
</center>

Agents are configured with different rates:
*  Add‭ ‬weight:‭  ‬rate the agent is added to network
*  Follow weight:‭  ‬rate agent‭ ‬IS FOLLOWED BY other agents
*  Follow rate:‭  ‬rate agent FOLLOWS other agents‭ 
*  Tweet rate:‭  ‬rate agent generates tweets
*  Tweet_type:‭  ‬proportion of tweets of each type agent generates.‭  ‬The‭ ‬4‭ ‬tweet_types are:‭  ‬plain,‭ ‬humorous,‭ ‬ideological‭ & ‬musical.
*  Followback rate:‭  ‬the rate at which an agent will‭ ‬follow an agent that has followed the agent‭ (‬on Facebook this is‭ ‬100%,‭ ‬all follows are mutual,‭ ‬on Twitter‭ ‬this varies,‭ ‬experimentally shown to be‭ ‬44%‭ ‬for ordinary users‭)‬.
*  Hashtag:‭  ‬care about region/ideology:‭  ‬if an agent follows another based on a‭ ‬#hashtag in their signal,‭ ‬the agent’s choice to follow may depend on tweeter’s region or ideology.‭ 

A‭ ‬sample agent configuration is shown below.‭  

<center>
<img src='../img/thisishashkat/D_agent_code.png'>
</center>

Rates are normalized,‭ ‬so if a standard agent has an add_weight of‭ ‬50‭ ‬and a celebrity agent has an add_weight of‭ ‬7,‭ ‬standard agents will be added with p‭ =  ‬50/‭(‬50+7‭) ‬or‭ ‬87.7%‭ ‬of the time.

The default configuration of Hashkat has two Agent_types pre-configured,‭ ‬Standard and Celebrity.‭  ‬To run a single agent simulation set one add_weight to zero.

The deluxe configuration of Hashkat has seven Agent_types pre-configured:‭ 
*  Standard
*  Celebrity
*  Humorist
*  Ideologue
*  Company
*  Government
*  Bot

Rates for the different Agent_types are pre-set to match our current understanding of each Agent_type’s behavior in Twitter,‭ ‬with the caveat that we do not have full information.

Users may change any and all parts of any agent configuration.‭  

Up to‭ ‬200‭ ‬Agent_types may be configured in the default build of Hashkat,‭ ‬and an unlimited number if the build is reconfigured.

A network is configured in: hashkat\INFILE.yaml = hashkat\DEFAULT.yaml
The build parameters may be changed in:  hashkat\src\config_static.h

## Follow‭ ‬Models (edge creation)

Now that we have agents,‭ ‬how do they‭ “‬chose‭” ‬to follow other agents‭?

<center>
<img src='../img/thisishashkat/E_follow.jpg'>
</center>

Hashkat‭ ‬has‭ ‬6‭ ‬Follow‭ ‬Models‭ ‬that‭ ‬specify‭ ‬how‭ ‬edges‭ ‬are‭ ‬created‭ ‬between‭ ‬agents.‭  ‬Each‭ ‬model‭ ‬has‭ ‬a‭ ‬different‭ ‬basis in graph theory.‭  

The models are:
*  Random‭ (‬Erdos‭ –‬Renyi‭)
*  Preferential Attachment‭ (‬Barabasi-Albert‭)
*  Agent
*  Agent‭ ‬Preferential‭ ‬Attachment
*  Hashtag
*  Combination‭ ‬of‭ ‬above‭ (‬called‭ ‬“Twitter‭”‬ in Hashkat‭)
*  Follow‭ ‬Back

Random: a random choice of whom to follow.

Preferential Attachment: an agent choses to follow the agent‭ ‬in the system‭ ‬with‭ ‬the most‭ ‬followers (highest in-degree).

Agent: an agent_types is preferred to be followed,‭ ‬for example,‭ ‬Celebrities.‭  ‬At present in Hashkat this is set in‭ ‬the‭ ‬'Follow_weight‭'‬ of the agent type,‭ ‬that is,‭ ‬all agent‭_‬types‭ ‬in‭ ‬the‭ ‬system‭ ‬will prefer to follow Celebrities at the same rate.‭ ‬  Within the agent_type,‭ ‬which specific agent is followed is random.

Agent Preferential‭ ‬Attachment: within the agent_type,‭ ‬i.e.‭ ‬Celebrity,‭ ‬the agent will prefer to follow the most popular Celebrity.

Hashtag: ‬a user specified‭ ‬proportion of tweets will have‭ ‬#hashtags which show the region‭ ‬and ideology‭ ‬of the tweeter.‭  ‬If randomly presented with a tweet with a hashtag,‭ ‬if an agent‭ ‬“cares about‭”‬ region or ideology,‭ ‬the agent may‭ ‬chose to follow‭ ‬the tweeter if the tweeter is of the SAME region or ideology as the receiving agent.

Combination: a combination of the above follow models may be chosen by the user for network behavior, to model the fundamental characteristic of human groups:  diversity. 

Follow-back: the choice to follow another agent based on their choice to follow you.  On Facebook, all follows must be mutual (100% follow-back).  On Twitter, experiments have shown varying rates of follow-back, with normal users following back 44% of the time.  In Hashkat, follow-back rate is set by agent_type, i.e. Celebrities have low follow-back rates.

## Tweet (Signal) Generation 

Now that we have nodes(agents) and edges(follows) what do we do with them?

<center>
<img src='../img/thisishashkat/G_tweets.jpg'>
</center>

Social networks exist to exchange signals between agents.    

<center>
<img src='../img/thisishashkat/H_tweet_signal.jpg'>
</center>

In Hashkat Agents may create 4 types of tweets:
*  Plain
*  Ideological
*  Humorous
*  Musical 

Tweet_type affects retweet rates.

## Unfollowing (edge deletion) 

Edge deletion may be enabled in Hashkat. 

In Hashkat, edge deletion (unfollowing) is based on absolute or relative 'chattiness' of an agent, where chattiness = tweet rate. 

Absolute chattiness: if an agent GENERATES tweets in excess of a (user specified) global rate, the generating agent will lose a random follower. 

Relative chattiness:  if an agent is RECEIVING tweets from another agent at double the rate the receiving agent usually receives tweets, the receiving agent will unfollow the chatty agent.  The rate that triggers the unfollow is relative to receiving agent.

## Populations in Regions

<center>
<img src='../img/thisishashkat/H_tweet_signal.jpg'>
</center>

Now that we have Agents (nodes) and Follows (edges) how do we create a more human social simulation? 

Hashkat adds factors of:
*  Language
*  Ideology
*  Retweet preferences 

To agents, then agents are generated within Regions according to specified Region parameters.. 

Let me describe each factor, then I will show how they come together in the creation of Regional Populations.

## Language

<center>
<img src='../img/thisishashkat/L_language.jpg'>
</center>

Hashkat (created in Ontario, Canada, North America) is currently hardcoded with three languages: 
*  English 
*  French 
*  Spanish 

plus one bilingual language:
*  French + English 

In the future we will be changing this to:
*  languages A,B,C 
*  bilingual types AB, BC, AC
*  trilingual type ABC

with user–defined names. 

Tweets are generated in the language of the tweeter.

A receiving agent cannot 'understand' a tweet if it is not generated in a language they speak, unless the tweet_type is 'musical'.

Therefore, an agent will not retransmit a tweet that is not in the same language the receiver speaks.

## Ideology

Ideology in Hashkat is conceived of as any belief and is independent of language, region, agent_type etc.

The default ideologies in Hashkat have color names: red, blue, green & orange.  By default there are 4.

Ideologies may be named by the user, and may be unlimited.

<center>
<img src='../img/thisishashkat/I_ideology.jpg'>
</center>

If an agent generates tweet with a #hashtag, the tweet will contain the ideology flag of the tweeter.

If an agent generates an ideological tweet, the content will be the color of the tweeter. 

Agents may read tweets of any ideology if sent by an agent they follow. 

Agents may chose to retweet signals of the same ideology at a different rate than those of a different ideology. 

## Retweet Preference Classes

Regardless of ideology, language, region or agent type, persons retransmit different content according to their own views of importance. rates.

Personally, I retransmit anything containing a cute mammal, while my uncle retransmits political content.  Clearly I will live longer.

<center>
<img src='../img/thisishashkat/catchill.jpg'>
</center>

Retweet preference classes in Hashkat are intended to mimic this behavior.  

Different retweet rates may be set, by tweet_type and by agent_type, called a preference class.

Different preference classes may be created.  Code for a sample preference class is shown below.

<center>
<img src='../img/thisishashkat/RETWEET_PREF.jpg'>
</center>

The effect of the preference class is to preferentially retweet some types of tweets.

<center>
<img src='../img/thisishashkat/T_retweet.jpg'>
</center>

## Regions

<center>
<img src='../img/thisishashkat/Z_europe.jpg'>
</center>

These three additional characteristics:
*  Language
*  Ideology
*  Preference Classes

are applied to populations in Regions.  

Regions are created, then agents are created within the Regions according to agent_type add_weights AND as citizens of the region with appropriate region characteristics applied

Sample code for region configuration is shown below.

<center>
<img src='../img/thisishashkat/REGION_INFILE.png'>
</center>

Thus, Hashkat ends up with diverse (user-specified) populations to best imitate a social environment.

By making these different and independent parameters, the diversity of Hashkat agents is greatly increased

Agent_types x Languages x Ideologies x Preference Classes.

<center>
<img src='../img/thisishashkat/RP_regions.png'>
</center>

## Program Information

<center>
<img src='../img/thisishashkat/title_pages_19.png'>
</center>

Hashkat may be installed on iOS or Linux/Ubuntu systems. 

Code is open source and available on GitHub as [hashkat](https://github.com/hashkat/hashkat) 

The program is fully documented with explanation & [tutorials](http://docs.hashkat.org/en/latest/)

Hashkat has a C++ engine and Python (v.2.7) configuration INFILE.

<center>
<img src='../img/thisishashkat/HASHKAT.png'>
</center>

## Output

Each simulation produces an Output directory with numerous analysis files (sample shown). 

Visualization files are created for Python (.py) or open source Gephi (.gexf) for agent sizes under 10,000. 

<center>
<img src='../img/thisishashkat/OUTPUT.png'>
</center>

Below is the **main_stats.dat** output file

<center>
<img src='../img/thisishashkat/S_stats_code.png'>
</center>

A sample **retweet_viz.gexf** file is shown, as visualized in **Gephi** open source software.

<center>
<img src='../img/thisishashkat/retweet_viz_gexf.jpg'>
</center>

## KMC

Hashkat uses a Kinetic Monte Carlo engine to do rate generation.

Rates for adding agents, tweeting, retweeting, following, being followed, unfollowing, Hashtag generation, tweet type, region population characteristics (language, ideology, retweet preference classes) are combined into a Rate Spectrum (R). 

At each time increment, a Rate Spectrum is created, a random number is generated, and based on that number, a choice is made. 

The example shows only Add & Tweet Rates. 

<center>
<img src='../img/thisishashkat/KMC.png'>
</center>

## Program Flow

For advanced users, the program flow of Hashkat is as follows:
<center>
<img src='../img/thisishashkat/KMC.png'>
</center>
Hashkat runs very quickly. 10,000 agents may be created within 2 minutes. 
Maximum number of agents, growth rates, and real time limits set in **hashkat/INFILE.yaml** & **hashkat/src/config_static.h**. 
Degree distributions and analysis files produced for every simulation. 
Simulations may be stopped, reconfigured, and restarted. 

## User POV

For the general user, interested in generating a social network:

1.  Hashkat is downloaded and installed in **/hashkat/**

1.  Gephi and other open source filed downloaded and installed per [Installation documentation](http://docs.hashkat.org/en/latest/installation/)

1.  default Hashkat parameters are: ideologies(4), agent_types (200) 

1.  In terminal Hashkat is built with command: **./build.sh**

1.  **hashkat/Output** directory must be discarded 

1.  A default INFILE.yaml is generated **hashkat/INFILE.yaml**

1.  The user desired network is configured in **hashkat/INFILE.yaml **

1.  The simulation is run in terminal with command:  **./run.sh**

1.  **Output** directory with analysis files is generated 

1.  **hashkat/Output** directory saved under a different name or discarded before another simulation can be run.

## Documentation

More information at Hashkat [documentation](http://docs.hashkat.org/en/latest/).
Documentation by Nick Buhagiar & Linda Kettle.




