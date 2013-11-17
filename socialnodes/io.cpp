#include <fstream>
#include <vector>
#include <string>
#include <cmath>

#include "io.h"
#include "util.h"

using namespace std;

/* This is a function that gives you the probability of a user following a 
   a given number of people. If you want to use this function, simply put
   P_OUT = 1 in the INFILE. This function will generate an output file that
   can be plotted using gnuplot. */

void POUT(Network& network, int MAX_USERS, int N_USERS) {
    int N_FOLLOW_DATA[MAX_FOLLOWING];
    for (int i = 0; i < MAX_FOLLOWING; i++) {
        N_FOLLOW_DATA[i] = 0;
    }

    ofstream OUTPUT;
    OUTPUT.open("P_OUT.dat");

    OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n";
    OUTPUT << "#N_FOLLOWS\tP(K)\tln(N_FOLLOWS)\tln(P(K))\n";
	
    for (int i = 0; i < MAX_USERS; i++) {
        DEBUG_CHECK(network[i].n_following <= MAX_FOLLOWING,
                "Cannot have more followers than MAX_FOLLOWING!");
        N_FOLLOW_DATA[network[i].n_following]++;
    }
   

    N_FOLLOW_DATA[0] -= (MAX_USERS - N_USERS);
    for (int i = 1; i < MAX_FOLLOWING; i++) {
        OUTPUT << i - 0.5 << "\t" << N_FOLLOW_DATA[i] / double(N_USERS)
                << "\t" << log(i) << "\t" << log(N_FOLLOW_DATA[i] / double(N_USERS)) << "\n" ;
        OUTPUT << i + 0.5 << "\t" << N_FOLLOW_DATA[i] / double(N_USERS)
                << "\t" << "\n";
    }

    OUTPUT.close();
}


/* This is a function that goes through the network and finds the probability
   of how many followers a given user will have. If you would like to use this
   function simply put P_IN = 1 in the INFILE. This program will produce an 
   output file that can be plotted using gnuplot. */

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
        output << "#N_FOLLOWERS\tP(K)\tln(N_FOLLOWERS)\tln(P(K))\n";
	int N_FOLLOWERS_DISTRO[MAX_USERS];
	for (int i = 0; i < MAX_USERS; i ++)
	{
		N_FOLLOWERS_DISTRO[i] = 0;
	}
	
	for (int i = 0; i < N_USERS; i ++)
	{
		N_FOLLOWERS_DISTRO[N_FOLLOWERS_DATA[i]] ++;
	}
	for (int i = 1; i < MAX_USERS; i ++)
	{
		output << i - 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_USERS) << "\t" << log(i) << "\t" << log(N_FOLLOWERS_DISTRO[i] / double(N_USERS)) << "\n";
		output << i + 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_USERS) << "\t" << "\n" ;
	}
output.close();
}	
	

