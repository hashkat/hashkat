#ifndef EVENT_H_
#define EVENT_H_

#include "dependencies/google/sparse_hash_set"
#include "util.h"

typedef google::sparse_hash_set<int, cats::Hasher> UsedEntities;

// The different events covered by the statistics gathering
enum EventID {
    // AD: TODO move all rates into this form
    EV_FOLLOWBACK,
    N_EVENTS // Not a real entry; evaluates to amount of events
};

struct EntityEventStats {
    double& operator[](int index) {
        DEBUG_CHECK(index >= 0 && index < N_EVENTS, "Out of bounds!");
        return stats_array[index];
    }
    EntityEventStats() {
        memset(stats_array, 0, sizeof(double) * N_EVENTS);
    }
private:
    // AD: TODO move all rates into this struct
    double stats_array[N_EVENTS];
};

// 0 - add, 1 - follow, 2 - tweet
const int number_of_diff_events = 3;

struct Rate_Function {
    std::string function_type;
    double slope, y_intercept, const_val, amplitude, exp_factor;
    std::vector<double> monthly_rates;
    Rate_Function() {
        slope = y_intercept = const_val = amplitude = exp_factor = -1;
        function_type = "not specified";
    }
};

struct Add_Rates {
    Rate_Function RF;
};

#endif
