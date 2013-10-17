#include <iostream>    // input/output
#include <fstream>     // input/output files
#include <time.h>      // for rand functions
#include <vector>      // for dynamic array allocations
#include <cmath>       // for log function	
#include <stdlib.h>    // for rand function

using namespace std;    // use the above libraries



int main()    // this is the main function, returns 0 always
{
	// to get random times
	srand(time(NULL));

	double t_final = 100000000.0; // 100 million minutes
	double t = 0.0;    // the initial time is 0 minutes
	double n_users = 2; // There are two nodes initially
	
	//define the rates
	double add_node_rate = 1/86400.0; // one day
	double bond_node_rate = 1/604800.0; // one week
	double tweet_rate = 1/3600.0; // one hour

	
	// define the cumulative variables
	double r_add = add_node_rate, r_bond = bond_node_rate*n_users, r_tweet = tweet_rate*n_users;

	// define the cumulative function and cumulative bin
	double r_total =  r_add + r_bond + r_tweet;
	
	vector <char> node_type; // All we have right now is joes denoted as A
				 // A = joe, B = corporation, C = celeb --> future plans :)
	node_type.push_back('A');
	node_type.push_back('A');	

	// defining our network arrays
	int max_users = 30000;  // max number of users -> small for now
	int max_following = 1000; // max number of followers per user --> seemed reasonable after being discussed 

	int NETWORK[max_users][max_following]; // This is the main network array

	int NFOLLOWING[max_users];

	//Initialize these above arrays
	for (int i = 0; i < max_users; i ++)
	{
		NFOLLOWING[i] = 0;
		for (int j = 0; j < max_following; j ++)
		{
			NETWORK[i][j] = -1; // Used instead of 0, -1 shows no action in the array
		}
	}

	while ( t < t_final )
	{
			// get the first uniform number
			double u_1 = (rand() % 100) / 100.0;
		
			if ( u_1 < add_node_rate)
			{
					

			//get second uniform number
			double u_2 = (rand() % 100) / 100.0;
			
			// increment by random time
			t += -log(u_2)/R_3;
		}
	}
		







	
return 0;
}
