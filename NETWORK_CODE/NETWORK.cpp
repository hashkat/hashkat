#include <iostream>    // input/output
#include <fstream>     // input/output files
#include <vector>      // for dynamic array allocations
#include <cmath>       // for log function
#include <time.h>
#include <stdlib.h>	
#include "INFILE.h"
#include <iomanip>

using namespace std;    // use the above libraries


int main()    // this is the main function, returns 0 always
{
	//GET INFO FROM INFILE
    //#####################################################

	int N_USERS = INFILE("N_USERS");
	int MAX_USERS = INFILE("MAX_USERS");
	int MAX_FOLLOWING = INFILE("MAX_FOLLOWING");
	int VERBOSE = INFILE("VERBOSE");	
	int RANDOM_INCR = INFILE("RANDOM_INCR");
	
	double T_FINAL = INFILE("T_FINAL");
	
	double R_FOLLOW = INFILE("R_FOLLOW");
	double R_TWEET = INFILE("R_TWEET");
	double R_ADD = INFILE("R_ADD");
	
	//#####################################################

	//DEFINE SOME VARIABLES NEEDED
	//#############################################################
	double TIME = 0.0;    // the initial time is 0 minutes
	
	double R_TOTAL =  R_ADD + R_FOLLOW*N_USERS + R_TWEET*N_USERS; 
	
	//Normalize the rates
	
	double R_ADD_NORM = R_ADD / R_TOTAL;
	double R_FOLLOW_NORM = R_FOLLOW *N_USERS  / R_TOTAL;
	double R_TWEET_NORM = R_TWEET *N_USERS / R_TOTAL;
	//#############################################################
	
	/*FUTURE PLAN IDEAS
	//#####################################################################
	vector <char> PEOPLE_TYPE; // All we have right now is joes denoted as A
				 // A = joe, B = corporation, C = celeb --> future plans 
	for (int i = 0; i < N_JOES; i ++)
	{
		PEOPLE_TYPE.push_back('A');
	}
	for (int i = 0; i < N_CELEB; i ++)
	{
		PEOPLE_TYPE.push_back('C');
	}
	for (int i = 0; i < N_ORG; i ++)
	{
		PEOPLE_TYPE.push_back('B');
		
	}*/
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
	
	
	//###############################################

	//CHECK OUR NUMBER OF STEPS, TWEETS and FOLLOWS
	long int NSTEPS = 0, N_TWEETS = 0, N_FOLLOWS = 0;

	//RANDOM SEED
	srand(time(NULL));
	
	//THIS IS THE MAIN LOOP
	//###############################################################################################
	while ( TIME < T_FINAL && N_USERS < MAX_USERS )
	{
		
			// get the first uniform number
                        // IT confirm that you have enough precision here - confirmed
			 double u_1 = ((long) rand() % 10000000 + 1) / 10000000.0;	

			// DECIDE WHAT TO DO
			//##############################################################################
			// If we find ourselves in the add user chuck of our cumuative function
			if (u_1 - R_ADD_NORM <= 1e-15 )
			{
				N_USERS ++;
				//call to function to decide which user to add
			}

			// If we find ourselves in the bond node chunk of our cumulative function
			else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM) <= 1e-15 )
			{
				N_FOLLOWS ++;
				double val = u_1 - R_ADD_NORM;
				int user = val / (R_FOLLOW_NORM / N_USERS);  // this finds the user
				NETWORK[user][NFOLLOWING[user]] = rand() % N_USERS;
				NFOLLOWING[user] ++;
			}
			
			// if we find ourselves in the tweet chuck of the cumulative function
			else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= 1e-15)
			{
				N_TWEETS ++;
				double val = u_1 - R_ADD_NORM - R_FOLLOW_NORM;
				int user = val/(R_TWEET_NORM/N_USERS); // this finds the user
			}
			
                        else
                        {
                        cout << "Disaster, event out of bounds" << endl;
                        }
			//##############################################################################
			// SP something wierd is going on here -- when n_users changes dynamically
			// we need that 2, but when n_users is fixed we dont need the 2			
			double DYNAMIC_ADD_RATE = N_USERS / TIME, DYNAMIC_FOLLOW_RATE = 2*N_FOLLOWS / (N_USERS*TIME), DYNAMIC_TWEET_RATE = 2*N_TWEETS / (N_USERS*TIME);
			
			// IT there should be different levels of verbosity. 0=debug, 1=normal, 2=no data writing at all
			// LEVELS OF VERBOSITY
			// ##################################################################################################
			if (VERBOSE == 0)
			{ DATA_TIME << fixed << setprecision(10) << "INCREMENT: " << NSTEPS << "\t" << "TIME_(min): " << TIME << "\t" << "RANDOM_NUM1: " << u_1 << "\t" << "N_USERS: " << N_USERS << "\t" << "N_FOLLOWS: " << N_FOLLOWS << "\t" << "N_TWEETS: " << N_TWEETS << "\t" << "R_ADD_NORM: " << R_ADD_NORM << "\t" << "R_FOLLOW_NORM: " << R_FOLLOW_NORM << "\t" << "R_TWEET_NORM: " << R_TWEET_NORM << "\t" << "DYNAMIC_ADD_RATE: " << DYNAMIC_ADD_RATE << "\t" << "DYNAMIC_FOLLOW_RATE: " << DYNAMIC_FOLLOW_RATE << "\t" << "DYNAMIC_TWEET_RATE: " << "\t" << DYNAMIC_TWEET_RATE << endl;
			}
			else if (VERBOSE == 1)
			{
				DATA_TIME << fixed << setprecision(5) << "TIME_(min):" << "\t" << TIME << "\t" << "NUMBER_OF_USERS:" << "\t" << N_USERS << "\t" << "NUMBER_OF_FOLLOWS:" << "\t" << N_FOLLOWS << "\t" << "NUMBER_OF_TWEETS:" << "\t" << N_TWEETS << "\t" <<  endl;
			}
			//SP else -> no output is written : There needs to be a more efficient way to do this. i.e. outside the main loop
			//##################################################################################################
			

			// TIME INCREMENTATION
			//####################################################
			if (RANDOM_INCR == 1)
			{
				// get second random number
				double u_2 = (rand() % 1000 + 1) / 1000.0;
				
				// increment by random time
				TIME += -log(u_2)/R_TOTAL;
			}
			else
			{
				TIME += 1/R_TOTAL;
			}
			//##################################################
			
			//update the rates if n_users has changed
			R_TOTAL = R_ADD + N_USERS*R_FOLLOW + N_USERS*R_TWEET ;
			R_ADD_NORM = R_ADD / R_TOTAL; 
			R_FOLLOW_NORM = R_FOLLOW*N_USERS  / R_TOTAL;
			R_TWEET_NORM = R_TWEET*N_USERS  / R_TOTAL;
			
	}
        // IT It looks like you may have issues with long vs short ints. NSTEPS went negative for one of my test cases. -> FIXED - NSTEPS is now of type 'long int' 
	

	//PRINT WHY PROGRAM STOPPED - SP needs to be more efficient as well, these if loops will slow down code
	//#######################################################################################
	if (TIME < T_FINAL)
	{
		cout << "\nReached maximum number of users.\n\n- - Program Terminated - -\n\n";
	}
	else
	{
		cout << "\nReached maximum amount of time.\n\n- - Program Terminated - -\n\n";
	}
	DATA_TIME.close();
	//###################################################################################################
			
return 0;
}
