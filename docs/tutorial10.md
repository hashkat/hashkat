[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">June 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Random Follow Model

A discussion on follow models and an exercise using the random follow model, this tutorial should take approximately 30 minutes to complete.

There are six different follow models that have been implemented into #k@, to allow greater flexibility and control in the networks the project simulates.
The six follow models are:

* Random
* Twitter Preferential
* Entity
* Preferential Entity
* Hashtag
* Twitter

Each of these follow models will be discussed and analyzed in each of their own tutorial, starting with this one, where we study the random follow model.

As the name implies, a random follow model is one in which the entities in which a twitter user follows is totally arbitrary.
You've already had some experience working with the random follow model, the simple network we created in Tutorial 1 implemented it,
but in this tutorial we are going to go into this model in much more detail.

Though the simple network we created in Tutorial 1 served as a quick and easy way to introduce one to #k@, the network we created from this
simulation was nowhere near realistic. Among other things, *Followback*, the ability for an entity to follow an entity who followed them,
was turned off, all the entites in Ontario spoke only English, while all the entities in Quebec spoke only
French, the entities were all the same type, and most importantly, none of the entities were able to tweet or retweet. For this lesson,
that is all going to change.

For reference, all the files that we will use in this simulation can be found in the examples directory in hashkat, under the title *tutorial04*.
You can also view the input file we will be creating for this example [here](https://github.com/hashkat/hashkat/blob/master/examples/tutorial04/INFILE.yaml).

Let's go into the INFILE.yaml file in the hashkat directory and make some changes. Using the INFILE.yaml used in Tutorial 1 as a starting point,
we'll keep the number of entities within the
simulation constant by having 1000 initial and maximum entities. This network we will be creating will be a lot more intricate,
and therefore take up a lot of computuational time. For quickness, we'll lower the maximum simulated time to 1000 simulated minutes.
We'll keep the maximum analysis steps to be unlimited for the most refined simulation, and increase the max real time to 5 minutes
to give the simulation more time to run its course. Interactive mode shall remain off, so both *enable_interactive_mode* and
*enable_lua_hooks* shall be *false* and the lua script will be irrelevant.*use_barabasi* we'll be used next tutorial,
so this shall be kept *false* and the Barabasi connections will therefore also be inconsequential.
The Barabasi exponent will remain at 1 since this gives the most accurate results. *use_random_time_increment* will be set to true
for superior results. *use_followback* will now be set to *true*, and the follow model will remain *random*, with the model weights,
which are only necessary for the twitter follow model, being ignored. We're going to not allow unfollowing to occur in this example,
but you are encouraged to experiment with it when creating your own network simulations, so we're going to set *stage1_unfollow* to false
and the unfollow tweet rate to an exceptionally high number for this simulation like 10,000 tweets.
We have also kept the probability of hashtag presence in tweets to be 50%.

Since the number of entities within this simulation will remain constant, add rate will therefore be 0. The output section of INFILE.yaml
will also be left as is, to produce as many data files for analysis as possible. The tweet and retweets ranks have remained the same,
though to be honest their purpose is just to organize them and has no real impact on the simulation. The follow ranks max threshold
and weight have been changed to the max number of entities and the max number of entities increased by 1 respectively. This as well as
no real impact on this simulation, but this is very important for twitter-preferential simulations, so we just chose to change them now
to their respective values to get you in the habit of changing the follow rank max threshold and weight to the max number of entities
and the max number of entities increased by one respectively. The *tweet_observations*
section of this program will be kept as is for all of these tutorials, though you are encouraged to change it and experiment with it
when running your own simulations. More information on this can be found on the Input page.

The ideologies have remained the same, but the *regions* sections has changed slightly. Instead of having one region only speaking
English and one region only speaking French, we have the weights of English to French speaking entities in Ontario to be 80 to 20,
and in Quebec to be 40 to 60. The Mexico region will not be included in any of these tutorials for simplicity. It is also important to
note that instead of all the regions using the *NoRetweet* preference class, they are all using the *Standard* preference class, with
its tweet transmission for all types of tweets and entities being multiplied by 10 to allow a greater number of retweets.
The *follow_reaction_prob* has remained at 1.

We have also have 4 entity types as opposed to just 1 for this simulation. They are named *Standard*, *Celebrity*, *Bot*,
and *Organization*, with the weighted probablities of an entity belonging to each type being 70 for *Standard* and 10 for each of
*Celebrity*, *Bot*, and *Organization*. The follow weight is 5 for all of these entity types, but this has no impact on this simulation,
only affecting those of which use the entity follow model. Each entity type has differing weights on the type of tweets they create,
but they all have the same *followback_probability* of 0.44. The *hashtag_follow_options*, *care_about_region* and *care_about_ideology*,
have been set to *false* for all the entity types, though this will have no impact on the simulation, only affecting those of which
apply the hashtag follow model. The entity types all have a constant follow rate and tweet rate of 0.01 per simulated minute.

Running this simulation, and analyzing our output, we can see that most
entities have a cumulative-degree distribution of 20, as expected, and we
can see that we have a much more complicated network of entities, with the purple nodes corresponding to the abundant Standard users, and
the different coloured nodes corresponding to the Celebrity, Bot, and Organization type entities, which are all roughly the same quantity.
These two separate networks of entities generated from our simulation again correspond to our English-speaking and French-speaking entities,
with the much more denser network representing the greater number and association of English-speaking entities.

![Cumulative Degree Distribution](/img/tutorial04/cumulative-degree_distribution_month_000.svg =1x  "Cumulative Degree Distribution")

![Visualization](/img/tutorial04/visualization.png =1x  "Visualization")

## Next Steps

With the completion of this tutorial, we have worked with the simplest of the six types of follow models.
Though we went through it in great detail, this example was just the tip of the iceberg of all the random follow model simulations you can
create. Try running your own random follow model simulation different from the one outlined above, and see what you can create.

When ready, move on to the next tutorial, where things get a lot more interesting and a little more complicated with the
preferential follow model.
