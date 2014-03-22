/*
 * TimeDepRateTree.h
 *
 *  Created on: Feb 17, 2014
 *      Author: adomurad
 */

#ifndef TIMEDEPRATETREE_H_
#define TIMEDEPRATETREE_H_

#include <cmath>
#include <vector>
#include <set>
#include <google/sparse_hash_set>

#include "cat_nodes.h"
#include "RateTree.h"

#include "DataReadWrite.h"
#include "TimeDepBinner.h"

#include "tweets.h"

/* From the follower list, we can reason about:
 *  - preference class, language, distance
 * From the retweet, we can reason about:
 *  - distance, humour
 * Each tweeter's follower list has homogenous (ie, same):
 *  - entity type (of tweeter)
 * */
const int RETWEET_RATE_ELEMENTS = 1;
const int RETWEET_TREE_N_CHILDREN = 10;

typedef RateVec<RETWEET_RATE_ELEMENTS> TweetReactRateVec;
typedef RateTree<Tweet, RETWEET_RATE_ELEMENTS, RETWEET_TREE_N_CHILDREN> TweetRateTree;

struct TweetRateDeterminer {
    TweetRateDeterminer(AnalysisState& state) : state(state){
    }
    double get_age(const Tweet& tweet); // Implemented in tweets.cpp
    void update_rate(TweetReactRateVec& rates, const Tweet& tweet, int bin);
    TweetReactRateVec get_rate(const Tweet& tweet, int bin);

    double get_cat_threshold(int bin) {
        return (1 << bin) * 90.0;
    }

    AnalysisState& state;
};

struct TimeDepRateTree {

    TimeDepRateTree(TweetRateDeterminer determiner, double initial_resolution, int number_of_bins) :
        determiner(determiner), initial_resolution(initial_resolution), binner(number_of_bins) {
        ASSERT(number_of_bins > 0, "Need more than 0 bins!");
        last_rate = 0;
        time = 0;
    }

    /*
     * Add an element. Determiner determines the rate associated.
     */
    ref_t add(const Tweet& data) {
        // ASSUMPTION: Add to initial time bin always
        const int TIME_BIN = 0;
        TweetReactRateVec rate_tuple = determiner.get_rate(data, TIME_BIN);
        ref_t ref = tree.add(data, rate_tuple);
        binner.add(checker(), ref);
        return ref;
    }

    size_t size() const {
        return tree.size();
    }

    typename TweetRateTree::Node& get(ref_t ref) {
        return tree.get(ref);
    }

    TweetReactRateVec rate_summary() {
        return tree.rate_summary();
    }

    double get_cat_threshold(int i) {
        if (i == 0) {
            return 0;
        }
        return initial_resolution* pow(2, i);
    }

    void update(double time) {
        this->time = time;

    }

    std::vector<Tweet> as_vector() {
        return tree.as_vector();
    }

    void print() {
        tree.print();
    }

    /* Principal KMC method, choose with respect to bin rates. */
    ref_t pick_random_weighted(MTwist& rng) {
        return tree.pick_random_weighted(rng);
    }

    READ_WRITE(rw) {
        rw << last_rate << initial_resolution;
        tree.visit(rw);
        binner.visit(rw);
    }
private:

    struct ElementChecker {
        ElementChecker(TimeDepRateTree& tree, double time) : tree(tree) {
            this->time = time;
            bin = -1;
        }
        void set_bin(int bin) {
            this->bin = bin;
        }

        double time;
        int bin;

        // Bin-move-check function for TimeDepBinner:
        bool check(ref_t id) {
            Tweet& t = tree.get(id).data;
            double elapsed = time - t.creation_time;
            if (elapsed > t.retweet_next_rebin_time) {
                // Move to a new bin:
                t.retweet_time_bin++;
                printf("MOVING TO BIN %d\n", t.retweet_time_bin);
                t.retweet_next_rebin_time = tree.determiner.get_cat_threshold(t.retweet_time_bin);
                return false;
            }
            return true;
        }

        // Comparison function for TimeDepBinner:
        bool operator()(ref_t a, ref_t b) {
            return a > b;
        }

        int initial_bin(ref_t a) {
            return 0; // All tweets start in bin 0
        }

        TimeDepRateTree& tree;
    };

    ElementChecker checker() {
        return ElementChecker(*this, time);
    }

    TweetRateDeterminer determiner;
    double last_rate, initial_resolution;
    TweetRateTree tree;
    double time;
    TimeDepBinner binner;
};


struct TweetBank {
    TimeDepRateTree tree;

    double get_total_rate() {
        return tree.rate_summary().tuple_sum;
    }

    TweetBank(AnalysisState& state);

    /*
     * Add an tweet. TweetRateDeterminer determines the reaction rate (follow or retweet) associated.
     */
    void add(const Tweet& data) {
//        static int ITER = 1;
        tree.add(data);
//        printf("Tweet %d: \n", ITER);
//        printf("------------------------------------------------------------------------\n");
//        print();
//        ITER++;
    }

    std::vector<Tweet> as_vector() {
        auto vec = tree.as_vector();
        for (int i = 0; i < vec.size(); i++) {
            DEBUG_CHECK(!vec[i].content.empty(), "Tweet has no content!");
        }
        return vec;
    }

    void print() {
        tree.print();
    }
    int n_active_tweets() const {
        return tree.size();
    }
    Tweet& pick_random_weighted(MTwist& rng) {
        ref_t ref = tree.pick_random_weighted(rng);
        return tree.get(ref).data;
    }

    READ_WRITE(rw) {
        tree.visit(rw);
        rw.check_visit(get_total_rate());
    }
};

#endif /* TIMEDEPRATETREE_H_ */
