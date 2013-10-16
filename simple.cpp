#include <iostream>
#include <fstream>
#include <time.h>
#include <vector>

using namespace std;

int main()
{
	int array[10][10000]={};

	srand(time(NULL));
	
	double addjoe = 0.5;
	double addceleb = 0.005;
	double joefollow = 0.1;
	double celebfollow = 0.5;

	int njoes = 10;
	int ncelebs = 2;
	int nmembers = njoes + ncelebs;	
	
	vector <int> celebs;
	vector <int> joes;

	for (int i = 0; i < 10; i ++)
        {
                if ( i == 2 || i == 5)
		{celebs.push_back(i);}
		else
		{joes.push_back(i);}
	}

	int t = 0;
	
	while (t  < 5000)
	{
		double rand_prob = (rand() % 100) / 100.;
		if ( rand_prob < addjoe)
		{
			njoes ++;
			nmembers = ncelebs + njoes;
			joes.push_back(nmembers);
		}
		if ( rand_prob < addceleb)
		{
			ncelebs ++; 
			nmembers = ncelebs + njoes;
			celebs.push_back(nmembers);
		}
		
		cout << "At t = " << t << ".\nThere are " << nmembers << " members. " << ncelebs << " are celebs and " << njoes << " are average people." << endl;
	t ++;		
	}
return 0;
}
	
	   










