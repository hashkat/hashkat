#ifndef TIMEDEPBINNER_H_
#define TIMEDEPBINNER_H_

#include <cstdio>
#include <vector>
#include <algorithm>

#include <iostream>

#include "DataReadWrite.h"

#include "dependencies/prettyprint.hpp"
#include "lcommon/perf_timer.h"

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

    READ_WRITE(rw) {
        rw << heap;
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

    READ_WRITE(rw) {
        rw.visit_objs(bins);
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

// Efficient structure for detecting if we should perform some periodic task
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

    READ_WRITE(rw) {
        rw << next_check_time, interval;
    }
private:
    double next_check_time, interval;
};

#endif
