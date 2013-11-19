/* Configuration and state structures */

#ifndef __STATE_H_ //** Header guards are necessary because C++ has a very low-level include syntax.
#define __STATE_H_ //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include "util.h"

// Hardcoded maxes:
const int MAX_FOLLOWING = 1000;

typedef int FollowArray[MAX_FOLLOWING]; //** The typedef syntax allows us to assign a convenient name to an array of size MAX_FOLLOWING



struct Person {
    int n_following;
    double add_in_time;
    double x_location, y_location; // the x and y locations of the node in the network
    FollowArray follows; //** This is the same as int follows[MAX_FOLLOWING];
    void initialize() {
    	double add_in_time = 0; // the time when users are added into the network
        n_following = 0;
        for (int i = 0; i < MAX_FOLLOWING; i++){
            follows[i] = -1; // Used instead of 0, -1 shows no action in the array
        }
    }
};

struct Network {
    Person* persons; //** This is a pointer - used to create a dynamic array
    Network() {
        persons = NULL;
    }
    ~Network() { //** This defines how to clean-up our Network object; we free the dynamic array
        free(persons);
    }
    inline Person& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
        return persons[index];
    }
    void preallocate(int n_persons) {
        //** This is low-level array allocation, used to be sure we allocate the network array as efficiently as possible:
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
