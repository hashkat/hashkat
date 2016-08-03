#include "agent.h"
#include "serialization.h"
#include "analyzer.h"

void Agent::api_serialize(cereal::JSONOutputArchive& ar, bool serialize_follow_sets) {
    std::string language = language_name(this->language);

    ar(NVP(id),
       cereal::make_nvp("agent_type", get_state(ar).config.agent_types[agent_type].name),
       cereal::make_nvp("preference_class", get_state(ar).config.pref_classes[preference_class].name),
       NVP(n_tweets), NVP(n_retweets),
       cereal::make_nvp("region", get_state(ar).config.regions.regions[region_bin].name),
       NVP(ideology_tweet_percent), NVP(creation_time),
       NVP(avg_chatiness), 
       NVP(language),
       cereal::make_nvp("chatty_agent_ids", chatty_agents),
       cereal::make_nvp("ideology", get_state(ar).config.ideologies[ideology_bin].name));
 
    if (serialize_follow_sets) {
        ar(NVP(following_set), NVP(follower_set));
    }
    ar (
       // following_set and follower_set left out for brevity. 
       NVP(following_method_counts), NVP(follower_method_counts),
       // Convenience members:
       cereal::make_nvp("n_followings", following_set.size()),
       cereal::make_nvp("n_followers", follower_set.size())
    );
}


