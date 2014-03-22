#ifndef TWEETS_H_
#define TWEETS_H_

#include <vector>
#include <cstdio>

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "cat_classes.h"

#include "events.h"
#include "entity_properties.h"

#include "DataReadWrite.h"

// information for when a user tweets
struct TweetContent {
    double time_of_tweet;
    UsedEntities used_entities;
    Language language;
    int humour_bin;
    int id_original_author; // The entity that created the original content
    TweetContent() {
        time_of_tweet = -1;
        language = N_LANGS; // Set to invalid
        id_original_author = -1;
        humour_bin = -1;
    }

    READ_WRITE(rw) {
        std::vector<int> ids;
        if (rw.is_writing()) {
            for (int id : used_entities) {
                ids.push_back(id);
            }
        }
        rw << ids;
        if (rw.is_reading()){
            for (int id : ids) {
                size_t prev_size = used_entities.size();
                used_entities.insert(id);
                ASSERT(used_entities.size() > prev_size, "'id' should be unique!");
            }
        }
        rw << time_of_tweet << language << id_original_author << humour_bin;
//        printf("GOT AT %.2f %d %d size=%d\n", time_of_tweet, language,
//                id_original_author, used_entities.size());
    }
};

// information for when a user retweets

// TODO Clear all temporary comments eventually
// A tweet is an orignal tweet if tweeter_id == content.author_id
struct Tweet {
    // The entity broadcasting the tweet
    int id_tweeter;

    // The 'linking' entity the tweet was retweeted from (a following of id_tweeter)
    // Equal to id_tweeter if the tweet was original content.
    int id_link;
    // The generation of the tweet, 0 if the tweet was original content
    int generation;
    // A tweet is an orignal tweet if tweeter_id == content.author_id
    smartptr<TweetContent> content;

    // The time the tweet was tweeted
    double creation_time;

    // Based on creation_time and the current time
    // Determines the 'omega' observation PDF dropoff rate of
    // the tweet's retweetability.
    int retweet_time_bin;

    // Next time to consider rebinning, always more than creation_time
    double retweet_next_rebin_time;

    explicit Tweet(const smartptr<TweetContent>& content = smartptr<TweetContent>()) : content(content) {
        id_tweeter = -1;
        id_link = -1;
        generation = -1;
        creation_time = 0;
        retweet_time_bin = -1;
        retweet_next_rebin_time = -1;
    }

    void print() {
        printf("(Tweeter = %d, Link = %d, Original Author = %d, Created = %.2f\n)",
                id_tweeter, id_link, content->id_original_author, creation_time);
    }

    READ_WRITE(rw) {
        rw << id_tweeter << creation_time << retweet_time_bin << retweet_next_rebin_time << id_link << generation;
        rw.visit_smartptr(content);
    }
};

typedef std::vector<Tweet> TweetList;

struct AnalysisState;

struct MostPopularTweet { 
    // this is the most retweeted tweet
    Tweet most_popular_tweet;
    // The max number of retweets for one tweet
    int global_max;
    MostPopularTweet() {
        global_max = 0;
    }
    READ_WRITE(rw) {
        rw << global_max;
        most_popular_tweet.visit(rw);
    }
};

#endif
