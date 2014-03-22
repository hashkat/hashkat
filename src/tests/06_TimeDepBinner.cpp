#include <fstream>
#include <iostream>
#include <map>

#include "tests.h"

#include "TimeDepBinner.h"

using namespace std;

SUITE(TimeDepBinner) {

    struct TimeFunction {
        TimeFunction(double time) {
            this->time = time;
            bin = -1;
        }
        void set_bin(int bin) {
            this->bin = bin;
        }

        double time;
        int bin;

        // Bin-move-check function for TimeDepBinner:
        bool check(int id) {
//            printf("CHECKING %d:%d vs %.2f\n", id, bin, time);
            return (bin > time);
        }

        // Comparison function for TimeDepBinner:
        bool operator()(int a, int b) {
            return a > b;
        }

        int initial_bin(int a) {
            return (int)time;
        }
    };

    TEST(basics) {
        const int N_BINS = 10;
        TimeDepBinner binner(N_BINS);
        TimeFunction func(0);
        for (int i = 0; i < 1000; i++) {
            binner.add(func, i);
        }
        for (double t = 0.0; t < 10.0; t++) {
            TimeFunction func(t);
            binner.update(func);

            int bin = (int)t;
            auto& bins = binner.get_bins();
            printf("At time %.2f...\n", t);
            printf("Bin %d has %d\n",  bin, (int)bins[bin].size());
            if (bin + 1 < bins.size()) {
                printf("Bin %d has %d\n",  bin+1, (int)bins[bin+1].size());
            }

        }
        printf("%d size \n", (int)binner.size());
        // Should be empty:
        CHECK((int)binner.size() == 0);
    }

}
