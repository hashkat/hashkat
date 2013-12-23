#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "io.h"
#include "util.h"

using namespace std;

// edgelist created for R (analysis) and python executable (drawing)
void output_position(Network& network, int n_entities) {
	
	ofstream output;
	output.open("network.dat");
	for (int i = 0; i < n_entities; i ++) {
		for (int j = 0; j < network.n_followers(i); j ++) {
			output << i << "\t" << network.following_i(i,j) << "\n";
		}
	}
	output.close();
} 

const int POUT_CAP = 1000; // AD: Rough work

/* This is a function that gives you the probability of a entity following a
 a given number of people. If you want to use this function, simply put
 P_OUT = 1 in the INFILE. This function will generate an output file that
 can be plotted using gnuplot. */

void POUT(Network& network, int MAX_ENTITIES, int N_ENTITIES, int N_FOLLOWS) {
	int N_FOLLOW_DATA[POUT_CAP + 1];
	for (int i = 0; i <= POUT_CAP; i++) {
		N_FOLLOW_DATA[i] = 0;
	}

	ofstream OUTPUT;
	OUTPUT.open("P_OUT.dat");
	
	OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n";
	OUTPUT << "#The mean of the distribution: " << double (N_FOLLOWS) / N_ENTITIES << "\n\n";
	OUTPUT << "#N_FOLLOWS\tP(K)\tln(N_FOLLOWS)\tln(P(K))\n";

	for (int i = 0; i < MAX_ENTITIES; i++) {
		int n_following = network.n_following(i);
		if (n_following <= POUT_CAP) {
			// AD: Rough work, discard n_following > POUT_CAP
			N_FOLLOW_DATA[n_following]++;
		}
	}

	N_FOLLOW_DATA[0] -= (MAX_ENTITIES - N_ENTITIES);
	for (int i = 1; i <= POUT_CAP; i++) {
		OUTPUT << i - 0.5 
			   << "\t" 
			   << N_FOLLOW_DATA[i] / double(N_ENTITIES) 
			   << "\t"
			   << log(i) 
			   << "\t" 
			   << log(N_FOLLOW_DATA[i] / double(N_ENTITIES))
			   << "\n"
			   << i + 0.5 
			   << "\t" 
			   << N_FOLLOW_DATA[i] / double(N_ENTITIES) 
			   << "\n";
			   
	}

	OUTPUT.close();
}
int factorial(int input_number) {
	int value = 1;
	for (int i = 1; i <= input_number; i ++) {
		value *= i;
	}
	return value;
}
/* This is a function that goes through the network and finds the probability
 of how many followers a given entity will have. If you would like to use this
 function simply put P_IN = 1 in the INFILE. This program will produce an
 output file that can be plotted using gnuplot. */

void PIN(Network& network, int MAX_ENTITIES, int N_ENTITIES, double r_follow_norm) {
	int N_FOLLOWERS_DATA[N_ENTITIES];
	for (int i = 0; i < N_ENTITIES; i++) {
		N_FOLLOWERS_DATA[i] = 0;
	}

	for (int entity = 0; entity < N_ENTITIES; entity++) {
		for (int i = 0; i < network.n_following(entity); i++) {
			N_FOLLOWERS_DATA[network.follow_i(entity, i)]++;
		}
	}
	ofstream output;
	output.open("P_IN.dat");

	output << "##### THIS IS THE P_IN(K) FUNCTION DATA #####\n\n";
	output << "#N_FOLLOWERS\tP(K)\tln(N_FOLLOWERS)\tln(P(K))\n";
	int N_FOLLOWERS_DISTRO[MAX_ENTITIES];
	for (int i = 0; i < MAX_ENTITIES; i++) {
		N_FOLLOWERS_DISTRO[i] = 0;
	}

	for (int i = 0; i < N_ENTITIES; i++) {
		Entity& p = network[i];
		N_FOLLOWERS_DISTRO[N_FOLLOWERS_DATA[i]]++;
	}
	for (int i = 1; i < MAX_ENTITIES; i++) {
		output << i - 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_ENTITIES)
				<< "\t" << log(i) << "\t"
				<< log(N_FOLLOWERS_DISTRO[i] / double(N_ENTITIES)) << "\n";
		output << i + 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_ENTITIES)
				<< "\t" << "\n";
	}
	output.close();
}


static void category_print(ofstream& output, const char* name, CategoryGroup& group) {
    output << name << ": ";
    for (int i = 0; i < group.categories.size(); i ++) {
        Category& c = group.categories[i];
        output << c.entities.size() << " at " << c.threshold << '\t';
    }
}

void Categories_Check(CategoryGroup& tweeting, CategoryGroup& following, CategoryGroup& retweeting) {
	ofstream output;
	output.open("Categories_Distro.dat");
	category_print(output, "Tweeting", tweeting);
	category_print(output, "Following", following);
	category_print(output, "Retweeting", retweeting);
	output.close();
}

