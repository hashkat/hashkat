#ifndef CAT_NODES_H_
#define CAT_NODES_H_

#include <vector>

#include <cstdio>
#include <cmath>
#include "util.h"
#include "dependencies/mtwist.h"
#include "lcommon/typename.h"
#include "lcommon/strformat.h"

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

// Place hash functions here:
struct Hasher {
    size_t operator()(int elem) const {
        return elem;
    }
};

template <int N_ELEMS>
struct RateTuple {
    RateTuple(double d = 0) {
        for (int i = 0; i < N_ELEMS; i++) {
            rates[i] = d;
        }
    }

    void operator+=(const RateTuple& o) {
        for (int i = 0; i < N_ELEMS; i++) {
            rates[i] += o.rates[i];
        }
    }

    bool operator==(const RateTuple& o) {
        for (int i = 0; i < N_ELEMS; i++) {
            if (rates[i] != o.rates[i]) {
                return false;
            }
        }
        return true;
    }
    double rates[N_ELEMS];
};

template <int N_ELEMS>
struct CompileTimeRateSwitch {
    typedef RateTuple<N_ELEMS> repr_t; // Default, use 'RateTuple<N_ELEMS>'
};

template <>
struct CompileTimeRateSwitch<1> {
    typedef double repr_t; // N_ELEMS == 1, use 'double'
};


// A set that dynamically picks its implementation based on the load.
// For
template <typename T, typename HasherT = Hasher>
struct FlexibleSet {
    enum {
        INITIAL = 0,
        THRESHOLD = 128
    };

    ~FlexibleSet() {
        clear();
    }
    FlexibleSet() {
        hash_impl = NULL;
        vector_impl.reserve(INITIAL);
    }

    struct iterator {
        typedef T value_type;
        int slot;
        T elem;
        iterator() : slot(0) {
        }
        T get() {
            return elem;
        }
    };

    bool pick_random_uniform(MTwist& rng, T& elem) {
        if (UNLIKELY(empty())) {
            return false;
        }
        if (UNLIKELY(hash_impl)) {
            HashSet& elems = *hash_impl;
            int n_hash_slots = elems.rep.table.size();
            int idx;
            while (true) {
                // We will loop until we find a hash-slot that
                // contains a valid instance of 'ElemT'.
                idx = rng.rand_int(n_hash_slots);
                if (elems.rep.table.test(idx)) {
                    elem = elems.rep.table.unsafe_get(idx);
                    if (elem != -1) {
                        return true;
                    }
                }
            }
            ASSERT(false, "Should be unreachable!");
            return false;
        } else {
            int idx = rng.rand_int(vector_impl.size());
            elem = vector_impl[idx];
            return true;
        }
    }

    void print() {
        if (hash_impl) {
            HashSet& elems = *hash_impl;
            printf("{");
            typename HashSet::iterator it = elems.begin();
            for (; it != elems.end(); ++it) {
                printf("%d ", *it);
            }
            printf("}\n");
        } else {
            printf("[");
            typename std::vector<T>::iterator it = vector_impl.begin();
            for (; it != vector_impl.end(); ++it) {
                printf("%d ", *it);
            }
            printf("]\n");
        }
    }
    bool iterate(iterator& iter) {
        if (UNLIKELY(hash_impl)) {
            HashSet& elems = *hash_impl;
            int n_hash_slots = elems.rep.table.size();

            bool found_element = false;
            // Find next available slot
            while (!found_element) {
                if (iter.slot >= n_hash_slots) {
                    // No more elements
                    return false;
                }
                // If not at end, we will loop until we find a hash-slot that
                // contains a valid instance of 'ElemT'.
                if (elems.rep.table.test(iter.slot)) {
                    T elem = elems.rep.table.unsafe_get(iter.slot);
                    if (elem != -1) {
                        iter.elem = elem;
                        found_element = true;
                    }
                }
                iter.slot++;
            }
        } else {
            if (iter.slot >= vector_impl.size()){
                return false;
            }
            iter.elem = vector_impl[iter.slot];
            iter.slot++;
        }
        return true;
    }

