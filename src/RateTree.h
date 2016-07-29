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

#ifndef RATETREE_H_
#define RATETREE_H_

#include "serialization.h"

// 100 bytes / tweet overhead
// Make global choice based on tree
// Time dependency checked based on thresholds
//  - Same binning throughout?
//  - How to know when to check?
//  - Have many bins right away, it makes sense
//  - How to assign a rate to unfollow??
//  - Based on random chance purely? Same as follow??
// - Can we do anything interesting with follow rates ?
//  - Retweet already does! Only interesting thing is dynamism
//  - At cost of double the rate storage, can have separate unfollow rates
//  - Effect is negligible, hard to model


template <int N_ELEM>
struct RateVec {
    double tuple[N_ELEM];
    double tuple_sum;
    RateVec(double v = 0) {
        for (int i = 0; i < N_ELEM; i++) {
            tuple[i] = v;
        }
        tuple_sum = v * N_ELEM;
    }
    void print() const {
        printf("(Sum = %.2f)[Elems: ", tuple_sum);
        for (int i = 0; i < N_ELEM; i++) {
            printf("%.2f ", tuple[i]);
        }
        printf("]");
    }
    void add(int n, double val) {
        tuple[n] += val;
        tuple_sum += val;
    }
    void add(RateVec& o) {
        for (int i = 0; i < N_ELEM; i++) {
            tuple[i] += o.tuple[i];
        }
        tuple_sum += o.tuple_sum;
    }
    void sub(RateVec& o) {
        for (int i = 0; i < N_ELEM; i++) {
            tuple[i] -= o.tuple[i];
        }
        tuple_sum -= o.tuple_sum;
    }
    RateVec delta(const RateVec& o) {
        RateVec ret = o;
        ret.sub(*this);
        return ret;
    }
    template <typename Archive>
    void serialize(Archive& ar) {
        for (double& val : tuple) {
            ar(val);
        }
        ar(tuple_sum);
    }
};

template <typename T, int N_ELEM, int N_CHILDREN = 2>
struct RateTree {
    typedef int ref_t;
    enum {
        INVALID = -1
    };

    struct Node {
        ref_t parent; // INVALID if root
        short depth;
        bool is_leaf; // Default true
        bool is_allocated; // Default false
        // Only leaves have data!! (Do not use if is_leaf is false!)
        T data;
        RateVec<N_ELEM> rates;

        ref_t children[N_CHILDREN]; // INVALID if not allocated

        template <typename Archive>
    void serialize(Archive& ar) {
            ar(parent, depth, is_leaf, is_allocated);
            ar(data);
            ar(rates);
            for (ref_t& child : children) {
                ar(child);
            }
        }

        double debug_rate_sum(RateTree& tree) { // Inefficient
            if (is_leaf) {
                return rates.tuple_sum;
            }
            double sum = 0.0;
            for (int i = 0; i < N_CHILDREN; i++) {
                if (children[i] != INVALID) {
                    sum += tree.get(children[i]).debug_rate_sum(tree);
                }
            }
            return sum;
        }

        bool has_vacancy() {
            for (int i = 0; i < N_CHILDREN; i++) {
                if (children[i] == INVALID) {
                    return true; // At least one free slot still
                }
            }
            return false;
        }
        void link_to_parent(ref_t parent, int depth) {
            this-> parent = parent;
            this-> depth = depth;
        }

        ref_t alloc_child(RateTree& tree, ref_t self, int child) {
             ref_t ref = tree.alloc_node();
             children[child] = ref;
             is_allocated = true; // Hack for parent object, which starts !is_allocated
             tree.get(ref).link_to_parent(self, depth + 1);
             return ref;
        }

        ref_t* seek_ref(ref_t ref) {
            for (int i = 0; i < N_CHILDREN; i++) {
                if (children[i] == ref) {
                    return &children[i];
                }
            }
            return NULL;
        }

        // Shift so that the tree can grow
        void alloc_parent(RateTree& tree, ref_t self) {
             ref_t ref = tree.alloc_node();
             tree.get(ref) = *this; // Make a copy
             tree.get(ref).is_leaf = false;
             depth++;
             // Update pointer from parent to self
             if (parent != INVALID) {
                 ref_t* self_loc = tree.get(parent).seek_ref(self);
                 *self_loc = ref;
             }
             parent = ref; // Take new node as parent
             // Become first child of new parent
             tree.get(parent).children[0] = self;
        }

        // If a child node was allocated, place it in child_output, otherwise place self.
        // Does NOT have any effect on the total rate balance.
        // Returns false if fails to do so.
        bool alloc_with_next_vacancy(RateTree& tree, ref_t self, ref_t& child_output) {
            // Leaf case:
            if (is_leaf) {
                alloc_parent(tree, self);
                tree.post_vacancy(self); // Vacant to expand again, if eventually necessary
                // Call into a parent case:
                tree.get(parent).alloc_with_next_vacancy(tree, parent, child_output);
                return true;
            }
            // Parent case:
            int i = 0;
            for (; i < N_CHILDREN; i++) {
                if (children[i] == INVALID) {
                    child_output = alloc_child(tree, self, i);
                    tree.post_vacancy(child_output); // Post child to vacancy list
                    break;
                }
            }
            if (i >= N_CHILDREN) {
                return false; // No vacancy
            }

            for (; i < N_CHILDREN; i++) {
                if (children[i] == INVALID) {
                    // At least one free slot still, re-post to vacancy list
                    tree.post_vacancy(self);
                    return true;
                }
            }
            // All slots have been used, stay removed from vacancy list
            return true;
        }
        Node() {
            parent = INVALID;
            is_leaf = true;
            is_allocated = false;
            depth = 0;
            for (int i =0; i < N_CHILDREN; i++) {
                children[i] = INVALID;
            }
        }

