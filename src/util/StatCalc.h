#ifndef STATGATHERER_H_
#define STATGATHERER_H_

#include <map>
#include <string>

#include <cstdio>
#include <cmath>
#include <algorithm>

#include "util.h"

// Describes a distributions average, min, max, and standard deviation
// Uses fixed memory and is numerically-stable (ie, does not create increasingly large intermediate numbers)
struct StatCalc {
    double min, max, average;
    double sum;
    double q_value; // standard deviation = sqrt(q_value / n_elements)
    double n_elements;

    StatCalc() {
        min = 0, max = 0;
        average = 0, sum = 0;
        q_value = 0, n_elements = 0;
    }

    void add_element(double element) {
        n_elements++;
        sum += element;
        max = std::max(max, element);
        min = std::min(min, element);
        // Update the intermediate 'Q-value' of our standard deviation formula
        double new_average = sum / n_elements;
        q_value += (element - average) * (element - new_average);
        average = new_average;
    }

    double standard_deviation() {
        if (n_elements == 0) {
            return 0;
        }
        return sqrt(q_value / n_elements);
    }

    void print_summary() {
        printf("%d Elements in Distribution\nMin %.2f Max %.2f Sum %.2f\nAvg %.2f (+-%.2f)\n",
                int(n_elements), min, max, sum, average, standard_deviation());
    }
};

struct DiscreteDist {
    std::map<int, int> totals;
    int n_elements;
    void add_element(int element) {
        totals[element]++;
        n_elements++;
    }
    void print_bin(const std::string& name, int bin) {
        printf("%s: %d (%g%%)\n", name.c_str(), totals[bin], 100 * totals[bin] / double(n_elements));
    }
};

#endif
