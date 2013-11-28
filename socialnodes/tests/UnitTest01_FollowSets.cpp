#include <dependencies/UnitTest++.h>

#include "../follow_set.h"

static const int FIRST_ALLOC = FOLLOW_SET_GROWTH_MULTIPLE * FOLLOW_LIMIT1;

SUITE(FollowSets) {
	static void check_content(FollowSet& f) {
		for (int i = 0; i < f.n_following; i++) {
			CHECK(f.location[i] == 0);
		}
	}
	TEST(growth) {
		FollowSetGrower grower;
		grower.preallocate(1024 /* bytes*/);

		FollowSet f;
		f.initialize();

		for (int i = 0; i < FOLLOW_LIMIT1; i++) {
			CHECK(f.capacity == FOLLOW_LIMIT1);
			CHECK(f.n_following == i);
			CHECK(f.location == f.follow_buffer1);
			CHECK(grower.add_follow(f, 0));
			check_content(f);
		}

		CHECK(grower.add_follow(f,0)); // We should grow after this
		CHECK(f.location != f.follow_buffer1);
		CHECK(f.capacity == FIRST_ALLOC);
	}

	TEST(memory_exhaustion) {
		FollowSetGrower grower;
		grower.preallocate(FIRST_ALLOC * sizeof(int) /* bytes*/);

		FollowSet f;
		f.initialize();

		for (int i = 0; i < FIRST_ALLOC; i++) {
			CHECK(grower.add_follow(f, 0));
			check_content(f);
		}
		CHECK(!grower.add_follow(f,0)); // We should NOT have room!
		check_content(f);
		CHECK(f.capacity == FIRST_ALLOC);
		CHECK(f.n_following == FIRST_ALLOC); // Has been capped
	}
}
