#include "entity.h"

#include "analyzer.h"

int follower_set::LanguageFilter::classify(AnalysisState& N, int entity_id) {
    return (int)N.network[entity_id].language;
}
