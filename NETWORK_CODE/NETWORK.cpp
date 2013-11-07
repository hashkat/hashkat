#include <iostream>    // input/output
#include <fstream>     // input/output files
#include <vector>      // for dynamic array allocations
#include <cmath>       // for log function
#include <time.h>
#include <stdlib.h>	
#include "INFILE.h"
#include <iomanip>
//#include "POUT.h"

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
	int P_OUT = INFILE("P_OUT");
	
	double T_FINAL = INFILE("T_FINAL");
	
	double R_FOLLOW = INFILE("R_FOLLOW");
	double R_TWEET = INFILE("R_TWEET");
	double R_ADD = INFILE("R_ADD");
	
	//#####################################################

	//DEFINE SOME VARIABLES NEEDED
	//#############################################################
	double TIME = 0.0;    // the initial time is 0 minutes
        double zerotol = 1e-16; // enough precision for really really low add rate	
	double R_TOTAL =  R_ADD + R_FOLLOW*N_USERS + R_TWEET*N_USERS; 
	
	//Normalize the rates
	
	double R_ADD_NORM = R_ADD / R_TOTAL;
	double R_FOLLOW_NORM = R_FOLLOW *N_USERS  / R_TOTAL;
	double R_TWEET_NORM = R_TWEET *N_USERS / R_TOTAL;
	
	//DECLARE THE MAIN NETWORK ARRAY
	//######################################################################
	// SP is there a way to delcare these using a function?
	int NETWORK[MAX_USERS][MAX_FOLLOWING]; // This is the main network array
	
	int NFOLLOWING[MAX_USERS];
	
	// INITIALIZE THE ABOVE ARRAYS
        // IT do this in a separate file

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

	long int N_STEPS = 0, N_TWEETS = 0, N_FOLLOWS = 0;

	//RANDOM SEED
	// If we try the same seeds, we should expect to get the same answer.
	srand(time(NULL));
	
	//THIS IS THE MAIN LOOP
	//###############################################################################################
	while ( TIME < T_FINAL || N_USERS < MAX_USERS )
	{
		
			// get the first uniform number [0,1). This selects a number from a long int (default RAND_MAX)
                        // IT confirm this is still uniform
 
			 double u_1 = (rand()+1) / (double(RAND_MAX)+1);
                        // IT it seems like u_1 has more than 16 digits...what's with that?
                         
			// DECIDE WHAT TO DO
			//##############################################################################
			// If we find ourselves in the add user chuck of our cumuative function
			if (u_1 - (R_ADD_NORM) <= zerotol )
			{
				N_USERS ++;
				//call to function to decide which user to add
			}

			// If we find ourselves in the bond node chunk of our cumulative function

			else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM) <= zerotol)
			{
				N_FOLLOWS ++;
				double val = u_1 - R_ADD_NORM;
				int user = val / (R_FOLLOW_NORM / N_USERS);  // this finds the user

                                // IT do a call to the follow function, i.e.
                                // IT confirm the user selection for random is correct (check to make sure boundaries work!)
 
				NETWORK[user][NFOLLOWING[user]] = int((rand()/double(RAND_MAX))*N_USERS);
				NFOLLOWING[user] ++;
			}
			
			// if we find ourselves in the tweet chuck of the cumulative function
			else if (u_1 - (R_ADD_NORM + R_FOLLOW_NORM + R_TWEET_NORM) <= zerotol )
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
		
			// something wierd is going on here -- when n_users changes dynamically
			// we need that 2, but when n_users is fixed we dont need the 2			
			
			double DYNAMIC_ADD_RATE = N_USERS / TIME, DYNAMIC_FOLLOW_RATE = 2*N_FOLLOWS / (N_USERS*TIME), DYNAMIC_TWEET_RATE = 2*N_TWEETS / (N_USERS*TIME);
			
			// ##################################################################################################
			if (VERBOSE == 0)
			{ DATA_TIME << fixed << setprecision(10) << "INCREMENT: " << N_STEPS << "\t" << "TIME_(min): " << TIME << "\t" << "RANDOM_NUM1: " << u_1 << "\t" << "N_USERS: " << N_USERS << "\t" << "N_FOLLOWS: " << N_FOLLOWS << "\t" << "N_TWEETS: " << N_TWEETS << "\t" << "R_ADD_NORM: " << R_ADD_NORM << "\t" << "R_FOLLOW_NORM: " << R_FOLLOW_NORM << "\t" << "R_TWEET_NORM: " << R_TWEET_NORM << "\t" << "DYNAMIC_ADD_RATE: " << DYNAMIC_ADD_RATE << "\t" << "DYNAMIC_FOLLOW_RATE: " << DYNAMIC_FOLLOW_RATE << "\t" << "DYNAMIC_TWEET_RATE: " << "\t" << DYNAMIC_TWEET_RATE << endl;
			}
			else if (VERBOSE == 1)
			{
				DATA_TIME << fixed << setprecision(5) << "TIME_(min):" << "\t" << TIME << "\t" << "NUMBER_OF_USERS:" << "\t" << N_USERS << "\t" << "NUMBER_OF_FOLLOWS:" << "\t" << N_FOLLOWS << "\t" << "NUMBER_OF_TWEETS:" << "\t" << N_TWEETS << "\t" <<  endl;
			}
			//SP else -> no output is written : There needs to be a more efficient way to do this. i.e. outside the main loop
			//##################################################################################################
			

			// TIME INCREMENTATION
			//####################################################
			
			//SP again we need to do this outside of the main loop.
			if (RANDOM_INCR == 1)
			{
				// get second random number
			        double u_2 = (rand()+1) / (double(RAND_MAX)+1);
				
				// increment by random time
				TIME += -log(u_2)/R_TOTAL;
			}
			else
			{
				TIME += 1/R_TOTAL;
			}
			//##################################################
			
			N_STEPS ++;			
			//update the rates if n_users has changed
			R_TOTAL = R_ADD + N_USERS*R_FOLLOW + N_USERS*R_TWEET ;
			R_ADD_NORM = R_ADD / R_TOTAL; 
			R_FOLLOW_NORM = R_FOLLOW*N_USERS / R_TOTAL;
			R_TWEET_NORM = R_TWEET*N_USERS / R_TOTAL;
			
	}

	//PRINT WHY PROGRAM STOPPED
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
			
/*	if (P_OUT == 1)
	{
		POUT(NFOLLOWING, MAX_USERS, MAX_FOLLOWING);
	}
	else
	{
		// nothing is done here
	}
*/
return 0;
}