        void rate_add(RateTree& tree, RateVec<N_ELEM>& vec) {
            rates.add(vec);
            if (parent != INVALID) {
                tree.get(parent).rate_add(tree, vec);
            }
        }
        /* Principal KMC method, choose with respect to bin rates. */
        ref_t random_weighted_bin(RateTree& tree, MTwist& rng) {
            bool dummy = false;
            while (!dummy) {
                double num = rng.rand_real_not1() * rates.tuple_sum;
                for (int i = 0; i < N_CHILDREN; i++) {
                    int c = children[i];
                    if (c != INVALID) {
                        num -= tree.get(c).rates.tuple_sum;
                        if (num <= ZEROTOL) {
                            dummy = true;
                            return i;
                        }
                    }
                }
            }
            ASSERT(false, "Logic error! No child to choose from.");
            return INVALID;
        }

        ref_t pick_random_weighted(RateTree& tree, MTwist& rng, ref_t self) {
            if (is_leaf) {
                return self; // We are done
            }
            ref_t child = children[random_weighted_bin(tree, rng)];
            return tree.get(child).pick_random_weighted(tree, rng, child);
        }

        void rate_sub(RateTree& tree, RateVec<N_ELEM>& vec) {
            rates.sub(vec);
            if (parent != INVALID) {
                tree.get(parent).rate_sub(tree, vec);
            }
        }

        void print(RateTree& tree, ref_t self, int tab) {
            for (int i = 0; i < tab; i++) { printf("  "); }
            printf("%s Node %d p=%d depth=%d\n", (!is_leaf ? "Parent" : "Leaf"), self, parent, depth);
            if (is_leaf) {
                for (int i = 0; i < tab; i++) { printf("  "); }
                data.print();
            }

            for (int i = 0; i < tab; i++) { printf("  "); }
            rates.print();
            printf("\n");
            for (int i =0; i < N_CHILDREN; i++) {
                if (children[i] != INVALID) {
                    tree.get(children[i]).print(tree, children[i], tab + 1);
                }
            }
        }
    };

    // Debug methods:
    void debug_check_rates() {
        std::vector<Node*> v = as_node_vector(/*all:*/ true);
        for (int i = 0; i < v.size(); i++) {
            double calc_sum = v[i]->debug_rate_sum(*this);
            double stored_sum = v[i]->rates.tuple_sum;
            if (fabs(calc_sum - stored_sum) >= 10e-5) {
                printf("GOT calc_sum= %f vs stored_sum= %f\b", calc_sum, stored_sum);
                ASSERT(false, "Should be (fairly) equal!");
            }
        }
    }
    void debug_check_reachability(ref_t ref) {
        std::vector<Node*> nv = as_node_vector();
        ASSERT(nv.size() == size(), "Size mismatch!");
        std::vector<Node*> v = as_node_vector(/*all:*/ true);
        int occurrences = 0;
        for (int i = 0; i < v.size(); i++) {
            if (v[i] == &get(ref)) {
                occurrences++;
            }
        }
        ASSERT(occurrences == 1, "Should occur exactly once in reachability list!")
    }

    RateTree() {
        n_elems = 0;
        node_pool.resize(1); // Allocate root
        ensure_vacancy_depth(0).push_back(0); // Starts vacant itself, and with vacant children slots
        get(0).is_leaf = false; // Root of the tree is not a leaf
    }
    Node& get(ref_t handle) {
        return node_pool[handle];
    }

    // Debugging only
    bool has_child(Node& p, ref_t handle) {
        for (int i = 0; i < N_CHILDREN; i++) {
            if (p.children[i] == handle) {
                return true;
            }
        }
        return false;
    }

    void as_node_vector(Node& node, std::vector<Node*>& vec, bool all = false) {
        if (node.is_leaf) {
            vec.push_back(&node);
            return;
        }
        if (all) {
            vec.push_back(&node);
        }
        // Not a leaf, query our children:
        for (int i = 0; i < N_CHILDREN; i++) {
            int c = node.children[i];
            if (c != INVALID) {
                as_node_vector(get(c), vec, all);
            }
        }
    }

    std::vector<Node*> as_node_vector(bool all = false) {
        std::vector<Node*> vec;
        if (node_pool[0].is_allocated) {
        as_node_vector(node_pool[0], vec, all);
        }
        return vec;
    }

