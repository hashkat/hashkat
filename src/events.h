#ifndef EVENT_H_
#define EVENT_H_

#include "dependencies/google/sparse_hash_set"
#include "cat_nodes.h"
#include "util.h"

typedef google::sparse_hash_set<int, cats::Hasher> UsedEntities;

// 0 - add, 1 - follow, 2 - tweet
const int number_of_diff_events = 2;

struct Rate_Function {
    std::string function_type;
    double slope, y_intercept, const_val, amplitude, exp_factor;
    std::vector<double> monthly_rates;
    Rate_Function() {
        slope = y_intercept = const_val = amplitude = exp_factor = -1;
        function_type = "not specified";
    }

    READ_WRITE(rw) {
        rw << function_type << slope << y_intercept << const_val << amplitude << exp_factor << monthly_rates;
    }
};

struct Add_Rates {
    Rate_Function RF;
};

#endif
