#ifndef TWEETS_H_
#define TWEETS_H_

#include "lcommon/smartptr.h"

#include "entity.h"

struct OriginalTweet {
    Language language; // The language that the tweet is in
    int author_id; // The entity that created the original content
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
};

struct TweetStore {
    struct PreferenceClassComponent: cats::StaticLeafClass<int, MAX_PREFERENCE_CLASSES> {
        int classify(AnalysisState& N, int entity_id); // Defined in entity.cpp
        template <typename AnyT>
        double get(AnyT& notused, int bin) {
            return 1; // TODO: Not used
        }
    };
    struct LanguageComponent: cats::StaticTreeClass<PreferenceClassComponent, N_LANGS> {
        int classify(AnalysisState& N, int entity_id); // Defined in entity.cpp
    };
};

#endif
