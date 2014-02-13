#ifndef TWEETS_H_
#define TWEETS_H_

#include "lcommon/smartptr.h"

#include "entity.h"

struct Tweet {
    struct Content {
        Language language; // The language that the tweet is in

    };

    smartptr<Content> contentPointer;
};



#endif
