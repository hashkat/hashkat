#include <fstream>
#include <iostream>
#include <map>

#include "tests.h"

#include "tweets.h"

using namespace std;

SUITE(TweetStore) {
    TEST(RateVectorTree) {
        MTwist rng;
        typedef RateVectorTree<int, 1> Tree;
        typedef Tree::RateVec RateVec;
        Tree vec_tree;
        RateVec vec;
        vec.tuple[0] = 1.0;
        vec.tuple_sum = 1.0;
        std::vector<int> elems;
        for (int i = 0; i < 91; i++) {
            int elem = vec_tree.add(i, vec);
            if (i % 2 == 0) {
                elems.push_back(elem);
            }
            int r = vec_tree.pick_random_weighted(rng);
        }
        vec_tree.print();
        for (int i = 0; i < elems.size(); i++) {
            vec_tree.remove(elems[i]);
        }
        for (int i = 0; i < 600; i++) {
            vec_tree.add(99985*(i+1), vec);
        }
        printf("AFTER\n");
        vec_tree.print();
    }
}
