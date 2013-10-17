#include <iostream>    // input/output
#include <fstream>     // input/output files
#include <time.h>      // for rand functions
#include <vector>      // for dynamic array allocations
#include <cmath>       // for log function	
#include <stdlib.h>    // for rand function

using namespace std;    // use the above libraries

// this finds the first 0 value in the following column
int first_zero(int val, int array[][3000])
{
	int i = 0;
	while( i < 1000 )
	{
		if (array[i] == 0)
		{
			break;
		}
	}
return i;
}

// This function is so one cant follow themselves
int not_same_index(int value, int nodes)
{
	srand(time(NULL));

	int rand_val = rand() % nodes;
	while (rand_val == value)
	{
		rand_val = rand() % nodes;
	}
return rand_val;
}	
	

int main()    // this is the main function, returns 0 always
{
	// to get random times
	srand(time(NULL));

	double t_final = 100000000.0; // 100 million minutes
	double t = 0.0;    // the initial time is 0 minutes

	//define the rates
	double add_node = 1/86400.0;
	double bond_node = 1/604800.0; 
	double tweet = 1/3600.0;

	
	// define the cumulative function
	double R_1(add_node), R_2(add_node + bond_node), R_3(add_node + bond_node + tweet);
	double cm_func[3] = {R_1, R_2, R_3};

	vector <char> node_type; // All we have right now is joes denoted as A
				 // A = joe, B = corporation, C = celeb --> future plans :)
	node_type.push_back('A');
	node_type.push_back('A');	

	int n_nodes = 2; // start out with 2 members
	
	int network[1000][3000] = {}; // This is the main network array

	while ( t < t_final )
	{
		for (int i = 0; i < n_nodes; i ++)
		{

			// get the first uniform number
			double u_1 = (rand() % 100) / 100.0;
		
			if ( R_1 > u_1*R_3 )
			{
				// if true add node
				n_nodes ++;
				node_type.push_back('A');
				cout << "Time = " << t << ". Number of nodes = " << n_nodes << endl;
			}

			if ( R_2 > u_1*R_3 && R_1 < u_1*R_3 )
			{
				// if true create bond between with random node
				int zero = first_zero(i, network);
				int follow = not_same_index(i, n_nodes);						   network[zero][i] = follow;
				cout << "Time = " << t << ". Bond formed.\n";		
			}

			if ( R_3 > u_1*R_3 && R_1 < u_1*R_3 && u_1*R_3 )
			{
				// if true, then user tweets but will take up alot of space
				cout << "Time = " << t << ". Someone tweeted.\n";
			}

			//get second uniform number
			double u_2 = (rand() % 100) / 100.0;
			
			// increment by random time
			t += -log(u_2)/R_3;
		}
	}
		







	
return 0;
}
