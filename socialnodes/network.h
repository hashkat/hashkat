/* Network state structures */

#ifndef __NETWORK_H_ //** Header guards are necessary because C++ has a very low-level include syntax.
#define __NETWORK_H_ //** Without them, you can get strange multiple-definition errors due to text duplication.

#include <cstdlib>
#include "util.h"

#include "follow_set.h"

struct Person {
    int type;
    FollowSet follow_set;
    double creation_time;
    double x_location, y_location;
    void initialize() {
    	// Initialize a user slot - without a user, yet
    	follow_set.initialize();
    	creation_time = 0.0;
    	x_location = -1, y_location = -1;
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
    Person& operator[](int index) { //** This allows us to index our Network struct as if it were an array.
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

    // Convenient network queries:
    int n_following(int person_id) {
    	return persons[person_id].follow_set.n_following;
    }
    int follow_i(int person_id, int follow_index) {
    	return persons[person_id].follow_set[follow_index];
    }
};

#endif
