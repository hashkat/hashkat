#include "lcommon/strformat.h"

#include "analyzer.h"
#include "tweets.h"
#include "entity.h"

double TweetRateDeterminer::get_age(Tweet& tweet) {
    return (tweet.creation_time - state.time);
}
