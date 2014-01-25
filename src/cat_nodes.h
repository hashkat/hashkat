#ifndef CAT_NODES_H_
#define CAT_NODES_H_

#include <vector>

#include <cstdio>
#include <cmath>
#include "util.h"
#include "dependencies/mtwist.h"
#include "lcommon/typename.h"

#include "network.h"

#include <google/sparse_hash_set>

namespace cats {

/* Adding to a category group defined recursively.
 * Templates allow us to nest, eg TreeNode<TreeNode<LeafNode>> represents a twice-nested category.
 * A guiding, generic, 'data structure', ClassifierT, is used recursively as well. To provide a ClassifierT structure,
 * create a structure like follows:
 *
 *  struct InnerClassifier {
 *      int classify(Network& N, Foo bar) {
 *          ... return inner category number ...
 *      }
 *      double get(Network& N, int bin) { return rates[bin]; }
 *      // The rates to use
 *      std::vector<double> rates;
 *  };
 *
 *  struct OuterClassifier {
 *      int classify(Network& N, Foo bar) {
 *          ... return outer category number ...
 *      }
 *      InnerClassifier& get(Network& N, int bin) { return children[bin]; }
 *      std::vector<InnerClassifier> children;
 *  };
 *
 *  Then, pass along an instance of OuterLayer when adding to the structure.
 */

/* Every LeafNode holds a google-spare-hash-set. These data structures are highly memory efficient.
 * They are also fairly dynamic. If we hit memory problems, we can write to disk and restart the simulation.
 * The simulation will then be 'defragmented'.
 * For this reason the nice properties of dynamic memory allocation (simplicity and very decent performance) were preferred. */
template <typename Elem>
struct LeafNode {
    typedef google::sparse_hash_set<Elem> HashSet;

    LeafNode() {
        total_rate = 0;
        // MUST be done to use erase() with Google's HashSet
        elems.set_deleted_key(Elem(-1));
    }

    int n_elems() {
        return elems.size();
    }

    template <typename NetworkT>
    bool add(NetworkT& N, double rate, double& delta, const Elem& elem) {
        if (insert(elem)) { // Did we insert a unique element?
            delta = rate;
            total_rate += delta;
            return true;
        }
        return false;
    }

    template <typename NetworkT>
    bool remove(NetworkT& N, double rate, double& delta, const Elem& elem) {
        if (elems.erase(elem) > 0) { // Did we erase an element that was in our set?
            delta = -rate;
            total_rate += delta;
            return true;
        }
        return false;
    }

    double get_total_rate() {
        return total_rate;
    }

    template <typename NetworkT>
    double recalc_rates(NetworkT& N, double rate) {
        total_rate = rate * elems.size();
        return total_rate;
    }

    template <typename NetworkT>
    void print(NetworkT& N, double rate, int bin, int layer) {
        for (int i = 0; i < layer; i++) {
            printf("  ");
        }
        printf("[Bin %d][leaf] (Total %.2f; N_elems %d; Rate %.2f) ", bin, float(total_rate), rate, n_elems());
        printf("[");
        typename HashSet::iterator it = elems.begin();
        for (; it != elems.end(); ++it) {
            printf("%d ", *it);
        }
        printf("]\n");
    }

    template <typename NetworkT, typename ClassifierT, typename Node>
    void transfer(NetworkT& N, ClassifierT& C, Node& o) {
        typename HashSet::iterator it = elems.begin();
        for (; it != elems.end(); ++it) {
            double delta = 0.0;
            o.add(N, C, delta, *it);
        }
        elems.clear();
    }

private:
    double total_rate;
    HashSet elems;

    // Returns true if the element was unique
    bool insert(const Elem& elem) {
        size_t prev_size = elems.size();
        elems.insert(elem);
        return (elems.size() > prev_size);
    }
};

/* Points either to further TreeNode's, or a LeafNode. */
template <typename SubCat>
struct TreeNode {
    TreeNode() {
        total_rate = 0;
        _n_elems = 0;
    }
    int n_elems() {
        return _n_elems;
    }

    void swap(TreeNode& o) {
        double tmp_total = total_rate;
        int tmp_n = _n_elems;
        _n_elems = o._n_elems;
        total_rate = o.total_rate;
        o._n_elems = tmp_n;
        o.total_rate = tmp_total;
        cats.swap(o.cats);
    }

