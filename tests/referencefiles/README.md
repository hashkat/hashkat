These directories each contain an INFILE.yaml file for a particular follow model and several other modifications. These files can be used to ensure that any changes you have made to the source code do or do not affect the output of hashkat.

To test, simply enter a directory, run the network simulation, and then run verify.py. Doing so will display every output file with either "True" or "False" beside it.

"True" means the file you have outputted from your simulation is exactly the same as the reference file, the output has not changed. "False" means that there is a difference between the file you have outputted and its corresponding output file, signifying a change in the output of hashkat.


test01 = agent_simulation_with_changing_rates
test02 = hashtag_simulation_with_varying_pref_classes
test03 = preferential_agent_simulation_with_mexico_and_spanish_added
test04 = random_simulation_with_retweets_constant_agents
test05 = twitter_simulation
test06 = twitter_suggest_simulation_with_retweets_changing_agents
test07 = twitter_suggest_simulation_with_retweets_changing_agents_nobarabasi
