#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "io.h"
#include "util.h"

using namespace std;

// edgelist created for R (analysis) and python executable (drawing)
void output_position(Network& network, int n_users) {
	
	ofstream output;
	output.open("network.dat");
	for (int i = 0; i < n_users; i ++) {
		for (int j = 0; j < network.n_following(i); j ++) {
			output << i << "\t" << network.follow_i(i,j) << "\n";
		}
	}
	output.close();
} 

const int POUT_CAP = 1000; // AD: Rough work

/* This is a function that gives you the probability of a user following a 
 a given number of people. If you want to use this function, simply put
 P_OUT = 1 in the INFILE. This function will generate an output file that
 can be plotted using gnuplot. */

void POUT(Network& network, int MAX_USERS, int N_USERS, int N_FOLLOWS) {
	int N_FOLLOW_DATA[POUT_CAP + 1];
	for (int i = 0; i <= POUT_CAP; i++) {
		N_FOLLOW_DATA[i] = 0;
	}

	ofstream OUTPUT;
	OUTPUT.open("P_OUT.dat");
	
	OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n";
	OUTPUT << "#The mean of the distribution: " << double (N_FOLLOWS) / N_USERS << "\n\n";
	OUTPUT << "#N_FOLLOWS\tP(K)\tln(N_FOLLOWS)\tln(P(K))\n";

	for (int i = 0; i < MAX_USERS; i++) {
		int n_following = network.n_following(i);
		if (n_following <= POUT_CAP) {
			// AD: Rough work, discard n_following > POUT_CAP
			N_FOLLOW_DATA[n_following]++;
		}
	}

	N_FOLLOW_DATA[0] -= (MAX_USERS - N_USERS);
	for (int i = 1; i <= POUT_CAP; i++) {
		OUTPUT << i - 0.5 
			   << "\t" 
			   << N_FOLLOW_DATA[i] / double(N_USERS) 
			   << "\t"
			   << log(i) 
			   << "\t" 
			   << log(N_FOLLOW_DATA[i] / double(N_USERS))
			   << "\n"
			   << i + 0.5 
			   << "\t" 
			   << N_FOLLOW_DATA[i] / double(N_USERS) 
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
 of how many followers a given user will have. If you would like to use this
 function simply put P_IN = 1 in the INFILE. This program will produce an
 output file that can be plotted using gnuplot. */

void PIN(Network& network, int MAX_USERS, int N_USERS, double r_follow_norm) {
	int N_FOLLOWERS_DATA[N_USERS];
	for (int i = 0; i < N_USERS; i++) {
		N_FOLLOWERS_DATA[i] = 0;
	}

	for (int user = 0; user < N_USERS; user++) {
		for (int i = 0; i < network.n_following(user); i++) {
			N_FOLLOWERS_DATA[network.follow_i(user, i)]++;
		}
	}
	ofstream output;
	output.open("P_IN.dat");

	output << "##### THIS IS THE P_IN(K) FUNCTION DATA #####\n\n";
	output << "#N_FOLLOWERS\tP(K)\tln(N_FOLLOWERS)\tln(P(K))\n";
	int N_FOLLOWERS_DISTRO[MAX_USERS];
	for (int i = 0; i < MAX_USERS; i++) {
		N_FOLLOWERS_DISTRO[i] = 0;
	}

	for (int i = 0; i < N_USERS; i++) {
		Person& p = network[i];
		N_FOLLOWERS_DISTRO[N_FOLLOWERS_DATA[i]]++;
	}
	for (int i = 1; i < MAX_USERS; i++) {
		output << i - 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_USERS)
				<< "\t" << log(i) << "\t"
				<< log(N_FOLLOWERS_DISTRO[i] / double(N_USERS)) << "\n";
		output << i + 0.5 << "\t" << N_FOLLOWERS_DISTRO[i] / double(N_USERS)
				<< "\t" << "\n";
	}
	output.close();
}

void Categories_Check(CategoryGroup& tweeting, CategoryGroup& following, CategoryGroup& retweeting) {
	ofstream output;
	output.open("Categories_Distro.dat");
	output << "\t\tFollowing\tTweeting\tRetweeting\n\n";
	
	for (int i = 0; i < following.categories.size(); i ++) {
		Category& C_follows = following.categories[i];
		Category& C_tweets = tweeting.categories[i];
		Category& C_retweets = retweeting.categories[i];
		output << "Thresholds:\t" << C_follows.threshold << "\t\t" << C_tweets.threshold << "\t\t" << C_retweets.threshold << "\n";
		output << "# Users:\t" << C_follows.users.size() << "\t\t" << C_tweets.users.size() << "\t\t" << C_retweets.users.size() << "\n\n";
		
	}
	output.close();
}

// this function is simply the P_OUT and P_IN function added together
void Cumulative_Distro(Network& network, int MAX_USERS, int N_USERS, int N_FOLLOWS) {
	int cumulative_distro[N_USERS];
	int n_followers_data[N_USERS];
	int n_followers_distro[N_USERS];
	int n_following_distro[N_USERS];
	
	for (int i = 0; i < N_USERS; i ++) {
		cumulative_distro[i] = 0;
		n_followers_distro[i] = 0;
		n_followers_data[i] = 0;
		n_following_distro[i] = 0;
	}
	for (int i = 0; i < N_USERS; i ++) {
		for (int j = 0; j < network.n_following(i); j ++) {
			n_followers_data[network.follow_i(i,j)] ++;
		}
	}
	for (int i = 0; i < N_USERS; i ++) {
		n_followers_distro[n_followers_data[i]] ++;
	}
	for (int i = 0; i < N_USERS; i ++) {
		n_following_distro[network.n_following(i)] ++;
	}
	for (int i = 0; i < N_USERS; i ++) {
		cumulative_distro[i] = n_followers_distro[i] + n_following_distro[i];
	}
	
	ofstream output;
	output.open("cumulative_distro.csv");
	
	for (int i = 0; i < N_USERS; i ++) {
		output << i - 0.5 << "," << cumulative_distro[i]/double(2*N_USERS) << "\n";
		output << i + 0.5 << "," << cumulative_distro[i]/double(2*N_USERS) << "\n";
	}
	output.close();
}
