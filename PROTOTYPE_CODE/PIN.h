#include <stdlib.h>
#include <fstream>

using namespace std;

void PIN(const int NETWORK_ARRAY[][MAX_FOLLOWERS], int MAX_USERS)
{
	int USER_NFOLLOWERS[MAX_USERS + 1], FOLLOWERS_HISTOGRAM[MAX_USERS];
	for (int i = 0; i < MAX_USERS + 1; i ++)
	{
		USER_NFOLLOWERS[i] = 0;
	}
	
	for (int i = 0; i < MAX_USERS; i ++)
	{
		FOLLOWERS_HISTOGRAM[i] = 0;

		for (int j = 0; j < MAX_FOLLOWS; j ++)
		{
			USER_NFOLLOWERS[NETWORK_ARRAY[i][j] + 1] ++;
		}
	}
	
	for (int i = 1; i < MAX_USERS + 1; i ++)
	{
		FOLLOWERS_HISTOGRAM[USER_FOLLOWERS[i]] ++;
	}

	ofstream OUTPUT;
	OUTPUT.open("POUT.dat");

	OUTPUT << "##### THIS IS THE P_IN(k) DATA #####\n\n";

	for (int i = 0; i < MAX_USERS; i ++)
	{
		OUTPUT << i - 0.5 << "\t" << FOLLOWERS_HISTOGRAM[i] << endl;
		OUTPUT << i + 0.5 << "\t" << FOLLOWERS_HISTOGRAM[i] << endl;
	}

OUTPUT.close();
}
	
			
