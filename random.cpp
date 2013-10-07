#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <time.h>

using namespace std;

int main()
{
	cout << "\n\n A very basic example of KMC\n\n";
	
	srand(time (NULL) );
        	
		double num = (rand() % 100)/100.0;
		cout << num << endl;
return 0;
}
	
	
