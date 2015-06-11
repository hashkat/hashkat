#ifndef HashedEdgeSet_H_
#define HashedEdgeSet_H_

#include <fstream>
#include "dependencies/mtwist.h"
#include "lcommon/typename.h"
#include "lcommon/strformat.h"
#include "DataReadWrite.h"

#include "SerializeBufferFileMock.h"
#include <google/sparse_hash_set>

// Trivial hash, for google sparse hash:
struct Hasher {
    size_t operator()(int elem) const {
        return elem;
    }
};

/*
 * Wrapper class for using google sparse hash to hold our graph.
 * Most importantly, using our modified sparse hash impl., we reach
 * in and use the internals to implement pick_random_uniform over
 * their array-like abstraction.
 *
 * T must be a pointer, or integer.
 */
template<typename T, typename HasherT = Hasher>
struct HashedEdgeSet {
    HashedEdgeSet() {
        hash_impl.set_deleted_key((T) -1);
    }

    struct iterator {
        typedef T value_type;
        int slot;
        T elem;
        iterator() :
                slot(0), elem(-1) {
        }
        T get() {
            DEBUG_CHECK(elem != -1, "Getting invalid element!")
            return elem;
        }
    };

    bool pick_random_uniform(MTwist& rng, T& elem) {
        if (UNLIKELY(empty())) {
            return false;
        }
		int n_hash_slots = hash_impl.rep.table.size();
		int idx;
		while (true) {
			// We will loop until we find a hash-slot that
			// contains a valid instance of 'ElemT'.
			idx = rng.rand_int(n_hash_slots);
			if (hash_impl.rep.table.test(idx)) {
				elem = hash_impl.rep.table.unsafe_get(idx);
				if (elem != -1) {
					return true;
				}
			}
		}
		ASSERT(false, "Should be unreachable!");
		return false;
    }

    void print() {
		printf("[");
		typename HashSet::iterator it = hash_impl.begin();
		for (; it != hash_impl.end(); ++it) {
			printf("%d ", *it);
		}
		printf("]\n");
    }
	bool iterate(iterator& iter) {
		int n_hash_slots = hash_impl.rep.table.size();

		bool found_element = false;
		// Find next available slot
		while (!found_element) {
			if (iter.slot >= n_hash_slots) {
				// No more elements
				return false;
			}
			// If not at end, we will loop until we find a hash-slot that
			// contains a valid instance of 'ElemT'.
			if (hash_impl.rep.table.test(iter.slot)) {
				T elem = hash_impl.rep.table.unsafe_get(iter.slot);
				if (elem != -1) {
					iter.elem = elem;
					found_element = true;
				}
			}
			iter.slot++;
		}
		return true;
	}
//
    bool contains(const T& elem) {
        /* Hashtable case: */
        return (hash_impl.find(elem) != hash_impl.end());
    }
    bool erase(const T& elem) {
        return hash_impl.erase(elem);
    }
    bool insert(const T& elem) {
		size_t prev_size = hash_impl.size();
		hash_impl.insert(elem);
		return (hash_impl.size() > prev_size);
    }

    bool empty() const {
        return hash_impl.empty();
    }
    size_t size() const {
        return hash_impl.size();
    }
    void clear() {
        hash_impl = HashSet();
        hash_impl.set_deleted_key((T) -1);
    }

    READ_WRITE(rw) {
        rw.checkMagic(0x5a56);
        if (rw.is_reading()) {
			clear();
			hash_impl.read_metadata(&rw.get_buffer());
			hash_impl.read_nopointer_data(&rw.get_buffer());
        } else if (rw.is_writing()) {
			hash_impl.write_metadata(&rw.get_buffer());
			hash_impl.write_nopointer_data(&rw.get_buffer());
        }
        rw.checkMagic(0x5a54);
    }

    std::vector<T> as_vector() {
        std::vector<T> ret;

        iterator iter;
        while (iterate(iter)) {
            ret.push_back(iter.get());
        }

        return ret;
    }

private:
    typedef google::sparse_hash_set<T, HasherT> HashSet;
    HashSet hash_impl; // If NULL, empty
};

#endif
