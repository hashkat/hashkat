#include "entity.h"

#include "analyzer.h"

int follower_set::LanguageClass::classify(AnalysisState& N, int entity_id) {
    return (int)N.network[entity_id].language;
}
