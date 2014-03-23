#ifndef TWEETBANK_H_
#define TWEETBANK_H_

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
const double RETWEET_REBIN_TIME_INTERVAL = 1.0;

typedef RateVec<RETWEET_RATE_ELEMENTS> TweetReactRateVec;
typedef RateTree<Tweet, RETWEET_RATE_ELEMENTS, RETWEET_TREE_N_CHILDREN> TweetRateTree;

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

        int i = 0;
        for (auto& bin : binner.get_bins()) {
            i++;
            double time = determiner.get_cat_threshold(i);
//            std::cout << "IN BIN " << i << " HAVE " << bin.size() << std::endl;
//            std::cout << "THEY HAVE elapsed-time < " << time << std::endl;
        }
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

    READ_WRITE(rw) {
        rw << last_rate << initial_resolution << time;
        tree.visit(rw);
        binner.visit(rw);
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
//        static int ITER = 1;
        tree.add(data);
//        printf("Tweet %d: \n", ITER);
//        printf("------------------------------------------------------------------------\n");
//        print();
//        ITER++;
    }

    std::vector<TweetRateTree::Node*> as_node_vector() {
        return tree.as_node_vector();
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

#endif
