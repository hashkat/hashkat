#include "entity.h"

#include "analyzer.h"

namespace follower_set {

    int LanguageComponent::classify(AnalysisState& N, int entity_id) {
        return (int) N.network[entity_id].language;
    }

    const char* LanguageComponent::cat_name(AnalysisState& N, int bin) {
        #define X(x) if (bin == x) return #x;
        X_ALL_LANGUAGES
        #undef X
        return NULL;
    }

    int PreferenceClassComponent::classify(AnalysisState& N, int entity_id) {
        return (int) N.network[entity_id].preference_class;
    }

    const char* PreferenceClassComponent::cat_name(AnalysisState& N, int bin) {
        std::vector<EntityPreferenceClass> P = N.config.pref_classes;
        if (P.size() <= bin) {
            return "<Unused Slot>";
        }
        return P[bin].name.c_str();
    }

}
