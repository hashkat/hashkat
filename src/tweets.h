#ifndef TWEETS_H_
#define TWEETS_H_

#include <vector>

#include "lcommon/smartptr.h"

#include "cat_classes.h"

#include "entity.h"

struct OriginalTweet {
    Language language; // The language that the tweet is in
    double humour; // The humour level of the tweet, 0 to 1
    int author_id; // The entity that created the original content
    OriginalTweet(Language language, double humour, int author_id) :
            language(language), humour(humour), author_id(author_id) {
    }
};

// A tweet is an orignal tweet if tweeter_id == content.author_id
struct Tweet {
    // The entity broadcasting the tweet
    int tweeter_id;
    smartptr<OriginalTweet> content;

    Tweet(int tweeter_id, const smartptr<OriginalTweet>& content) :
            tweeter_id(tweeter_id), content(content) {
    }

    bool is_original_tweet() {
        return (content->author_id == tweeter_id);
    }

    bool is_retweet() {
        return !is_original_tweet();
    }
    OriginalTweet* operator->() {
        return content.get();
    }
};

template <typename T, int N_ELEM, int N_CHILDREN = 10>
struct RateVectorTree {
    typedef int ref_t;

    struct RateVec {
        double tuple[N_ELEM];
        double tuple_sum;
        RateVec() {
            for (int i = 0; i < N_ELEM; i++) {
                tuple[i] = 0;
            }
            tuple_sum = 0;
        }
        void add(RateVec& o) {
            for (int i = 0; i < N_ELEM; i++) {
                tuple[i] += o.tuple[i];
            }
            tuple_sum += o.tuple_sum;
        }
        void sub(RateVec& o) {
            for (int i = 0; i < N_ELEM; i++) {
                tuple[i] -= o.tuple[i];
            }
            tuple_sum -= o.tuple_sum;
        }
        RateVec delta(RateVec& o) {
            RateVec ret = o;
            ret.sub(*this);
            return ret;
        }
    };

    struct Node {
        ref_t parent; // -1 if root
        short depth;
        bool is_used; // Default false
        T data;
        RateVec data_rates, child_sum;

        ref_t children[N_CHILDREN]; // -1 if not allocated

        bool has_vacancy() {
            for (int i = 0; i < N_CHILDREN; i++) {
                if (children[i] == -1) {
                    return true; // At least one free slot still
                }
            }
            return false;
        }
        void link_to_parent(ref_t parent, int depth) {
            this-> parent = parent;
            this-> depth = depth;
        }
        // If a child node was allocated, place it in child_output, otherwise place self.
        // Returns true if all spots are used up after this operation
        // Does NOT propagate changes up the tree
        bool use_vacancy(RateVectorTree& tree, ref_t self, ref_t& child_output) {
            if (!is_used) {
                child_output = self;
                // We are done here
            }
            int i = 0;
            for (; i < N_CHILDREN; i++) {
                if (children[i] == -1) {
                    if (!is_used) {
                        return false; // At least one free slot still
                    }
                    ref_t ref = tree.alloc_node();
                    children[i] = ref;
                    tree.get(ref).link_to_parent(self, depth + 1);
                }
            }

            for (; i < N_CHILDREN; i++) {
                if (children[i] == -1) {
                    return false; // At least one free slot still
                }
            }
            return true; // All slots have been used, should remove from vacancy list
        }
        Node() {
            parent = -1;
            is_used = false;
            depth = 0;
            for (int i =0; i < N_CHILDREN; i++) {
                children[i] = -1;
            }
        }

        void rate_add(RateVec& vec) {
            child_sum.add(vec);
        }

        void rate_sub(RateVec& vec) {
            child_sum.sub(vec);
        }
    };
    RateVectorTree() {
        size = 0;
        node_pool.resize(1); // Allocate root
    }
    Node& get(ref_t handle) {
        return node_pool[handle];
    }

    ref_t add(const T& data, const cats::RateTuple<N_ELEM>& tuple) {
        ref_t node = alloc_node();
        size++;
        return node;
    }

private:
    ref_t alloc_node() {
        int size = node_pool.size();
        node_pool.resize(size + 1);
        return size;
    }
    // Find a vacant parent to link our node to
    // Does NOT propagate changes up the tree
    ref_t find_vacancy() {
        // A node with some sort of vacancy *must* exist
        for (int i = 0; i < vacancy_list.size(); i++) {
            std::vector<ref_t>& sub_list = vacancy_list[i];
            if (!sub_list.empty()) {
                ref_t vacancy = sub_list.back();
                ref_t new_node;
                bool used_up = get(vacancy).use_vacancy(*this, vacancy, new_node);
                if (used_up) {
                    sub_list.pop_back(); // Ensure proper cleanup
                }
                return new_node;
            }
        }
        ASSERT(false, "Should never happen!");
        return -1;
    }
    size_t size;
    std::vector<Node> node_pool; // 0 is the root node
    std::vector<std::vector<ref_t> > vacancy_list;
};

//struct TweetStore {
//    struct NodePool {
//
//    };
//
//    template <int N_ELEM, int N_CHILDREN = 10>
//    struct LinkedRateTupleNode {
//
//    template <int N_ELEM, int N_CHILDREN = 10>
//    struct LinkedRateTupleNode {
//        LinkedRateTupleNode* parent; // NULL if root
//        cats::RateTuple<N_ELEM> tuple;
//        double tuple_sum;
//        double children_sum;
//        LinkedRateTupleNode* children[N_CHILDREN];
//    };
//    struct DistanceComponent: cats::StaticLeafClass<Tweet, N_BIN_DISTANCE> {
//        int classify(AnalysisState& N, Tweet& tweet); // Defined in tweets.cpp
//        std::string cat_name(AnalysisState& N, int bin);
//        template <typename AnyT>
//        double get(AnyT& notused, int bin) {
//            return 1; // TODO: Not used
//        }
//    };
//    struct HumourComponent: cats::StaticTreeClass<DistanceComponent, N_BIN_HUMOUR> {
//        int classify(AnalysisState& N, Tweet& tweet); // Defined in tweets.cpp
//        std::string cat_name(AnalysisState& N, int bin);
//    };
//
//    // Related to how old the tweet is
//    struct TimeComponent: cats::TreeClass<HumourComponent> {
//        int classify(AnalysisState& N, Tweet& tweet); // Defined in tweets.cpp
//    };
//    // Related to how much time has passed
//    struct FollowOrderComponent: cats::TreeClass<TimeComponent> {
//        int classify(AnalysisState& N, Tweet& tweet); // Defined in tweets.cpp
//    };
//
//    typedef FollowOrderComponent Rates;
//    typedef Rates::CatGroup CatGroup;
//
//    void add(const Tweet& tweet) {
//        for (int i =0; i < N_BIN_PREFERENCE_CLASS; i++) {
////            stores[i]
//        }
//    }
//
//    Rates rates[N_BIN_PREFERENCE_CLASS];
//    // Store one group for every preference bin
//    CatGroup stores[N_BIN_PREFERENCE_CLASS];
//};

#endif
