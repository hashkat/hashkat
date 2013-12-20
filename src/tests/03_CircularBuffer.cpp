#include <dependencies/UnitTest++.h>

#include "../CircularBuffer.h"

template <typename T, int n>
static int check(CircularBuffer<T, n>& buffer, int a=0, int b=0, int c=0, int d=0) {
    int read[4] = {0,0,0,0};
    int N = buffer.copy(read);
    CHECK(read[0] == a);
    CHECK(read[1] == b);
    CHECK(read[2] == c);
    CHECK(read[3] == d);
    return N;
}

SUITE(CircularBuffers) {
	TEST(correctness) {
	    CircularBuffer<int, 4> buffer;
	    buffer.add(1);
	    check(buffer, 1);
	    buffer.add(2);
	    check(buffer, 1, 2);
	    buffer.add(3);
	    check(buffer, 1, 2, 3);
	    buffer.add(4);
	    int N = check(buffer, 1, 2, 3, 4);
        CHECK(N == 4);
	    buffer.add(5);
	    check(buffer, 2, 3, 4, 5);
	    for (int i = 6; i < 100; i++) {
	        buffer.add(i);
	        int N = check(buffer, i - 3, i - 2, i - 1, i);
	        CHECK(N == 4);
	    }
	}
}
