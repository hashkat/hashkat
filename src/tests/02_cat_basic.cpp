#include <vector>
#include <map>
#include "tests.h"

#include "dependencies/mtwist.h"

#include "cat_nodes.h"
#include "cat_classes.h"

using namespace std;

SUITE(cat_basic) {

struct MockNetwork {
};

TEST(leaf_node) {
    using namespace cats;
    MockNetwork N;
    LeafNode<int> leaf;
    double delta = 0.0;
    for (int elem = 0; elem < 1000; elem++) {
        CHECK(leaf.add(N, 1.0, delta, elem));
        CHECK(!leaf.add(N, 1.0, delta, elem));
    }
    CHECK(leaf.get_total_rate() == 1000);
    CHECK(delta == 1.0);
}

struct MockLeafClass : cats::LeafClass<int> {
    MockLeafClass(int mod) {
        modulo = mod;
    }
    int classify(MockNetwork& N, int elem) {
        return (elem % modulo);
    }
    int modulo;
};

TEST(tree_node) {
    using namespace cats;

    MockNetwork N;

    MockLeafClass classifier(10);
    vector<double> rates;
    for (int i = 0; i < 10; i++) {
        rates.push_back(i);
    }
    classifier.fill_rates(rates);
    MockLeafClass::CatGroup cat_group;

    double delta = 0.0;
    double prev_total = 0.0;
    for (int elem = 0; elem < 10; elem++) {
        bool new_elem = cat_group.add(N, classifier, delta, elem);
        CHECK(new_elem);
        double new_total = cat_group.get_total_rate();
        CHECK(new_total == prev_total + delta);
        prev_total = new_total;
    }
}

}
