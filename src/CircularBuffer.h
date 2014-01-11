/*
 * CircularBuffer.h
 *
 *  Created on: Dec 19, 2013
 *      Author: adomurad
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include "util.h"
#include <algorithm>

//** For KR and IT,
//** **Creating** templates has a cost model that is only beneficial if you understand them
//** completely & they are definitely not an easy feature to learn.
//** **Using** them however is very easy and convenient.

//** HOWEVER -- I strongly recommend eventually learning simple usages of templates because
//** they are an *excellent code reuse tool*

//** Usage comments follow.

// CircularBuffer implements a buffer that deletes the oldest element
// when capacity is reached. The capacity is given directly in the type,
// for example:
// CircularBuffer<int, 4> circular_buffer;
// We then add elements, eg circular_buffer.add(1);
//
// We access the elements by indexing copying the contents to an array (which returns the size)

template <typename T, int cap>
class CircularBuffer {
public:
    CircularBuffer() {
        start = end = 0;
    }

    void add(const T& val) {
        buffer[end] = val;
        end = (end == cap) ? 0 : end + 1;
        // Optimize for the always-full case, which happens the moment it is filled.
        if (end == start) {
            // If we are full, push the start and end together in a circular fashion
            start = (start + 1) % (cap+1);
        }
    }

    bool check_recent(T& t) {
        if (start == end) {
            return false;
        }
        t = buffer[end-1];
        return true;
    }

    // Copies the elements into an array, returning the number.
    // The passed array should have capacity 'cap'.
    // The size of region copied is returned.
    int copy(int* arr) {
        int n = 0;
        int s_cap = (start <= end) ? std::min(cap + 1, (int)end) : cap + 1;
        for (int i = start; i < s_cap; i++) {
            arr[n] = buffer[i];
            n++;
        }
        if (start > end) {
            // This is the 'circular case', the additional end elements are near the start:
            for (int i = 0; i < end; i++) {
                arr[n] = buffer[i];
                n++;
            }
        }
        return n;
    }

    short start, end;
private:
    T buffer[cap + 1]; // Contains sentinel element. Shielded from direct access
};

#endif /* CIRCULARBUFFER_H_ */
