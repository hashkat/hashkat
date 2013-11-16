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

void POUT(Network& network, int MAX_USERS, int N_USERS) {
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
   
    N_FOLLOW_DATA[0] -= (MAX_USERS - N_USERS);
    for (int i = 0; i < MAX_FOLLOWING; i++) {
        OUTPUT << i - 0.5 << "\t" << N_FOLLOW_DATA[i] / double(N_USERS)
                << "\n" ;
        OUTPUT << i + 0.5 << "\t" << N_FOLLOW_DATA[i] / double(N_USERS)
                << "\n";
    }

    OUTPUT.close();
}

void PIN(Network& network, int MAX_USERS, int N_USERS)
{	
	int N_FOLLOWERS_DATA[N_USERS];
	for (int i = 0; i < N_USERS; i ++)
	{
		N_FOLLOWERS_DATA[i] = 0;
	}

	for (int i = 0; i < N_USERS; i ++)
	{
		Person& p = network[i];
		for (int j = 0; j < p.n_following; j ++)
		{
			N_FOLLOWERS_DATA[p.follows[j]] ++;
		}
	}

	ofstream output;
	output.open("P_IN.dat");
	
	output << "##### THIS IS THE P_IN(K) FUNCTION DATA #####\n\n";

	int N_FOLLOWERS_DISTRO[MAX_USERS];
	for (int i = 0; i < MAX_USERS; i ++)
	{
		N_FOLLOWERS_DISTRO[i] = 0;
	}
	
	for (int i = 0; i < N_USERS; i ++)
	{
		N_FOLLOWERS_DISTRO[N_FOLLOWERS_DATA[i]] ++;
	}
	for (int i = 0; i < MAX_USERS; i ++)
	{
		output << i - 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_USERS) << "\n";
		output << i + 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_USERS) << "\n";
	}
}	
	

