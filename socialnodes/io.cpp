#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include "io.h"
#include "util.h"

using namespace std;

void output_position(Network& network, int n_users) {
	ofstream output;
	output.open("POSITIONS.gexf");
	output << "<gexf version=\"1.2\">\n"
			<< "<meta lastmodifieddate=\"2013-11-21\">\n"
			<< "<creator> Kevin Ryczko </creator>\n"
			<< "<description> Social Network Visualization </description>\n"
			<< "</meta>\n"
			<< "<graph mode=\"static\" defaultedgetype=\"directed\">\n"
			<< "<nodes>\n";
	for (int i = 0; i < n_users; i++) {
		Person& p = network[i];
		output << "<node id=\"" << i << "\" label=\"" << p.type << "\" />\n";
	}
	output << "</nodes>\n" << "<edges>\n";
	for (int i = 0; i < n_users; i++) {
		for (int j = 0; j < network.n_following(i); j++) {
			output << "<edge id=\"" << i << "\" source=\"" << i
					<< "\" target=\"" << network.follow_i(i, j) << "\"/>\n";
		}
	}
	output << "</edges>\n" << "</graph>\n" << "</gexf>";
	output.close();
} 



const int POUT_CAP = 1000; // AD: Rough work

/* This is a function that gives you the probability of a user following a 
 a given number of people. If you want to use this function, simply put
 P_OUT = 1 in the INFILE. This function will generate an output file that
 can be plotted using gnuplot. */

void POUT(Network& network, int MAX_USERS, int N_USERS) {
	int N_FOLLOW_DATA[POUT_CAP + 1];
	for (int i = 0; i <= POUT_CAP; i++) {
		N_FOLLOW_DATA[i] = 0;
	}

	ofstream OUTPUT;
	OUTPUT.open("P_OUT.dat");

	OUTPUT << "##### THIS IS THE P_OUT(K) FUNCTION DATA #####\n\n";
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
		OUTPUT << i - 0.5 << "\t" << N_FOLLOW_DATA[i] / double(N_USERS) << "\t"
				<< log(i) << "\t" << log(N_FOLLOW_DATA[i] / double(N_USERS))
				<< "\n";
		OUTPUT << i + 0.5 << "\t" << N_FOLLOW_DATA[i] / double(N_USERS) << "\t"
				<< "\n";
	}

	OUTPUT.close();
}

/* This is a function that goes through the network and finds the probability
 of how many followers a given user will have. If you would like to use this
 function simply put P_IN = 1 in the INFILE. This program will produce an
 output file that can be plotted using gnuplot. */

void PIN(Network& network, int MAX_USERS, int N_USERS) {
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
		N_FOLLOWERS_DISTRO[network.n_following(i)]++;
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