    std::vector<T> as_vector() {
        std::vector<Node*> node_vec = as_node_vector();
        std::vector<T> vec;
        for (int i = 0; i < node_vec.size(); i++) {
            vec.push_back(node_vec[i]->data);
        }
        return vec;
    }

    // NOTE: Should only be exposing ref's to leaf nodes,
    // don't call with parent nodes.
    void remove(ref_t handle) {
        DEBUG_CHECK(get(handle).is_leaf, "Cannot remove parent nodes in this tree implementation.");
        Node& n = get(handle);
        Node& p = get(n.parent);
        // Perform unlink
        bool removed = false, was_full = true;
        for (int i = 0; i < N_CHILDREN; i++) {
            if (p.children[i] == INVALID) {
                was_full = false;
            } else if (p.children[i] == handle) {
                p.children[i] = INVALID;
                removed = true;
            }
        }
        if (was_full) {
            // Important: Repost parent to vacancy list if not already in it!!
            post_vacancy(n.parent);
        }

        DEBUG_CHECK(handle != 0, "Could not remove top node!");
        DEBUG_CHECK(removed, "Could not remove child!");
        DEBUG_CHECK(!has_child(p, handle), "Removed child, but duplicate exists!");
        get(n.parent).rate_sub(*this, n.rates);
        n_elems--;
        free_list.push_back(handle);
        n = Node();// 'Wipe' the node
//        debug_check_rates();
    }

    ref_t pick_random_weighted(MTwist& rng) {
        ASSERT(size() > 0, "No element to pick!");
        return get(0).pick_random_weighted(*this, rng, 0);
    }

    ref_t add(const T& data, const RateVec<N_ELEM>& tuple) {
        const int BUFF = 3; // Buffer room resolve cases where nodes can become deallocated during internal methods
        node_pool.reserve(node_pool.size() + BUFF);
        ref_t node = find_vacancy();
        Node& n = get(node);
        n.data = data;
        n.rates = tuple;
        get(n.parent).rate_add(*this, n.rates);
        n_elems++;

//        debug_check_reachability(node);
//        debug_check_rates();
        return node;
    }
    void replace_rate(ref_t ref, const RateVec<N_ELEM>& tuple) {
        Node& n = get(ref);
        RateVec<N_ELEM> delta = n.rates.delta(tuple);
        n.rates = tuple;
        get(n.parent).rate_add(*this, delta);
    }

    RateVec<N_ELEM> rate_summary() {
        return get(0).rates;
    }

    void print() {
        get(0).print(*this, 0, 0);
        printf("Leaf nodes = %d, total number of nodes = %d\n", (int) size(), (int) node_pool.size());
    }
    size_t size() const {
        return n_elems;
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(n_elems, free_list); //Freed nodes
        ar(node_pool, vacancy_list);
        for (auto& list : vacancy_list) {
            ar(list);
        }
        printf("Checking tweet/retweet RateTree structure integrity...\n");
        debug_check_rates();
        printf("Tweet/retweet RateTree structure integrity checks out.\n");
    }
private:
    typedef std::vector<ref_t> ref_list;
    ref_t alloc_node() {
        PERF_TIMER();
        ref_t n;
        if (!free_list.empty()) {
            // Reuse deallocated nodes before anything else:
            n = free_list.back();
            free_list.pop_back();
        } else {
            int size = node_pool.size();
            node_pool.resize(size + 1);
            n = size;
        }
        get(n).is_allocated = true;
        return n;
    }

    void post_vacancy(ref_t ref) {
        ensure_vacancy_depth(get(ref).depth).push_back(ref);
    }

    bool check_validity(int depth, ref_t ref) {
        return (get(ref).depth == depth);
    }

    ref_t find_vacancy(int depth, ref_list& sub_list) {
        PERF_TIMER();
        while (!sub_list.empty()) {
            ref_t vacancy = sub_list.back();
            // We assume we need to pop the element, we re-add as necessary (eg in alloc_with_next_vacancy or right below)
            sub_list.pop_back();
            if (!check_validity(depth, vacancy) ) {
                continue;
            }
            ref_t new_node;
            bool valid = get(vacancy).alloc_with_next_vacancy(*this, vacancy, new_node);
            if (!valid) {
                continue;
            }
            return new_node;
        }
        return INVALID;
    }
    // Find a vacant parent to link our node to
    // Does NOT propagate changes up the tree (add does)
    ref_t find_vacancy() {
        // A node with some sort of vacancy *must* exist
        for (int depth = 0; depth < vacancy_list.size(); depth++) {
            ref_list& sub_list = vacancy_list[depth];
            ref_t vacancy = find_vacancy(depth, sub_list);
            if (vacancy != INVALID) {
                return vacancy;
            }
        }
        ASSERT(false, "Should never happen!");
        return INVALID;
    }
    ref_list& ensure_vacancy_depth(int depth) {
        if (vacancy_list.size() < depth + 1) {
            vacancy_list.resize(depth + 1);
        }
        return vacancy_list[depth];
    }

    size_t n_elems;
    std::vector<ref_t> free_list; //Freed nodes
    std::vector<Node> node_pool; // 0 is the root node
    std::vector<ref_list> vacancy_list;
};

#endif
