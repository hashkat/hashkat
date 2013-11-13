#include <stdlib.h>
#include <time.h>

using namespace std;

int FOLLOW()
{
	srand(time(NULL));

	rndm_num = (rand() % 1000 + 1) / 1000.0;

	if (rndm_num > 0.5)
	{
		// follow a celeb
	}
	
	if (rndm_num <= 0.5 && rndm_num > 0.15)
	{
		// follow a corp
	}
	
	if (rndm_num <= 0.25)
	{
		// follow a joe
	}
//return a user id
}
