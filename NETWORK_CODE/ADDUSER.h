#include <stdlib.h>

using namespace std;

char CHOOSE_USER(double random_num, double joe_rate, double celeb_rate, double org_rate, int &N_JOES, int &N_CELEBS, int &N_ORGS)
{
	double add_r_total = joe_rate + celeb_rate + org_rate;
	double joe_norm = joe_rate / add_r_total;
	double celeb_norm = celeb_rate / add_r_total;
	double org_norm = org_rate / add_r_total;

	char type;

	if ( random_num - joe_norm < 0 )
	{
		N_JOES ++;
		type = 'A';
	}
	if (random_num - (joe_norm + celeb_norm) < 0 && random_num - joe_norm >= 0)
	{
		N_CELEBS ++;
		type = 'C';
	}
	if (random_num - (joe_norm + celeb_norm) >= 0)
	{
		N_ORGS ++;
		type = 'B';
	}
return type;
}
