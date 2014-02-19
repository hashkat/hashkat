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
    TEST(RateTree) {
        MTwist rng;
        typedef RateTree<RateV, 1> Tree;
        Tree vec_tree;
        RateVec<1> vec;
        vec.tuple[0] = 1.0;
        vec.tuple_sum = 1.0;
        for (int z = 0; z < 25; z++) {
            std::vector<int> elemsA;
            std::vector<int> elemsB;
            for (int i = 0; i < 91; i++) {
                int elem = vec_tree.add(i, vec);
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
            }
            for (int i = 0; i < 600; i++) {
                int elem = vec_tree.add(99985*(i+1), vec);
                elemsB.push_back(elem);
            }
            printf("AFTER\n");
            vec_tree.print();
            for (int i = 0; i < elemsB.size(); i++) {
                vec_tree.remove(elemsB[i]);
            }
        }
    }
}