    bool erase(const T& elem) {
        if (UNLIKELY(hash_impl) ) {
            return hash_impl->erase(elem);
        }
        // Must scan for element, linearly
        // Luckily, we are below THRESHOLD, and thus should be fairly small
        const int nElem = vector_impl.size();
        for (int i = 0; i < nElem; i++) {
            // Individual case unlikely -- must happen once, though, typically.
            if (UNLIKELY(vector_impl[i] == elem)) {
                vector_impl.erase(vector_impl.begin() + i);
                return true;
            }
        }
        return false;
    }
    bool insert(const T& elem) {
        if (UNLIKELY(vector_impl.size() > THRESHOLD)) {
            switch_to_hash_set();
        }
        if (UNLIKELY(hash_impl) ) {
            HashSet& elems = *hash_impl;
            size_t prev_size = elems.size();
            elems.insert(elem);
            return (elems.size() > prev_size);
        }
        // Must scan for element, linearly
        // Luckily, we are below THRESHOLD, and thus should be fairly small
        const int nElem = vector_impl.size();
        for (int i = 0; i < nElem; i++) {
            if (UNLIKELY(vector_impl[i] == elem)) {
                // Already in our set!
                return false;
            }
        }
        // Otherwise, simply append to our vector:
        vector_impl.push_back(elem);
        return true;
    }
    bool empty() const {
        return hash_impl ? hash_impl->empty() : vector_impl.empty();
    }
    size_t size() const {
        return hash_impl ? hash_impl->size() : vector_impl.size();
    }
    void clear() {
        delete hash_impl;
        hash_impl = NULL;
        vector_impl = std::vector<T>();
    }
private:
    // If we pass THRESHOLD, we should switch to a different implementation
    void switch_to_hash_set() {
        hash_impl = new HashSet(size()*2);
        // MUST be done to use erase() with Google's HashSet:
        hash_impl->set_deleted_key((T)-1);
        // Ensure our vector's memory is reclaimed:
        vector_impl = std::vector<T>();
    }
    typedef google::sparse_hash_set<T, HasherT> HashSet;
    HashSet* hash_impl; // If NULL, use vector_impl
    std::vector<T> vector_impl;
};

/* Every LeafNode holds a google-spare-hash-set. These data structures are highly memory efficient.
 * They are also fairly dynamic. If we hit memory problems, we can write to disk and restart the simulation.
 * The simulation will then be 'defragmented'.
 * For this reason the nice properties of dynamic memory allocation (simplicity and very decent performance) were preferred. */
template <typename ElemT, int N_RATE_ELEMS = 1, typename HasherT = Hasher>
struct LeafNode {
    typedef typename CompileTimeRateSwitch<N_RATE_ELEMS>::repr_t rate_t;
    typedef FlexibleSet<ElemT, HasherT> ElemSet;
    typedef ElemT value_type;
    typedef typename ElemSet::iterator iterator;

    bool iterate(iterator& iter) {
        return elems.iterate(iter);
    }

    LeafNode() : total_rate(0) {
    }

    int n_bins() const {
        return size();
    }

    int size() const {
        return elems.size();
    }

    template <typename StateT>
    bool add(StateT& N, rate_t rate, rate_t& delta, const ElemT& elem) {
        if (insert(elem)) { // Did we insert a unique element?
            delta = rate;
            total_rate += delta;
            return true;
        }
        return false;
    }

    template <typename StateT>
    bool add(StateT& N, rate_t rate, const ElemT& elem) {
        rate_t dummy;
        return add(N, rate, dummy, elem);
    }

    template <typename StateT>
    bool remove(StateT& N, rate_t rate, rate_t& delta, const ElemT& elem) {
        if (elems.erase(elem) > 0) { // Did we erase an element that was in our set?
            delta = -rate;
            total_rate += delta;
            return true;
        }
        return false;
    }

    template <typename StateT>
    bool remove(StateT& N, rate_t rate, const ElemT& elem) {
        rate_t dummy;
        return remove(N, rate, dummy, elem);
    }

    rate_t get_total_rate() {
        return total_rate;
    }

    template <typename StateT>
    rate_t recalc_rates(StateT& N, rate_t rate) {
        total_rate = rate * elems.size();
        return total_rate;
    }

