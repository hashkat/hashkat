/* Configuration and state structures */

#ifndef __STATE_H_
#define __STATE_H_

#include <cstdlib>
#include "util.h"

// Hardcoded maxes:
const int MAX_FOLLOWING = 1000;

typedef int FollowArray[MAX_FOLLOWING];

struct Person {
    int n_following;
    FollowArray follows;
    void initialize() {
        n_following = 0;
        for (int i = 0; i < MAX_FOLLOWING; i++){
            follows[i] = -1; // Used instead of 0, -1 shows no action in the array
        }
    }
};

struct Network {
    Person* persons;
    Network() {
        persons = NULL;
    }
    ~Network() {
        free(persons);
    }
    inline Person& operator[](int index) {
        return persons[index];
    }
    void preallocate(int n_persons) {
        persons = (Person*)malloc(sizeof(Person) * n_persons);
        // This is very likely to be a large allocation, check for failures:
        if (persons == NULL) {
            panic("Network::preallocate failed");
        }
        for (int i = 0; i < n_persons; i++) {
            persons[i].initialize();
        }
    }
};

#endif
