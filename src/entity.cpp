#include "entity.h"

#include "analyzer.h"

int follower_set::LanguageComponent::classify(AnalysisState& N, int entity_id) {
    return (int)N.network[entity_id].language;
}

int follower_set::PreferenceClassComponent::classify(AnalysisState& N, int entity_id) {
    return (int)N.network[entity_id].preference_class;
}
