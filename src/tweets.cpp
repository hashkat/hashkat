#include "lcommon/strformat.h"

#include "analyzer.h"
#include "tweets.h"
#include "entity.h"

double TweetRateDeterminer::get_age(Tweet& tweet) {
    double age = (state.time - tweet.creation_time);
    DEBUG_CHECK(age >= 0.0, "Cannot have negative ages!");
    return age;
}