    template <typename StateT>
    void print(StateT& N, rate_t rate, int bin = 0 , int layer = 0, std::string name = std::string()) {
        for (int i = 0; i < layer; i++) {
            printf("  ");
        }
        std::string repr;
        if (name.empty()) {
            repr = format("Bin %d", bin);
        } else {
            repr = format("%s (Bin %d)", name.c_str(), bin);
        }
        printf("[%s][leaf] (Total %.2f; N_elems %d; Rate %.2f) ",
                repr.c_str(),
                float(total_rate),
                rate,
                size());
        elems.print();
    }

    template <typename StateT, typename ClassifierT, typename Node>
    void transfer(StateT& N, ClassifierT& C, Node& o) {
        iterator it;
        while (elems.iterate(it)) {
            rate_t delta = 0.0;
            o.add(N, C, delta, it.get());
        }
        elems.clear();
    }

    bool pick_random_uniform(MTwist& rng, ElemT& elem) {
        return elems.pick_random_uniform(rng, elem);
    }

    ElemT pick_random_uniform(MTwist& rng) {
        ElemT elem;
        if (pick_random_uniform(rng, elem)) {
            return elem;
        }
        ASSERT(false, "Failure in pick_random_uniform");
        return elem;
    }

    bool pick_random_weighted(MTwist& rng, ElemT& elem) {
        // Same thing for LeafNode's
        return pick_random_uniform(rng, elem);
    }

    ElemT pick_random_weighted(MTwist& rng) {
        ElemT elem;
        if (pick_random_weighted(rng, elem)) {
            return elem;
        }
        ASSERT(false, "Failure in pick_random_weighted");
        return elem;
    }

    int debug_size() { // Inefficient
        return size();
    }
    double debug_rate_sum() { // Inefficient
        return total_rate;
    }
private:
    rate_t total_rate;
    ElemSet elems;

    // Returns true if the element was unique
    bool insert(const ElemT& elem) {
        return elems.insert(elem);
    }
};

// Create a mock of a vector using only a static array, that is compatible enough
// for our purposes. Mainly, we do not assume the vector starts empty,
// and we do not assume we can increase the initial capacity.
// If we are OK with accepting out-of-bounds as an error, StaticVector
// will provide much better memory properties.
template <typename T, int N_ELEMS>
struct StaticVector {
    void resize(int amount) {
        if (amount > N_ELEMS) {
            error_exit("Category out-of-bounds! See comment above.");
        }
    }
    // Number of categories
    size_t size() {
        return N_ELEMS;
    }

    bool empty() {
        return false;
    }
    void swap(StaticVector& o) {
        for (int i = 0; i < size(); i++) {
            elem_data[i].swap(o.elem_data[i]);
        }
    }
    T* begin() {
        return elem_data;
    }
    T* end() {
        return elem_data + N_ELEMS;
    }

    T& operator[](int index) {
        DEBUG_CHECK(index >= 0 && index < N_ELEMS, "Out of bounds!");
        return elem_data[index];
    }
private:
    T elem_data[N_ELEMS];
};

/* Points either to further TreeNode's, or a LeafNode. */
template <typename SubCat, typename CatDataT = std::vector<SubCat>, int N_RATE_ELEMS = 1>
struct TreeNode {
    typedef typename CompileTimeRateSwitch<N_RATE_ELEMS>::repr_t rate_t;

    typedef typename SubCat::iterator sub_iterator;
    typedef typename SubCat::value_type value_type;
    typedef CatDataT child_type;

    double debug_size() { // Inefficient
        int n_elems = 0.0;
        for (int i = 0; i < n_bins(); i++) {
            n_elems += (*this)[i].debug_size();
        }
        return n_elems;
    }

    double debug_rate_sum() { // Inefficient
        double sum = 0.0;
        for (int i = 0; i < n_bins(); i++) {
            sum += (*this)[i].debug_rate_sum();
        }
        return sum;
    }
    void debug_check_sums() {
        ASSERT(debug_size() == size(), "Sizes don't match!");
        ASSERT(debug_rate_sum() == total_rate, "Rates don't match!");
    }
    struct iterator {
        typedef typename sub_iterator::value_type value_type;
        int slot;
        sub_iterator sub_iter;
        iterator() {
            slot = 0;
        }
        value_type get() {
            return sub_iter.get();
        }
    };

