#include <fstream>
#include <vector>
#include <string>

#include "io.h"
#include "util.h"

using namespace std;

// Read the variable from the given 'input_file' and 'demand'ed variable
double load_config_var(string input_file, string demand) {
    ifstream file;

    file.open(input_file.c_str()); //** We must call c_str() because 'ifstream' expects a C-style pointer
    if (!file) {
        panic("Input file could not be found!\n");
    }

    vector<string> input;
    string text;
    double value;

    while (!file.eof()) {
        file >> text;
        input.push_back(text);
    }
    for (int i = 0; i < input.size(); i++) {
        if (input[i] == demand) {
            value = atof(input[i + 2].c_str());
        }
    }
    file.close();
    return value;
}

void POUT(Network& network, int MAX_USERS) {
    int N_FOLLOW_DATA[MAX_FOLLOWING];
    for (int i = 0; i < MAX_FOLLOWING; i++) {
        N_FOLLOW_DATA[i] = 0;
    }

    ofstream OUTPUT;
    OUTPUT.open("P_OUT.dat");

    OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n";

    for (int i = 0; i < MAX_USERS; i++) {
        DEBUG_CHECK(network[i].n_following <= MAX_FOLLOWING,
                "Cannot have more followers than MAX_FOLLOWING!");
        N_FOLLOW_DATA[network[i].n_following]++;
    }
    for (int i = 0; i < MAX_FOLLOWING; i++) {
        OUTPUT << i - 0.5 << "\t" << N_FOLLOW_DATA[i] / double(MAX_USERS)
                << endl;
        OUTPUT << i + 0.5 << "\t" << N_FOLLOW_DATA[i] / double(MAX_USERS)
                << endl;
    }

    OUTPUT.close();
}
