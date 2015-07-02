#!/bin/bash
set -e
./run.sh --tests $@

./tests/referencefiles/agent_simulation_with_changing_rates/run.sh

./tests/referencefiles/agent_simulation_with_changing_rates/verify.py


./tests/referencefiles/hashtag_simulation_with_varying_pref_classes/run.sh

./tests/referencefiles/hashtag_simulation_with_varying_pref_classes/verify.py


./tests/referencefiles/preferential_agent_simulation_with_mexico_and_spanish_added/run.sh

./tests/referencefiles/preferential_agent_simulation_with_mexico_and_spanish_added/verify.py


./tests/referencefiles/random_simulation_with_retweets_constant_agents/run.sh

./tests/referencefiles/random_simulation_with_retweets_constant_agents/verify.py


./tests/referencefiles/twitter_simulation/run.sh

./tests/referencefiles/twitter_simulation/verify.py


./tests/referencefiles/twitter_suggest_simulation_with_retweets_changing_agents/run.sh

./tests/referencefiles/twitter_suggest_simulation_with_retweets_changing_agents/verify.py


./tests/referencefiles/twitter_suggest_simulation_with_retweets_changing_agents_nobarabasi/run.sh

./tests/referencefiles/twitter_suggest_simulation_with_retweets_changing_agents_nobarabasi/verify.py