    bool iterate(iterator& iter) {
        int& bin = iter.slot;
        while (bin < cats.size()) {
            if (cats[bin].iterate(iter.sub_iter)) {
                return true;
            }
            bin++;
            iter.sub_iter = sub_iterator();
        }
        return false;
    }

    TreeNode() {
        total_rate = 0;
        n_elems = 0;
    }
    int size() const {
        return n_elems;
    }

    void swap(TreeNode& o) {
        rate_t tmp_total = total_rate;
        int tmp_n = n_elems;
        n_elems = o.n_elems;
        total_rate = o.total_rate;
        o.n_elems = tmp_n;
        o.total_rate = tmp_total;
        cats.swap(o.cats);
    }

    /* Useful for time-dependent rates
     * Assumes that bins all move together in discrete steps */
    template <typename StateT, typename ClassifierT>
    void shift_and_recalc_rates(StateT& S, ClassifierT& C) {
        // Collapse all categories that will be more than the intended maximum
        // into the maximal category
        int last = C.size() - 1;
        ensure_bin(last);
        DEBUG_CHECK(last >= 1, "Logic error")
        cats[last - 1].transfer(S, C.get(S, last), cats[last]);
        // The category 'last - 1' is now empty, we begin swapping from here.
        // The empty category should be swapped into first slot by end
        for (int i = last - 1; i >= 1; i--) {
            cats[i].swap(cats[i-1]);
        }
        recalc_rates(S, C);
    }

    template <typename StateT, typename ClassifierT, typename Node>
    void transfer(StateT& S, ClassifierT& C, Node& o) {
        for (int i = 0; i < cats.size(); i++) {
            cats[i].transfer(S, C, o);
        }
        cats.clear();
    }

    template <typename StateT, typename ClassifierT>
    rate_t recalc_rates(StateT& S, ClassifierT& C) {
        total_rate = 0;
        for (int i = 0; i < n_bins(); i++) {
            total_rate += cats[i].recalc_rates(S, C.get(S,i));
        }
        return total_rate;
    }

    // Leaf node, return true if the element already existed
    template <typename StateT, typename ClassifierT>
    bool add(StateT& S, ClassifierT& C, rate_t& ret, const value_type& elem) {
        rate_t delta = 0.0;
        int bin = C.classify(S, elem);
        ensure_bin(bin);
        if (cats[bin].add(S, C.get(S, bin), delta, elem)) { // Did we insert a unique element?
            n_elems++;
            DEBUG_CHECK(delta >= 0, "Negative rate delta on 'add'!");
            total_rate += delta;
            ret = delta;
//            debug_check_sums();
            return true;
        }
//        debug_check_sums();
        return false;
    }

    template <typename StateT, typename ClassifierT>
    bool add(StateT& N, ClassifierT& C, const value_type& elem) {
        rate_t dummy;
        return add(N, C, dummy, elem);
    }

    bool pick_random_uniform(MTwist& rng, value_type& elem) {
        if (cats.empty() || size() == 0) {
            return false;
        }
        // Same thing for LeafNode's
        SubCat& sub_cat = cats[random_uniform_bin(rng)];
//        debug_check_sums();
        return sub_cat.pick_random_uniform(rng, elem);
    }

    value_type pick_random_uniform(MTwist& rng) {
        value_type elem;
        if (pick_random_uniform(rng, elem)) {
            return elem;
        }
        ASSERT(false, "Failure in pick_random_uniform");
        return elem;
    }

    bool pick_random_weighted(MTwist& rng, value_type& elem) {
        if (total_rate == 0.0 || size() == 0 ) {
            return false;
        }
        // Same thing for LeafNode's
        SubCat& sub_cat = cats[random_weighted_bin(rng)];
        return sub_cat.pick_random_weighted(rng, elem);
    }

    value_type pick_random_weighted(MTwist& rng) {
        value_type elem;
        if (pick_random_weighted(rng, elem)) {
            return elem;
        }
        ASSERT(false, "Failure in pick_random_weighted");
        return elem;
    }

