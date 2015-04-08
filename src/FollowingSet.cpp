#include "FollowingSet.h"

using namespace std;

void FollowingSet::print(AnalysisState& S) {
    if (size() > 0) {
        printf("[%s] (N_elems %d)\n", "FollowingSet", (int) size());
        implementation.print();
    } else {
        printf("<Empty>\n");
    }
}
