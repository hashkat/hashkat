#include "entity_follow_sets.h"

#include "analyzer.h"

typedef OldFollowerSet::Context Context;

// Helper functions
static Entity& owner(Context& N) {
    return N.N.network[N.owner_id];
}

static Entity& entity(Context& N, int entity_id) {
    return N.N.network[entity_id];
}

int OldFollowerSet::LanguageComponent::classify(Context& N, int entity_id) {
    return (int)entity(N, entity_id).language;
}

std::string OldFollowerSet::LanguageComponent::cat_name(Context& N, int bin) {
    return language_name(bin);
}

// Compute order of distance between tweeter and follower
int OldFollowerSet::DistanceComponent::classify(Context& N, int entity_id) {
    return 0;
}

std::string OldFollowerSet::DistanceComponent::cat_name(Context& N, int bin) {
    const double interval = 1.0 / N_BIN_DISTANCE / 2;
    return format("dist: %.2f to %.2f", bin * interval, (bin + 1) * interval);
}

int OldFollowerSet::PreferenceClassComponent::classify(Context& N, int entity_id) {
    return (int) entity(N, entity_id).preference_class;
}

std::string OldFollowerSet::PreferenceClassComponent::cat_name(Context& N, int bin) {
    std::vector<EntityPreferenceClass> P = N.N.config.pref_classes;
    if (P.size() <= bin) {
        return "<Unused Slot>";
    }
    return P[bin].name.c_str();
}

//OldFollowerSet::Context::Context(AnalysisState& N, int owner_id) : N(N),
//        R(N.config.follower_rates.get_rates(N.network[owner_id])), owner_id(owner_id){
//}
