#ifndef ENTITY_FOLLOW_SETS_H_
#define ENTITY_FOLLOW_SETS_H_

#include "cat_classes.h"
#include "cat_nodes.h"

#include "events.h"
#include "entity_properties.h"
#include "config_static.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

struct FollowerSet {

    /******************************************************************************
     * Nested category definitions. These define the necessary classification
     * methods for configurating the classes in cat_classes.h, which in turn
     * provide the necessary implementations of cat_nodes.h.
     *
     * This rather complicated scheme allows for easy rearranging of the categorization
     * layers, without worrying about the implementation of the categorization itself.
     *
     * This was chosen so that the code could be written in a way that allowed for
     * independent tweaking of the data-structure implementation.
     *
     * The methods are implemented in entity_follow_sets.cpp.
     *****************************************************************************/

    struct Context;
    struct PreferenceClassComponent: cats::StaticLeafClass<int, N_BIN_PREFERENCE_CLASS> {
        int classify(Context& N, int entity_id); // Defined in entity.cpp
        std::string cat_name(Context& N, int bin);
    };
    // The distance you are to your followers is a categorization dimension
    struct DistanceComponent: cats::StaticTreeClass<PreferenceClassComponent, N_BIN_DISTANCE> {
        std::string cat_name(Context& N, int bin);
        int classify(Context& N, int entity_id); // Defined in entity.cpp
    };
    struct LanguageComponent: cats::StaticTreeClass<DistanceComponent, N_LANGS> {
        std::string cat_name(Context& N, int bin);
        int classify(Context& N, int entity_id); // Defined in entity.cpp
    };

    typedef LanguageComponent Rates;
    typedef LanguageComponent::CatGroup Followers;
    typedef cats::NodeIterator<Followers> iterator;

    /*****************************************************************************
     * The context option is used during the nested-classification so that
     * we can refer to the owned of the FollowerSet.
     *
     * We need to pass AnalysisState for most lookups (eg Entity variables),
     * but we also need owner_id so that we may calculate distance.
     *****************************************************************************/

    struct Context {
        Context(AnalysisState& N, int owner_id); // Implemented in cpp file

        AnalysisState& N; // Analysis state
        Rates& R;
        int owner_id; // Owner of the follower set
    };

    /*****************************************************************************
     * Public methods:
     ******************************************************************************/

    iterator begin() {
        return iterator::begin(&implementation);
    }

    iterator end() {
        return iterator::end(&implementation);
    }

    bool add(Context& C, int id) {
        return implementation.add(C, C.R, id);
    }

    bool remove(Context& C, int id) {
        return implementation.remove(C, C.R, id);
    }

    bool pick_random_weighted(MTwist& rng, int& id) {
        return implementation.pick_random_weighted(rng, id);
    }

    bool pick_random_uniform(MTwist& rng, int& id) {
        return implementation.pick_random_uniform(rng, id);
    }

    void print(Context& C) {
        implementation.print(C, C.R);
    }

    size_t size() const {
        return implementation.size();
    }
    double get_total_rate() {
        return implementation.get_total_rate();
    }

    /* Return an iterable collection of
     * the children categories of the first layer: */
    Followers::child_type children() {
        return implementation.children();
    }

private:
    Followers implementation;
};

struct FollowingSet {
    typedef cats::LeafNode<int> Followings;
    typedef cats::NodeIterator<Followings> iterator;

    void print(AnalysisState& S) {
        implementation.print(S, /*Dummy: */ 1.0);
    }

    void print(AnalysisState& S, int id) {
        implementation.add(S, /*Dummy: */ 1.0, id);

    }

    bool add(AnalysisState& S, int id) {
        return implementation.add(S, /*Dummy: */ 1.0, id);
    }

    bool remove(AnalysisState& S, int id) {
        return implementation.remove(S, /*Dummy: */ 1.0, id);
    }

    // No such thing as pick_random_weighted for FollowingSet
    bool pick_random_uniform(MTwist& rng, int& id) {
        return implementation.pick_random_uniform(rng, id);
    }

    iterator begin() {
        return iterator::begin(&implementation);
    }

    iterator end() {
        return iterator::end(&implementation);
    }

    size_t size() const {
        return implementation.size();
    }

private:
    Followings implementation;
};


#endif
