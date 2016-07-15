/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

#ifndef TWEETBANK_H_
#define TWEETBANK_H_

#include <cmath>
#include <vector>
#include <set>
#include <google/sparse_hash_set>

#include "RateTree.h"

#include "serialization.h"
#include "TimeDepBinner.h"

#include "tweets.h"

/* From the follower list, we can reason about:
 *  - preference class, language, distance
 * From the retweet, we can reason about:
 *  - distance, humour
 * Each tweeter's follower list has homogenous (ie, same):
 *  - agent type (of tweeter)
 * */
const double RETWEET_REBIN_TIME_INTERVAL = 1.0;

typedef RateVec</*Rates per: */ 1> TweetReactRateVec;
typedef RateTree<Tweet, /*Rates per: */ 1, /*Branching factor:*/ 4> TweetRateTree;

struct TweetRateDeterminer {
    TweetRateDeterminer(AnalysisState& state) : state(state){
    }
    double get_age(const Tweet& tweet); // Implemented in tweets.cpp
    void update_rate(TweetReactRateVec& rates, const Tweet& tweet, int bin);
    TweetReactRateVec get_rate(const Tweet& tweet, int bin);

    double get_cat_threshold(int bin);

    AnalysisState& state;
};

struct TimeDepRateTree {

    TimeDepRateTree(TweetRateDeterminer determiner, double initial_resolution, int number_of_bins) :
        periodic(RETWEET_REBIN_TIME_INTERVAL), determiner(determiner),
        initial_resolution(initial_resolution), binner(number_of_bins) {
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
        return determiner.get_cat_threshold(i);
    }

    void update(double time) {
        this->time = time;
        if (!periodic.has_past(time)) {
            return;
        }
        binner.update(checker());

//        int i = 0;
//        for (auto& bin : binner.get_bins()) {
//            i++;
//            double time = determiner.get_cat_threshold(i);
//            std::cout << "IN BIN " << i << " HAVE " << bin.size() << std::endl;
//            std::cout << "THEY HAVE elapsed-time < " << time << std::endl;
//        }
    }

    std::vector<TweetRateTree::Node*> as_node_vector() {
        return tree.as_node_vector();
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

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(NVP(periodic));
        // determiner carries no important state
        ar(NVP(last_rate), NVP(initial_resolution), NVP(time));
        ar(NVP(tree));
        ar(NVP(binner));
    }
    int n_bins() {
        return binner.get_bins().size();
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
    bool check(ref_t id);

        // Comparison function for TimeDepBinner:
        bool operator()(ref_t id1, ref_t id2) {
            Tweet& t1 = tree.get(id1).data;
            Tweet& t2 = tree.get(id2).data;
            // Ensure the heap in TimeDepBinner is a min-heap:
            return t1.creation_time > t2.creation_time;
        }

        int initial_bin(ref_t a) {
            return 0; // All tweets start in bin 0
        }

        TimeDepRateTree& tree;
    };

    ElementChecker checker() {
        return ElementChecker(*this, time);
    }

    // Checks whether we should update all elements
    TimePeriodChecker periodic;

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
        PERF_TIMER();
        tree.add(data);
    }

    std::vector<TweetRateTree::Node*> as_node_vector() {
        return tree.as_node_vector();
    }

    std::vector<Tweet> as_vector() {
        auto vec = tree.as_vector();
        for (int i = 0; i < vec.size(); i++) {
            DEBUG_CHECK(vec[i].content.get(), "Tweet has no content!");
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

    template <typename Archive>
    void serialize(Archive& ar) {
        tree.serialize(ar);
    }
};

#endif
