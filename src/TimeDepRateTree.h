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

// Logarithmically organizes elements using RateTree
template <typename T, int N_ELEM, typename RateDeterminer, int N_CHILDREN = 10>
struct TimeDepRateTree {
    typedef google::sparse_hash_set<ref_t, cats::Hasher> TimeSet;
    // For sanity checking, call fall back to the API-compatible std::set:
//    typedef std::set<ref_t> TimeSet;
    typedef std::vector<TimeSet> TimeSets;

    TimeDepRateTree(RateDeterminer determiner, double initial_resolution, int number_of_bins) :
        determiner(determiner), initial_resolution(initial_resolution), number_of_bins(number_of_bins) {
        ASSERT(number_of_bins > 0, "Need more than 0 bins!");
        time_sets.resize(number_of_bins);
        for (int i = 0; i < number_of_bins; i++) {
            time_sets[i].set_deleted_key(-1);
        }
        last_rate = 0;
    }

    ref_t add(const T& data, const RateVec<N_ELEM>& tuple) {
        // ASSUMPTION: Add to initial time set always
        ref_t ref = tree.add(data, tuple);
        time_sets[0].insert(ref);
        return ref;
    }

    size_t size() const {
        return tree.size();
    }

    typename RateTree<T, N_ELEM, N_CHILDREN>::Node& get(ref_t ref) {
        return tree.get(ref);
    }

    RateVec<N_ELEM> rate_summary() {
        return tree.rate_summary();
    }

    double get_cat_threshold(int i) {
        if (i == 0) {
            return 0;
        }
        return initial_resolution* pow(2, i);
    }

    // NOTE: Should only be exposing ref's to leaf nodes,
    // don't call with parent nodes.
    void remove(ref_t handle) {
        // Keep sparsehash! of each
        // How to remove the element? Can have t.get_time()
        T& elem = tree.get(handle).data;
        bool removed = false;
        for (int i = 0; i < time_sets.size(); i++) {
            bool removed = time_sets[i].erase(elem);
            if (removed) {
                double t = elem.get_time();
                DEBUG_CHECK(get_cat_threshold(i - 1) <= t && t < get_cat_threshold(i), "Logic error!");
                removed = true;
                return;
            }
        }
        DEBUG_CHECK(removed, "Logic error!");
        // Must be done last:
        tree.remove(handle);
    }
    void update(double time) {
        int this_bin = (int)floor(last_rate / initial_resolution);
        int next_bin = (int)floor((time) / initial_resolution);

        for (int bin = 0; bin < time_sets.size(); bin++) {
            if (this_bin == next_bin) {
                return; // Logarithmic spacing -- this will be true for all next time bins
            }

            double threshold = determiner.get_cat_threshold(bin);
            TimeSet& set = time_sets[bin];
            TimeSet::iterator it = set.begin();
            for (; it != set.end();) {
                TimeSet::iterator curr = it;
                ++it;

                ref_t ref = *curr;
                T& data = tree.get(ref).data;
                // Need to find new rates
                double age = determiner.get_age(data);
                if (age >= threshold) {
                    // Add to next bin, if it exists:
                    bool is_last_bin = (bin == time_sets.size() - 1);
                    if (!is_last_bin) {
                        time_sets[bin + 1].insert(ref);
                    } else {
                        tree.remove(ref);
                    }
                    RateVec<N_ELEM> rates = determiner.get_rate(data, bin);
                    tree.replace_rate(ref, rates);

                    delete_list.push_back(*curr);
                }
            }
            for (int i = 0; i < delete_list.size(); i++) {
                set.erase(delete_list[i]);
            }
            delete_list.clear();

            this_bin /= 2;
            next_bin /= 2;
        }
    }

    std::vector<T> as_vector() {
        return tree.as_vector();
    }

    void print() {
        tree.print();
    }

    /* Principal KMC method, choose with respect to bin rates. */
    ref_t pick_random_weighted(MTwist& rng) {
        return tree.pick_random_weighted(rng);
    }

private:
    RateDeterminer determiner;
    double last_rate, initial_resolution;
    RateTree<T, N_ELEM, N_CHILDREN> tree;
    std::vector<ref_t> delete_list;
    TimeSets time_sets;
    int number_of_bins;
};

#endif /* TIMEDEPRATETREE_H_ */
