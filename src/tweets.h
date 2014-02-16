#ifndef TWEETS_H_
#define TWEETS_H_

#include <vector>

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "RateVectorTree.h"
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
