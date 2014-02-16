#include "entity.h"

#include "analyzer.h"


namespace follower_set {
    // Helper functions
    static Entity& owner(Context& N) {
        return N.N.network[N.owner_id];
    }

    static Entity& entity(Context& N, int entity_id) {
        return N.N.network[entity_id];
    }

    int LanguageComponent::classify(Context& N, int entity_id) {
        return (int)entity(N, entity_id).language;
    }

    std::string LanguageComponent::cat_name(Context& N, int bin) {
        #define X(x) if (bin == x) return #x;
        X_ALL_LANGUAGES
        #undef X
        return NULL;
    }

    // Compute order of distance between tweeter and follower
    int DistanceComponent::classify(Context& N, int entity_id) {
        Entity& own = owner(N);
        Entity& follower = entity(N, entity_id);
        return own.location.distance_bin(follower.location);
    }

    std::string DistanceComponent::cat_name(Context& N, int bin) {
        const double interval = 1.0 / N_BIN_DISTANCE / 2;
        return format("dist: %.2f to %.2f", bin * interval, (bin + 1) * interval);
    }

    int PreferenceClassComponent::classify(Context& N, int entity_id) {
        return (int) entity(N, entity_id).preference_class;
    }

    std::string PreferenceClassComponent::cat_name(Context& N, int bin) {
        std::vector<EntityPreferenceClass> P = N.N.config.pref_classes;
        if (P.size() <= bin) {
            return "<Unused Slot>";
        }
        return P[bin].name.c_str();
    }

}
