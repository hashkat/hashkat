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

// Adapted from http://stackoverflow.com/questions/15278343/c11-thread-safe-queue 
#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>

template <class T>
class SafeQueue {
public:
    void enqueue(T t) {
        std::lock_guard<std::mutex> lock(m);
        q.push(t);
        c.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(m);
        while (q.empty()) {
            c.wait(lock);
        }
        T val = q.front();
        q.pop();
        return val;
    }

    T nonblocking_dequeue() {
        std::unique_lock<std::mutex> lock(m);
        if (q.empty()) {
            return T();
        }
        T val = q.front();
        q.pop();
        return val;
    }
private:
    std::queue<T> q;
    mutable std::mutex m;
    std::condition_variable c;
};
#endif
