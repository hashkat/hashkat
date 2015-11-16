#include "tweets.h"
#include "analyzer.h"

int HashTags::select_agent(AnalysisState& state, bool region_choice, bool ideology_choice, int default_region, int default_ideology) {
    int region_bin = choose_bin(state.rng, region_choice, default_region, state.config.regions.size());
    int ideology_bin = choose_bin(state.rng, ideology_choice, default_ideology, state.config.ideologies.size());
    if (!hashtag_groups[ideology_bin][region_bin].circ_buffer.empty()) {
        int agent_to_follow = hashtag_groups[ideology_bin][region_bin].circ_buffer.pick_random_uniform(state.rng);
        return agent_to_follow;
    }
    return -1;
}

