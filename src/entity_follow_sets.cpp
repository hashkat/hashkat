#include "entity_follow_sets.h"

#include "analyzer.h"

typedef FollowerSet::Context Context;

// Helper functions
static Entity& owner(Context& N) {
    return N.N.network[N.owner_id];
}

static Entity& entity(Context& N, int entity_id) {
    return N.N.network[entity_id];
}

int FollowerSet::LanguageComponent::classify(Context& N, int entity_id) {
    return (int)entity(N, entity_id).language;
}

std::string FollowerSet::LanguageComponent::cat_name(Context& N, int bin) {
    return language_name(bin);
}

// Compute order of distance between tweeter and follower
int FollowerSet::DistanceComponent::classify(Context& N, int entity_id) {
    return 0;
}

std::string FollowerSet::DistanceComponent::cat_name(Context& N, int bin) {
    const double interval = 1.0 / N_BIN_DISTANCE / 2;
    return format("dist: %.2f to %.2f", bin * interval, (bin + 1) * interval);
}

int FollowerSet::PreferenceClassComponent::classify(Context& N, int entity_id) {
    return (int) entity(N, entity_id).preference_class;
}

std::string FollowerSet::PreferenceClassComponent::cat_name(Context& N, int bin) {
    std::vector<EntityPreferenceClass> P = N.N.config.pref_classes;
    if (P.size() <= bin) {
        return "<Unused Slot>";
    }
    return P[bin].name.c_str();
}

FollowerSet::Context::Context(AnalysisState& N, int owner_id) : N(N),
        R(N.config.follower_rates.get_rates(N.network[owner_id])), owner_id(owner_id){
}
