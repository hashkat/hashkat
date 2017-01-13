#include "tweets.h"
#include "analyzer.h"

void Tweet::api_serialize(cereal::JSONOutputArchive& ar) {
    std::string content_type = tweet_type_name(content->type);
    std::string language = language_name(content->language);
    Agent& author = get_state(ar).network[content->id_original_author];

    ar(cereal::make_nvp("id", id_tweet),
       cereal::make_nvp("content_id", content->id),
       cereal::make_nvp("broadcasted_at", creation_time),
       cereal::make_nvp("broadcaster_id", id_tweeter),
       cereal::make_nvp("author_id", content->id_original_author),
       cereal::make_nvp("previous_broadcaster_id", id_link),
       cereal::make_nvp("has_hashtag", hashtag),
       cereal::make_nvp("retweets_since_origin", generation),
       cereal::make_nvp("author_region", get_state(ar).config.regions.regions[author.region_bin].name),
       cereal::make_nvp("author_agent_type", get_state(ar).config.agent_types[author.agent_type].name),
       cereal::make_nvp("author_ideology", get_state(ar).config.ideologies[author.ideology_bin].name),
       NVP(content_type), NVP(language),
       // Twitter API:
       cereal::make_nvp("retweet_count", content->used_agents.size()),
       cereal::make_nvp("author_follower_count", get_state(ar).network[content->id_original_author].follower_set.size()),
       NVP(content_type)
    );
}

int HashTags::select_agent(AnalysisState& state, bool region_choice, bool ideology_choice, int default_region, int default_ideology) {
    int region_bin = choose_bin(state.rng, region_choice, default_region, state.config.regions.size());
    int ideology_bin = choose_bin(state.rng, ideology_choice, default_ideology, state.config.ideologies.size());
    if (!hashtag_groups[ideology_bin][region_bin].circ_buffer.empty()) {
        int agent_to_follow = hashtag_groups[ideology_bin][region_bin].circ_buffer.pick_random_uniform(state.rng);
        return agent_to_follow;
    }
    return -1;
}

