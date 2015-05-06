[hashkat.org](http://hashkat.org)

<span style="color:black; font-family:Georgia; font-size:1.5em;">May 2015 - This site is currently under construction. Please return regularly over the course of the summer for further updates. </span>

# The Random Follow Model

As the name implies, a random follow model is one in which the entities in which a twitter user follows is totally arbitrary.
You've already had some experience working with the random follow model, the simple network we created in Tutorial 1 implemented it,
but in this tutorial we are going to go into this model in much more detail. This tutorial should take approximately 30 minutes to complete.

Though the simple network we created in Tutorial 1 served as a quick and easy way to introduce one to #k@, the network we created from this
simulation was nowhere near to realistic. *Followback*, the ability for an entity to follow an entity who followed them, was turned off,
entities were prevented from unfollowing anyone, all the entites in Ontario spoke only English, while all the entities in Quebec spoke only
French, the entities were all the same type, and most importantly, none of the entities were able to tweet or retweet. For this lesson,
that is all going to change.

All the files that we use in this simulation can be found in the examples directory in hashkat, under the title *tutorial04*.
You can also view the input file we will be using for this example [here](https://github.com/hashkat/hashkat/blob/master/examples/tutorial04/INFILE.yaml).

As you can see in this input file, we have

![In Degree Distribution](/img/tutorial04/in-degree_distribution_month_000.svg =1x  "In Degree Distribution")

![Out Degree Distribution](/img/tutorial04/out-degree_distribution_month_000.svg =1x  "Out Degree Distribution")

![Cumulative Degree Distribution](/img/tutorial04/cumulative-degree_distribution_month_000.svg =1x  "Cumulative Degree Distribution")

![Visualization](/img/tutorial04/visualization.png =1x  "Visualization")

