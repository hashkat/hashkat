#include "tests.h"

#include "../MemPoolVector.h"

const int MEMPOOL_THRESHOLD = 96;

static const int FIRST_ALLOC = FOLLOW_SET_GROWTH_MULTIPLE * MEMPOOL_THRESHOLD;

SUITE(MemPoolVectors) {
    typedef MemPoolVector<int, MEMPOOL_THRESHOLD> MPVec;
    typedef MemPoolVectorGrower<int> MPVecGrower;

	static void check_content(MPVec& f) {
		for (int i = 0; i < f.size; i++) {
			CHECK(f.location[i] == 0);
		}
	}
	TEST(growth) {
	    MPVecGrower grower;
		grower.preallocate(1024 /* bytes*/);

		MPVec f;

		for (int i = 0; i < MEMPOOL_THRESHOLD; i++) {
			CHECK(f.capacity == MEMPOOL_THRESHOLD);
			CHECK(f.size == i);
			CHECK(f.location == f.buffer1);
			CHECK(grower.add_if_possible(f, 0));
			check_content(f);
		}

		CHECK(grower.add_if_possible(f,0)); // We should grow after this
		CHECK(f.location != f.buffer1);
		CHECK(f.capacity == FIRST_ALLOC);
	}

	TEST(memory_exhaustion) {
	    MPVecGrower grower;
		grower.preallocate(FIRST_ALLOC * sizeof(int) /* bytes*/);

		MPVec f;

		for (int i = 0; i < FIRST_ALLOC; i++) {
			CHECK(grower.add_if_possible(f, 0));
			check_content(f);
		}
		CHECK(!grower.add_if_possible(f,0)); // We should NOT have room!
		check_content(f);
		CHECK(f.capacity == FIRST_ALLOC);
		CHECK(f.size == FIRST_ALLOC); // Has been capped
	}
}
