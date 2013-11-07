#include <stdlib.h>
#include <fstream>

using namespace std;

void POUT(int NFOLLOWING[], int MAX_USERS, int MAX_FOLLOWING)
{
	int N_FOLLOW_DATA[MAX_FOLLOWING];
	for (int i = 0; i < MAX_FOLLOWING; i ++)
	{
		N_FOLLOW_DATA[i] = 0;
	}

	ofstream OUTPUT;
	OUTPUT.open("P_OUT.dat");
	
	OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n"; 
	
	for (int i = 0; i < MAX_USERS; i ++)
	{
		N_FOLLOW_DATA[NFOLLOWING[i]] ++;
	}
	for (int i = 0; i < MAX_FOLLOWING; i ++)
	{
		OUTPUT << i - 0.5 << "\t" << N_FOLLOW_DATA[i] << endl;
		OUTPUT << i + 0.5 << "\t" << N_FOLLOW_DATA[i] << endl;
	}

OUTPUT.close();
}
