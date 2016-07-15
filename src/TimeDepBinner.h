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

#ifndef TIMEDEPBINNER_H_
#define TIMEDEPBINNER_H_

#include <cstdio>
#include <vector>
#include <algorithm>

#include <iostream>

#include "serialization.h"

#include "dependencies/prettyprint.hpp"
#include "lcommon/perf_timer.h"
#include "lcommon/Timer.h"

struct TimeDepBin {
    typedef std::vector<int> Heap;

    template <typename Checker>
    void add(Checker& checker, int id) {
        heap.push_back(id);
        std::push_heap(heap.begin(), heap.end(), checker);
//        std::cout << "ID: " << id << std::endl;
//        std::cout << heap << std::endl;
    }

    template <typename Checker>
    int pop(Checker& checker) {
        int next = heap.front();
        std::pop_heap(heap.begin(), heap.end(), checker);
        heap.pop_back();
        return next;
    }

    // Whether the max-element passes a check.
    // Returns true if the bin is empty.
    template <typename Checker>
    bool top_check(Checker& checker) {
        if (empty()) {
            return true;
        }
        return checker.check(heap.front());
    }

    size_t size() {
        return heap.size();
    }
    bool empty() {
        return heap.empty();
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(heap);
    }
private:
    Heap heap;
};

class TimeDepBinner {
public:
    TimeDepBinner(int n_bins) : bins(n_bins) {
    }

    template <typename Checker>
    void add(Checker checker, int id) {
        PERF_TIMER();
        int bin = checker.initial_bin(id);
        bins[bin].add(checker, id);
    }

    // Whether the max-element passes a check.
    // Returns true if the bin is empty.
    template <typename Checker>
    void update(Checker checker) {
        for (int i = 0; i < bins.size(); i++) {
            checker.set_bin(i);
            while (!bins[i].top_check(checker)) {
                move_top(checker, i);
            }
        }
    }

    size_t size() {
        size_t sum = 0;
        for (auto& vec : bins) {
            sum += vec.size();
        }
        return sum;
    }

    std::vector<TimeDepBin>& get_bins() {
        return bins;
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(bins);
    }
private:

    // Move the top element to the next bin, or remove it completely.
    template <typename Checker>
    void move_top(Checker& checker, int bin) {
        auto& bin1 = bins[bin];
        int element = bin1.pop(checker);

        if (bin + 1 < bins.size()) {
//            printf("MOVING %d from %d to %d\n", element, bin, bin +1);
            auto& bin2 = bins[bin + 1];
            checker.set_bin(bin + 1);
            bin2.add(checker, element);
            checker.set_bin(bin); // Reset for 'update'
        } else {
//            printf("BOOTING %d\n", element);
        }
    }

    std::vector<TimeDepBin> bins;
};

// Efficient structure for detecting if we should perform some periodic task, in simulated time
struct TimePeriodChecker {
    TimePeriodChecker(double interval, double start_time = 0) {
        next_check_time = start_time + interval;
        this->interval = interval;
    }

    bool has_past(double t) {
        if (t < next_check_time) {
            return false;
        }
        while (t >= next_check_time) {
            next_check_time += interval;
        }
        return true;
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(next_check_time, interval);
    }
private:
    double next_check_time, interval;
};

// Efficient structure for detecting if we should perform some periodic task, in real time
struct RealTimePeriodChecker {
    // Duration is in real-minutes, the standard input configuration
    RealTimePeriodChecker(double minute_duration) {
        microsecond_duration = minute_duration * 60 * 1000;
    }
    bool has_past() {
       auto time_elapsed = timer.get_microseconds(); 
       auto time_diff = time_elapsed + remainder - microsecond_duration;
       if (time_diff > 0) {
           timer.start();
           // Drift the remainder by how far away we were from a microsecond_duration
           remainder += time_elapsed - microsecond_duration;
           return true;
       }
       return false;
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(microsecond_duration);
    }
private:
    Timer timer;
    double remainder = 0;
    double microsecond_duration = 0;
};

// Is configured to either check a real or simulated period of time.
struct RealOrSimulatedTimePeriodChecker {
    RealOrSimulatedTimePeriodChecker(const TimePeriodChecker&     simulated_time_period_checker) : 
        use_simulated_time(true), 
        simulated_time_period_checker(simulated_time_period_checker), 
        real_time_period_checker(0) {
    }
    RealOrSimulatedTimePeriodChecker(const RealTimePeriodChecker& real_time_period_checker) : 
        use_simulated_time(false), 
        simulated_time_period_checker(0), 
        real_time_period_checker(real_time_period_checker) {
    }
    bool has_past(double t) {
        return use_simulated_time ? simulated_time_period_checker.has_past(t) : real_time_period_checker.has_past();
    }

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(use_simulated_time, simulated_time_period_checker, real_time_period_checker);
    }
private:
    bool use_simulated_time = true;
    TimePeriodChecker simulated_time_period_checker;
    RealTimePeriodChecker real_time_period_checker;
};


#endif
