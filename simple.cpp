#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

int main()
{
	int array[10][100]={};

	srand(time(NULL));
	
	double addmember = 0.1;
	double follow = 0.5;

	double rand_num;
	int rand_cola, rand_rowa, rand_colb, rand_rowb;
	int counter = 0;

	ofstream output;
	output.open("growth.dat");

	while (counter < 900)
	{
		counter ++;
		rand_num = (rand() % 100) / 100.0;
		rand_cola = rand() % 100;
		rand_rowa = rand() % 10;
		rand_colb = rand() % 100;
		rand_rowb = rand() % 10;
		
		if (rand_num < follow && rand_cola != rand_colb && rand_rowa != rand_rowb)
		{
			array[rand_rowa][rand_cola] = rand_rowb;
		}
	
		output << "ITERATIVE STEP: " << counter << " ; rate = " << rand_num << "\n\n";	
			
		for (int i = 0; i < 10; i ++)
		{
			for (int j = 0; j < 100; j ++)
			{
				output << array[i][j] << " ";
				if (j == 99)
				{
					output << "\n";
				}
			}
		}
	}
output.close();
return 0;
}
	
	
