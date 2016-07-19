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
#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include "util.h"
#include <algorithm>

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
        if (LIKELY(end == start)) {
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
    bool empty() {
       return (start == end);
    }
    T pick_random_uniform(MTwist& rng) {
    	T temp[cap];
    	int n = copy(temp);
        return temp[rng.rand_int(n)];
    }

    short start, end;

    template <typename Archive>
    void serialize(Archive& ar) {
        size_t size = cap + 1;
        ar( cereal::make_size_tag(size) );
        ASSERT(size == cap + 1, "Size not matched.");
        for (T& elem : buffer) {
            ar( elem );
        }
    }
private:
    T buffer[cap + 1]; // Contains sentinel element. Shielded from direct access
};

#endif /* CIRCULARBUFFER_H_ */
