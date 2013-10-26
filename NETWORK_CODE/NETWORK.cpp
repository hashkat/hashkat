#include <iostream>    // input/output
#include <fstream>     // input/output files
#include <vector>      // for dynamic array allocations
#include <cmath>       // for log function
#include <time.h>
#include <stdlib.h>	
#include "INFILE.h"

using namespace std;    // use the above libraries


int main()    // this is the main function, returns 0 always
{
	//GET INFO FROM INFILE
    //#####################################################
	int N_USERS = INFILE("N_USERS");
	int MAX_USERS = INFILE("MAX_USERS");
	int MAX_FOLLOWING = INFILE("MAX_FOLLOWING");
	
	double T_FINAL = INFILE("T_FINAL");
	
	double R_ADD = INFILE("R_ADD");
	double R_FOLLOW = INFILE("R_FOLLOW");
	double R_TWEET = INFILE("R_TWEET");	
	//#####################################################

	//DEFINE SOME VARIABLES NEEDED
	//#############################################################
	double TIME = 0.0;    // the initial time is 0 minutes
	
	double R_TOTAL =  R_ADD + R_FOLLOW * N_USERS + R_TWEET * N_USERS; //Normalize the rates
	double R_ADD_NORM = R_ADD / R_TOTAL;
	double R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
	double R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;
	//#############################################################
	
	//FUTURE PLAN IDEAS
	//#####################################################################
	vector <char> node_type; // All we have right now is joes denoted as A
				 // A = joe, B = corporation, C = celeb --> future plans 
	node_type.push_back('A');
	node_type.push_back('A');	
	//#####################################################################
	
	//DECLARE THE MAIN NETWORK ARRAY
	//######################################################################
	int NETWORK[MAX_USERS][MAX_FOLLOWING]; // This is the main network array
	ofstream NETWORK_ARRAY;
	NETWORK_ARRAY.open("NETWORK_ARRAY");
	
	int NFOLLOWING[MAX_USERS];
	ofstream NFOLLOWING_DATA;
	NFOLLOWING_DATA.open("NFOLLOWING");
	
	// INITIALIZE THE ABOVE ARRAYS
	for (int i = 0; i < MAX_USERS; i ++)
	{
		NFOLLOWING[i] = 0;
		for (int j = 0; j < MAX_FOLLOWING; j ++)
		{
			NETWORK[i][j] = -1; // Used instead of 0, -1 shows no action in the array
		}
	}
	//#######################################################################

	//DECLARE OUTPUT FILE TO PLOT DATA
	//################################################
	ofstream DATA_TIME;
	DATA_TIME.open("DATA_vs_TIME");
	DATA_TIME << "#Time (min)\tN_USERS\tN_FOLLOWS\tN_TWEETS\n";
	//###############################################

	//CHECK OUR NUMBER OF STEPS, TWEETS and FOLLOWS
	int NSTEPS = 0, N_TWEETS = 0, N_FOLLOWS = 0;

	//RANDOM SEED
	srand(time(NULL));
	
	
	while ( TIME < T_FINAL && N_USERS < MAX_USERS )
	{
		
			// get the first uniform number
			double u_1 = (rand() % 1000 + 1) / 1000.0;
		

			// If we find ourselves in the add user chuck of our cumuative function
			if (u_1 - R_ADD_NORM <= 0.0)
			{
				N_USERS ++;
			}

			// If we find ourselves in the bond node chunk of our cumulative function
			if (u_1 - R_ADD_NORM - R_FOLLOW_NORM <= 0.0 && u_1 - R_ADD_NORM > 0.0)
			{
				N_FOLLOWS ++;
				double val = u_1 - R_ADD_NORM;
				int user = val / (R_FOLLOW_NORM / N_USERS);  // this finds the user
				NETWORK[user][NFOLLOWING[user]] = rand() % N_USERS;
				NFOLLOWING[user] ++;
			}
			
			// if we find ourselves in the tweet chuck of the cumulative function
			if (u_1 - R_ADD_NORM - R_FOLLOW_NORM > 0.0)
			{
				N_TWEETS ++;
				double val = u_1 - R_ADD_NORM - R_FOLLOW_NORM;
				int user = val/(R_TWEET_NORM/N_USERS); // this finds the user
			}
			
			
			DATA_TIME << TIME << "\t\t" << N_USERS << "\t\t" << N_FOLLOWS << "\t\t" << N_TWEETS << endl;
			
			//get second uniform number
			double u_2 = (rand() % 1000 + 1) / 1000.0;
			// increment by random time
			TIME += -log(u_2)/R_TOTAL;
			
			NSTEPS ++;
			
			//update the rates if n_users has changed
			R_TOTAL = R_ADD + R_FOLLOW * N_USERS + R_TWEET * N_USERS; 
			R_FOLLOW_NORM = R_FOLLOW * N_USERS / R_TOTAL;
			R_TWEET_NORM = R_TWEET * N_USERS / R_TOTAL;
			
	}

	DATA_TIME.close();
			
return 0;
}
