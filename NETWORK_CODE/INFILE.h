// This program returns the value of the desired string you give it, this links the INCAR type file to the program

#include <fstream>     // for input/output files
#include <time.h>      // for rand functions
#include <vector>      // for dynamic array allocations

using namespace std;    // use the above libraries

double INFILE(string demand)
{
	ifstream INCAR;
	
	INCAR.open("INFILE");
	
	vector <string> input;
	string text;
	double value;
	
	while(!INCAR.eof())
	{
		INCAR >> text;
		input.push_back(text);
	}	
		for (int i = 0; i < input.size(); i ++ )
		{
			if (input[i] == demand)
			{
				value = atof(input[i+2].c_str());
			}
		}
	INCAR.close();
return value;
}
