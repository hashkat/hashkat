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
	int n_users = 2; // There are two nodes initially
	
	//define the rates
	double add_node_rate = 1/86400.0; // one day
	double bond_node_rate = 1/604800.0; // one week
	double tweet_rate = 1/3600.0; // one hour

	
	// define the cumulative variables
	double r_add = add_node_rate, r_follow = bond_node_rate*n_users, r_tweet = tweet_rate*n_users;

	// define the cumulative function and cumulative bin
	double r_total =  r_add + r_follow + r_tweet;
	
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
		
			cout << "Time = " << t << endl;

			if ( u_1 < r_add)
			{
				n_users ++;
				cout << "Number of users = " << n_users << endl;
			}
			
			if ( u_1 > r_add && u_1 < r_follow)
			{
				double bond_incr = r_follow / n_users;  // same as bond_node_rate, but just keeping it consistent with other histogram codes
					int choose_user = u_1 / bond_incr;
					NETWORK[choose_user][NFOLLOWING[choose_user]] = rand() % n_users;				
					cout << "User " << choose_user << " followed someone\n";
				
			}
			if ( u_1 > r_follow )
			{
				double tweet_incr = r_tweet / n_users; // see the comment above regarding bond_incr

					int choose_user = u_1 / tweet_incr;
					cout << "User " << choose_user << " Tweeted\n";
			}
			//get second uniform number
			double u_2 = (rand() % 100) / 100.0;
			// increment by random time
			t += -log(u_2)/r_total;
			
			//update the rates if n_users has changed
			r_add = add_node_rate;
			r_follow = bond_node_rate*n_users;
			r_tweet = tweet_rate*n_users;
		
	}
			
return 0;
}
