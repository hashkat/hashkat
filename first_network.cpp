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
	double add_node_rate = 1/3600.0; // one hour
	double bond_node_rate = 1/7200.0; // two hours
	double tweet_rate = 1/3600.0; // one hour

	
	// define the cumulative variables
	double r_add = add_node_rate, r_follow = bond_node_rate*n_users, r_tweet = tweet_rate*n_users;

	// define the cumulative function and cumulative bin
	double r_total =  r_add + r_follow + r_tweet;
	double r_1 = r_add / r_total;
	double r_2 = r_follow / r_total;
	double r_3 = r_tweet / r_total;
	
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

	// lets look at the number of steps the program makes
	int steps = 0;

	while ( t < t_final )
	{
			// get the first uniform number
			double u_1 = (rand() % 100 + 1) / 100.0;
		
			// Display the time
			cout << "Time = " << t << "; incr = " << steps << endl;

			// If we find ourselves in the add user chuck of our cumuative function
			if (u_1 - r_1 <= 0.0)
			{
				n_users ++;
				cout << "There are " << n_users << " users\n";
			}

			// If we find ourselves in the bond node chunk of our cumulative function
			if (u_1 - r_1 - r_2 <= 0.0 && u_1 - r_1 > 0.0)
			{
				double val = u_1 - r_1;
				int user = val/(r_2/n_users);  // this finds the user
				NETWORK[user][NFOLLOWING[user]] = rand() % n_users;
				NFOLLOWING[user] ++;
				cout << "User " << user << " followed someone\n"; 
			}
			
			// if we find ourselves in the tweet chuck of the cumulative function
			if (u_1 - r_1 - r_2 > 0.0)
			{
				double val = u_1 - r_1 - r_2;
				int user = val/(r_3/n_users); // this finds the user
				cout << "User " << user << " tweeted\n";
			}
			
			//get second uniform number
			double u_2 = (rand() % 100 + 1) / 100.0;
			// increment by random time
			t += -log(u_2)/r_total;
			
			steps ++;
			
			//update the rates if n_users has changed
			r_add = add_node_rate;
			r_follow = bond_node_rate*n_users;
			r_tweet = tweet_rate*n_users;
			r_total = r_add + r_follow + r_tweet;
			r_1 = (r_add / r_total);
			r_2 = (r_follow / r_total);
			r_3 = (r_tweet / r_total);
			
	}
			
return 0;
}
