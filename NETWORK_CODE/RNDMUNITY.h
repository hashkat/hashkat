// This program returns a value from the set 0 < x <= 1

#include <time.h>
#include <stdlib.h>

double RNDM_UNITY()
{
	srand(time(NULL)); 
	
	return (rand() % 100 + 1) / 100; 
}