    // Leaf node, return true if the element was actually in the set
    template <typename StateT, typename ClassifierT>
    bool remove(StateT& S, ClassifierT& C, rate_t& ret, const value_type& elem) {
        rate_t delta = 0.0;
        int bin = C.classify(S, elem);
        ensure_bin(bin);
        if (cats[bin].remove(S, C.get(S, bin), delta, elem)) { // Did we insert a unique element?
            n_elems--;
            DEBUG_CHECK(delta <= 0, "Positive rate delta on 'remove'!");
            total_rate += delta;
            ret = delta;
//            debug_check_sums();
            return true;
        }
//        debug_check_sums();
        return false;
    }

    template <typename StateT, typename ClassifierT>
    bool remove(StateT& N, ClassifierT& C, const value_type& elem) {
        rate_t dummy;
        return remove(N, C, dummy, elem);
    }

    void ensure_bin(int bin) {
        if (bin >= cats.size()) {
            cats.resize(bin + 1);
        }
    }

    /* Uniform method, choose with respect to amount only. */
    int random_uniform_bin(MTwist& rng) {
        int num = rng.rand_int(size());
        for (int i = 0; cats.size(); i++) {
            num -= cats[i].size();
            if (num <= 0) {
                return i;
            }
        }
        ASSERT(false, "Logic error! No bin to choose from, or n_elems don't add up.");
        return -1;
    }
    /* Principal KMC method, choose with respect to bin rates. */
    int random_weighted_bin(MTwist& rng) {
        double num = rng.genrand_real2() * total_rate;
        for (int i = 0; i < cats.size(); i++) {
            num -= cats[i].get_total_rate();
            if (num <= 0) {
                return i;
            }
        }
        ASSERT(!cats.empty(), "Logic error! No bin to choose from.");
        return cats.size() - 1; // Assume we should choose last one due to floating point issues
    }

    size_t n_bins() {
        return cats.size();
    }

    SubCat& operator[](int bin) {
        return cats[bin];
    }

    rate_t get_total_rate() {
        return total_rate;
    }

    template <typename StateT, typename ClassifierT>
    void print(StateT& S, ClassifierT& C, int bin = -1, int layer = 0, std::string name = std::string()) {

        for (int i = 0; i < layer; i++) {
            printf("  ");
        }
        std::string repr = "Root";
        if (bin != -1) {
            if (name.empty()) {
                repr = format("Bin %d", bin);
            } else {
                repr = format("%s (Bin %d)", name.c_str(), bin);
            }
        }
        printf("[%s][%s] (Total %.2f; N_elems %d) \n",
                repr.c_str(),
                cpp_type_name_no_namespace(C).c_str(),
                float(total_rate),
                size()
        );
        for (int i = 0; i < cats.size(); i++) {
            if (cats[i].size() > 0) {
                cats[i].print(S, C.get(S, i), i, layer + 1, C.cat_name(S, i));
            }
        }
    }

    CatDataT& children() {
        return cats;
    }
private:
    rate_t total_rate; // Total weight of the subtree
    int n_elems;
    CatDataT cats;
};


/* Helper class for implementing begin() & end() semantics in wrapping classes.
 * Implements the minimal iterator interface used by C++11 iteration. */
template <typename CatGroup>
struct NodeIterator {
    typedef typename CatGroup::iterator Iter;

    NodeIterator(CatGroup* cats, const Iter& it) {
        this->cats = cats;
        this->it = it;
        have_next = true;
    }

    NodeIterator& operator++() {
//        printf("GOT TO SLOT %d\n", it.slot);
        have_next = cats->iterate(it);
//        printf("HAVE NEXT?  %d\n", have_next);
        return *this;
    }

    bool operator==(const NodeIterator& o) {
        if (!have_next && !o.have_next) {
            return true; // Hack to finish iteration
        }
        return cats == o.cats && it.slot == o.it.slot && have_next == o.have_next;
    }
    bool operator!=(const NodeIterator& o) {
        return !(*this == o);
    }
    static NodeIterator begin(CatGroup* cats) {
        auto ret = NodeIterator(cats, Iter());
        if (cats->size() == 0) {
            ret.have_next = false;
        } else {
            ++ret;
        }
        return ret;
    }
    static NodeIterator end(CatGroup* cats) {
        auto ret = begin(cats);
        ret.have_next = false;
        return ret;
    }

    typename Iter::value_type operator*() {
        return it.get();
    }

private:
    Iter it;
    CatGroup* cats;
    bool have_next;
};


}

#endif
