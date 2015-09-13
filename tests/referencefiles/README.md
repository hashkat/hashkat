These directories each contain an INFILE.yaml file for a particular follow model and several other modifications. These files can be used to ensure that any changes you have made to the source code do or do not affect the output of hashkat.

To test, simply run tests.sh in the hashkat directory. These tests will be run and if there are any discrepancies between what is outputted and what is expected to be outputted, the file(s) in which this discrepancy occurred will be displayed on the screen. These tests correspond to the following:

test01 = agent_simulation_with_changing_rates
test03 = preferential_agent_simulation_with_mexico_and_spanish_added
test04 = random_simulation_with_retweets_constant_agents
test05 = twitter_simulation
test06 = twitter_suggest_simulation_with_retweets_changing_agents
test07 = twitter_suggest_simulation_with_retweets_changing_agents_nobarabasi
test08 = retweet_test_with_tweet_transmission_set_to_0
test09 = retweet_test_with_tweet_transmission_set_to_0.5
test10 = retweet_test_with_tweet_transmission_set_to_1
test11 = same as test10 but with following via retweets disabled

test99 = hashtag_simulation_with_varying_pref_classes
