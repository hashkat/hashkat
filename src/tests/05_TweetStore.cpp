#include <fstream>
#include <iostream>
#include <map>

#include "tests.h"

#include "tweets.h"

using namespace std;

SUITE(TweetStore) {
    struct RateV {
        int val;
        RateV(int val = -999) :
            val(val) {
        }
        void print() {
            printf("  Stored: %d\n", val);
        }
    };

    typedef RateTree<RateV, 1> Tree;

    static void basic_check(Tree& vec_tree) {
        std::vector<Tree::Node*> vec = vec_tree.as_node_vector();
        CHECK(vec.size() == vec_tree.size());
        double sum = vec_tree.rate_summary().tuple_sum;
        double alt_sum = 0.0;
        for (int i = 0; i < vec.size(); i++) {
            CHECK(vec[i]->is_leaf);
            CHECK(vec[i]->depth > 0);
            CHECK(vec[i]->is_allocated);
            alt_sum += vec[i]->rates.tuple_sum;
        }
        CHECK(sum == alt_sum);
    }

    TEST(RateTree) {
        MTwist rng;
        RateVec<1> vec;
        Tree vec_tree;
        vec.tuple[0] = 1.0;
        vec.tuple_sum = 1.0;
        for (int z = 0; z < 25; z++) {
            std::vector<int> elemsA;
            std::vector<int> elemsB;
            for (int i = 0; i < 91; i++) {
                int elem = vec_tree.add(i, vec);
                basic_check(vec_tree);
                if (i % 2 == 0) {
                    elemsA.push_back(elem);
                } else {
                    elemsB.push_back(elem);
                }
                int r = vec_tree.pick_random_weighted(rng);
            }
            vec_tree.print();
            for (int i = 0; i < elemsA.size(); i++) {
                vec_tree.remove(elemsA[i]);
                basic_check(vec_tree);
            }
            for (int i = 0; i < 600; i++) {
                int elem = vec_tree.add(99985*(i+1), vec);
                basic_check(vec_tree);
                elemsB.push_back(elem);
            }
            printf("AFTER\n");
            vec_tree.print();
            for (int i = 0; i < elemsB.size(); i++) {
                vec_tree.remove(elemsB[i]);
                basic_check(vec_tree);
            }
        }
    }
}