// this function is simply the P_OUT and P_IN function added together
void Cumulative_Distro(Network& network, int MAX_ENTITIES, int N_ENTITIES, int N_FOLLOWS) {
	int cumulative_distro[N_ENTITIES];
	int n_followers_data[N_ENTITIES];
	int n_followers_distro[N_ENTITIES];
	int n_following_distro[N_ENTITIES];
	
	for (int i = 0; i < N_ENTITIES; i ++) {
		cumulative_distro[i] = 0;
		n_followers_distro[i] = 0;
		n_followers_data[i] = 0;
		n_following_distro[i] = 0;
	}
	for (int i = 0; i < N_ENTITIES; i ++) {
		for (int j = 0; j < network.n_following(i); j ++) {
			n_followers_data[network.follow_i(i,j)] ++;
		}
	}
	for (int i = 0; i < N_ENTITIES; i ++) {
		n_followers_distro[n_followers_data[i]] ++;
	}
	for (int i = 0; i < N_ENTITIES; i ++) {
		n_following_distro[network.n_following(i)] ++;
	}
	for (int i = 0; i < N_ENTITIES; i ++) {
		cumulative_distro[i] = n_followers_distro[i] + n_following_distro[i];
	}
	
	ofstream output;
	output.open("cumulative_distro.dat");
	
	for (int i = 0; i < N_ENTITIES; i ++) {
		output << i - 0.5 << "\t" << cumulative_distro[i]/double(2*N_ENTITIES) << "\n";
		output << i + 0.5 << "\t" << cumulative_distro[i]/double(2*N_ENTITIES) << "\n";
	}
	output.close();
}
// produces an output file with entity type statistics, makes sure things are working properly
void entity_statistics(Network& network, int n_follows, int n_entities, int max_entities, EntityType* entitytype) {
	ofstream output;
	output.open("entity_percentages.dat");
	int entity_counts[max_entities];
	int average_followers_from_network[max_entities];
	int average_followers_from_lists[max_entities];
	for (int i = 0 ; i < max_entities; i ++) {
		entity_counts[i] = 0;
		average_followers_from_network[i] = 0;
		average_followers_from_lists[i] = 0;
	}
	for (int i = 0 ; i < n_entities; i ++) {
		Entity& p = network[i];
		for (int j = 0; j < max_entities; j ++) {
			if (p.entity == j) {
				entity_counts[j] ++;
				average_followers_from_network[j] += p.follower_set.size;
			}
		}
	} 
	output << "\n ****** Info regarding numbers of entity types - BASED ON NETWORK ARRAY ****** \n\n";
	for (int i = 0; i < max_entities; i ++) {
		output << "Number of entity " << i << " : " << entity_counts[i] << "    Fraction: " << entity_counts[i]/double(n_entities) << "\n";
	}
	output << "\n ****** Info regarding numbers of entity types - BASED ON USER_LISTS ****** \n THIS SHOULD BE THE SAME AS ABOVE \n\n";
	for (int i = 0; i < max_entities; i ++) {
		output << "Number of entity " << i << " : " << entitytype[i].entity_list.size() << "    Fraction: " << entitytype[i].entity_list.size()/double(n_entities) << "\n";
	}
	output << "\n\n ****** Info regarding following certain entity types - BASED ON NETWORK ARRAY ****** \n\n";
	double sum = 0;
	for (int i = 0; i < max_entities; i ++) {
		sum += average_followers_from_network[i]/double(entity_counts[i]);
	}
	for (int i = 0; i < max_entities; i ++) {
		output << "Average number of follows for entity " 
			   << i << ": " << average_followers_from_network[i]/double(entity_counts[i])
			   << "     Fraction: " << average_followers_from_network[i]/double(n_follows) << "\n";
	}
	for (int i = 0; i < max_entities; i ++){
		for (int j = 0; j < entitytype[i].entity_list.size(); j++) {
			Entity& p = network[entitytype[i].entity_list[j]];
			average_followers_from_lists[i] += p.follower_set.size;
		}
	}
	output << "\n\n ****** Info regarding following certain entity types - BASED ON USER_LISTS ****** \n      SHOULD BE THE SAME AS ABOVE\n\n";
	double sum1 = 0;
	for (int i = 0; i < max_entities; i ++) {
		sum1 += average_followers_from_lists[i] / double(entitytype[i].entity_list.size());
	}
	for (int i = 0; i < max_entities; i ++) {
		output << "Average number of follows for entity " 
			   << i << ": " << average_followers_from_lists[i]/double(entitytype[i].entity_list.size())
			   << "     Fraction: " << average_followers_from_network[i]/double(n_follows) << "\n"; 
	}
	output.close();
}