    /* Useful for time-dependent rates */
    template <typename NetworkT, typename ClassifierT>
    void shift_and_recalc_rates(NetworkT& N, ClassifierT& C) {
        cats.resize(cats.size() + 1);
        // Empty category should be swapped into first slot by end
        for (int i = cats.size() - 1; i >= 1; i--) {
            cats[i].swap(cats[i-1]);
        }
        // Collapse all categories that are more than the intended maximum
        int max = C.size();
        for (int i = max; i < cats.size(); i++) {
            cats[i].transfer(N, C.get(N, max - 1), cats[max - 1]);
        }
        cats.resize(C.size());
        recalc_rates(N, C);
    }

    template <typename NetworkT, typename ClassifierT, typename Node>
    void transfer(NetworkT& N, ClassifierT& C, Node& o) {
        for (int i = 0; i < cats.size(); i++) {
            cats[i].transfer(N, C, o);
        }
//        cats.clear();
    }

    template <typename NetworkT, typename ClassifierT>
    double recalc_rates(NetworkT& N, ClassifierT& C) {
        total_rate = 0;
        for (int i = 0; i < size(); i++) {
            total_rate += cats[i].recalc_rates(N, C.get(N,i));
        }
        return total_rate;
    }

    // Leaf node, return true if the element already existed
    template <typename NetworkT, typename ClassifierT, typename Elem>
    bool add(NetworkT& N, ClassifierT& C, double& ret, const Elem& elem) {
        double delta = 0.0;
        int bin = C.classify(N, elem);
        ensure_bin(bin);
        if (cats[bin].add(N, C.get(N, bin), delta, elem)) { // Did we insert a unique element?
            _n_elems++;
            total_rate += delta;
            ret = delta;
            return true;
        }
        return false;
    }

    // Leaf node, return true if the element was actually in the set
    template <typename NetworkT, typename ClassifierT, typename Elem>
    bool remove(NetworkT& N, ClassifierT& C, double& ret, const Elem& elem) {
        double delta = 0.0;
        int bin = C.classify(N, elem);
        ensure_bin(bin);
        if (cats[bin].remove(N, C.get(N, bin), delta, elem)) { // Did we insert a unique element?
            _n_elems--;
            total_rate += delta;
            ret = delta;
            return true;
        }
        return false;
    }

    void ensure_bin(int bin) {
        if (bin >= cats.size()) {
            cats.resize(bin + 1);
        }
    }

    /* Uniform method, choose with respect amount only. */
    int pick_random_uniform(MTwist& rng) {
        int num = rng.rand_int(n_elems());
        for (int i = 0; i < cats.size(); i++) {
            num -= cats.n_elems();
            if (num <= 0) {
                return i;
            }
        }
        ASSERT(false, "Logic error! No bin to choose from, or n_elems don't add up.");
        return -1;
    }
    /* Principal KMC method, choose with respect to bin rates. */
    int pick_random_weighted(MTwist& rng) {
        double num = rng.genrand_real2() * total_rate;
        for (int i = 0; i < cats.size(); i++) {
            num -= cats.total_rate;
            if (num <= 0) {
                return i;
            }
        }
        ASSERT(!cats.empty(), "Logic error! No bin to choose from.");
        return cats.size() - 1; // Assume we should choose last one due to floating point issues
    }

    size_t size() {
        return cats.size();
    }

    SubCat& operator[](int bin) {
        return cats[bin];
    }

    double get_total_rate() {
        return total_rate;
    }

    template <typename NetworkT, typename ClassifierT>
    void print(NetworkT& N, ClassifierT& C, int bin = 0, int layer = 0) {
        for (int i = 0; i < layer; i++) {
            printf("  ");
        }
        printf("[Bin %d][%s] (Total %.2f; N_elems %d) \n",
                bin,
                cpp_type_name_no_namespace(C).c_str(),
                float(total_rate),
                n_elems()
        );
        for (int i = 0; i < cats.size(); i++) {
            cats[i].print(N, C.get(N, i), i, layer + 1);
        }
    }
private:
    double total_rate; // Total weight of the subtree
    int _n_elems;
    std::vector<SubCat> cats;
};

}

#endif
