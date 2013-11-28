/* Defines structures necessarily for handling follow sets. */

#ifndef FOLLOW_SETS_H_
#define FOLLOW_SETS_H_

#include <vector>
#include <map>
#include "util.h"
#include "constants.h"

/* Follow sets are lists of unique users that a given user is following.
 *
 * We optimize for the case n_following <= FOLLOW_LIMIT1 by placing the
 * elements directly in the array.
 *
 * For n_following > FOLLOW_LIMIT1, FollowSetGrower provides a new follow
 * set to point to, when n_following == FOLLOW_LIMIT1, and grow() is used.
 *
 * To use a follow set, simply use
 */
struct FollowSet {
    int n_following, capacity;
    int* location;

	// Define follow_set1 as an array of fixed-length 'FOLLOW_LIMIT1'
    // When n_following <= FOLLOW_LIMIT1, location points to this array
    int follow_buffer1[FOLLOW_LIMIT1]; // (Short-object-optimization)

    void copy(int* source, int len) {
        for (int i = 0; i < len; i++) {
        	location[i] = source[i];
        }
        for (int i = len; i < capacity; i++){
            location[i] = -1; // Fill rest with -1 to show no action in the array
        }
    }
    void initialize() {
        n_following = 0;
        location = follow_buffer1;
        capacity = FOLLOW_LIMIT1;
        copy(NULL, 0);
    }

    int& operator[](int index) { //** This allows us to index our FollowSet struct as if it were an array.
    	DEBUG_CHECK(within_range(index, 0, n_following), "Operator out of bounds");
    	return location[index];
    }

    // Slow, for debugging/testing purposes:
    void sanity_check() {
    	DEBUG_CHECK(n_following <= capacity, "Capacity bug");
    	for (int i = 0; i < n_following; i++) {
    		DEBUG_CHECK(location[i] != -1, "Invalid follow content");
    	}
    }
};

struct DeletedFollowSet {
	// A follow set buffer that overgrew its boundary, and can be used by another user.
	int* location;
	int capacity;
	DeletedFollowSet(int* loc, int cap) {
		location = loc, capacity = cap;
	}
};

typedef std::vector<DeletedFollowSet> DeletedList;


/* Handles growing follow sets, and allocating buffers when n_following > FOLLOW_LIMIT1. */
struct FollowSetGrower {
	int* memory;
	int used, capacity;
	DeletedList deletions;

	FollowSetGrower() {
		memory = NULL;
		used = 0, capacity = 0;
	}
	void preallocate(int max_bytes) {
		capacity = max_bytes / sizeof(int);
		memory = new int[capacity];
	}
	~FollowSetGrower() {
		delete[] memory;
	}

	// Add a follower to the follow set, potentially growing the array.
	// If we had to grow AND we have run out of allocated memory, we do nothing and return false.
	// Otherwise, we return true.
	bool add_follow(FollowSet& f, int follow) {
    	DEBUG_CHECK(f.n_following <= f.capacity, "Logic error, array should have been grown!");
		if (f.n_following == f.capacity) {
			if (!grow_follow_set(f)) {
				// Not enough room for this FollowSet allocation; this is actually fine. Do nothing here.
				// Virtually all other users will still have a long way to go before hitting their cap.
				return false;
			}
		}
    	DEBUG_CHECK(f.n_following < f.capacity, "Logic error");
		f.location[f.n_following] = follow;
		f.n_following++;
		return true;
	}

private:
	bool grow_follow_set(FollowSet& f) {
		static std::map<void*, void*> allocs;

		int new_capacity = f.capacity * FOLLOW_SET_GROWTH_MULTIPLE;
		int* new_location = NULL;
		// Search recycled buffers
		for (DeletedList::iterator candidate = deletions.begin(); candidate != deletions.end(); ++candidate) {
			// Compatible deleted buffer, recycle it:
			if (candidate->capacity == new_capacity) {
				new_location = candidate->location;
				deletions.erase(candidate);
				break;
			}
		}

		// Go into our block of memory and allocate a follow set:
		if (new_location == NULL) {
			new_location = allocate(new_capacity);
		}
		if (new_location == NULL) {
			return false; // Not enough room!
		}

		// Recycle our old buffer
		if (f.location != f.follow_buffer1) {
			// If we are not pointing to our embedded 'follow_set1', we must be pointing within the 'memory' array
			DEBUG_CHECK(within_range(f.location, memory, memory + capacity), "Logic error");
			deletions.push_back(DeletedFollowSet(f.location, f.capacity));
#ifdef SLOW_DEBUG_CHECKS
			allocs[f.location] = NULL;
#endif
		}
		int* old_loc = f.location;
		f.location = new_location;
		f.capacity = new_capacity;
		f.copy(old_loc, f.n_following);
#ifdef SLOW_DEBUG_CHECKS
		DEBUG_CHECK(allocs[f.location] == NULL, "Logic error!");
		allocs[f.location] = (void*)&f;
		f.sanity_check();
#endif
		return true;
	}
	int* allocate(int cap) {
		if (used + cap > capacity) {
			return NULL; // Not enough room!
		}
		used += cap;
		return &memory[used - cap];
	}
};

#endif
