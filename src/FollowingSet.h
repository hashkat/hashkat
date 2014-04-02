#ifndef FOLLOWING_SET_H_
#define FOLLOWING_SET_H_

#include "cat_classes.h"
#include "cat_nodes.h"

#include "events.h"
#include "entity_properties.h"
#include "config_static.h"

// Forward declare, to prevent circular header inclusion:
struct AnalysisState;

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
