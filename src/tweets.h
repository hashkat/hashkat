#ifndef TWEETS_H_
#define TWEETS_H_

#include <vector>
#include <cstdio>

#include "lcommon/smartptr.h"
#include "mtwist.h"

#include "cat_classes.h"

#include "events.h"
#include "entity_properties.h"
#include "CircularBuffer.h"

#include "DataReadWrite.h"

#include "FollowerSet.h"

// information for when a user tweets
struct TweetContent {
    TweetType type = (TweetType)-1;
    double time_of_tweet = -1;
    UsedEntities used_entities;
    Language language = N_LANGS; // Set to invalid
    int ideology_bin = -1; // 0 == no ideology
    int hashtag_bin = -1;
    int id_original_author = -1; // The entity that created the original content

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
        rw << time_of_tweet << language << ideology_bin << hashtag_bin << id_original_author;
    }
};

// Represents a tweet, either original, or a rebroadcast.
struct Tweet {
    // The entity broadcasting the tweet
    // A tweet is an orignal tweet if tweeter_id == content.author_id
    int id_tweeter = -1;

    // The 'linking' entity the tweet was retweeted from (a following of id_tweeter)
    // Equal to id_tweeter if the tweet was original content.
    int id_link = -1;
    // The generation of the tweet, 0 if the tweet was original content
    int generation = -1;
    // A tweet is an orignal tweet if tweeter_id == content.author_id
    smartptr<TweetContent> content;

    // The time the tweet was tweeted
    double creation_time = 0;

    // Based on creation_time and the current time
    // Determines the 'omega' observation PDF dropoff rate of
    // the tweet's retweetability.
    int retweet_time_bin = -1;
    
    // does this tweet include a hashtag, set false as default
    bool hashtag = false;

    // Next time to consider rebinning, always more than creation_time
    double retweet_next_rebin_time = -1;

    /* Rates with which this tweet is retweeted: */
    FollowerSet::Weights react_weights;

    explicit Tweet(const smartptr<TweetContent>& content = smartptr<TweetContent>()) {
        this->content = content;
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

struct AnalysisState;

struct MostPopularTweet { 
    // this is the most retweeted tweet
    Tweet most_popular_tweet;
    // The max number of retweets for one tweet
    int global_max = 0;

    READ_WRITE(rw) {
        rw << global_max;
        most_popular_tweet.visit(rw);
    }
};

struct HashtagGroup {
    // circular buffer with 10 elements
    CircularBuffer<int, 10> circ_buffer;
};

struct HashTags {
    // this is the set of bins for idealogies and regions
    HashtagGroup hashtag_groups[N_BIN_IDEOLOGIES][N_BIN_REGIONS];
    
    READ_WRITE(rw) {
        for (auto& outer : hashtag_groups) {
            for (auto& groups : outer) {
                // Note: works because HashtagGroup contains no pointers internally:
                rw << groups;
            }

        }
    }
};

#endif
