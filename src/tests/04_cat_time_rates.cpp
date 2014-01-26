#include <vector>
#include <map>
#include "tests.h"

#include "dependencies/mtwist.h"

#include "cat_nodes.h"
#include "cat_classes.h"

using namespace std;

SUITE(cat_time_rates) {

    struct MockEntity {
        int entity_type;
        double creation_time;
        MockEntity(int et, double creation_time) :
                entity_type(et), creation_time(creation_time) {

        }
    };

    typedef vector<MockEntity> MockNetwork;

    struct EntityTypeClass: cats::LeafClass<int> {
        EntityTypeClass() {
        }
        int classify(MockNetwork& N, int elem) {
            return N[elem].entity_type;
        }
    };

    struct TimeClass: cats::TreeClass<EntityTypeClass> {
        TimeClass(double res) {
            resolution = res;
        }
        int classify(MockNetwork& N, int elem) {
            return int(N[elem].creation_time / resolution);
        }
        double resolution; // At what resolution to bin
    };

    TEST(time_dependent_rates) {
        using namespace cats;

        const int TIME_CAT_FREQ = 1;

        MockNetwork N;
        TimeClass time_class(TIME_CAT_FREQ);
        TimeClass::CatGroup cat_group;

        vector<vector<double> > rates;
        for (int month = 0; month < 10; month++) {
            const int ENTITY_TYPES = 5;
            vector<double> subrates;
            for (int et = 0; et < ENTITY_TYPES; ++et) {
                subrates.push_back(month + 1);
                N.push_back(MockEntity(et, month));
                N.push_back(MockEntity(et, month));
            }
            rates.push_back(subrates);
        }
        time_class.fill_rates(rates);

        for (int i = 0; i < N.size(); i++) {
            double delta = 0.0;
            bool added = cat_group.add(N, time_class, delta, i);
            CHECK(added);
//        CHECK(delta == i);
        }
        // Compare print out for minor details
        cat_group.print(N, time_class);
        CHECK(cat_group.get_total_rate() == 550);
        printf("MOVING THE TIME COHORTS\n");
        cat_group.shift_and_recalc_rates(N, time_class);
        // Simulate a month going by, in a model where rates go up over time.
        CHECK(cat_group.get_total_rate() == 640);
        cat_group.print(N, time_class);

        MTwist rng;
        rng.init_genrand(29);
        int elem = -1;
        for (int i = 0; i < 100; i++) {
            cat_group.pick_random_weighted(rng, elem);
            printf("Random pick: %d\n", elem);
        }
        {
            // Iteration sub-test
            int i = 0, elem = -1;
            for (TimeClass::iterator it; cat_group.iterate(it, elem);) {
                printf("Element %d: %d\n", i++, elem);
            }
        }
    }
}

