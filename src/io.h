#ifndef __IO_H_
#define __IO_H_

#include <fstream>

#include "analyze.h"
#include "network.h"

void POUT(Network& network, int MAX_ENTITIES, int N_ENTITIES, int N_FOLLOWS);
void PIN(Network& network, int MAX_ENTITIES, int N_ENTITIES, double R_FOLLOW_NORM);
void output_position(Network& network, int end_time);

void output_network_statistics(AnalysisState& state);

int factorial(int input_number);
void Categories_Check(CategoryGroup& tweeting, CategoryGroup& following, CategoryGroup& retweeting);
void Cumulative_Distro(Network& network, int MAX_ENTITIES, int N_ENTITIES, int N_FOLLOWS);
void entity_statistics(Network& network,int n_follows, int n_entities, int max_entities, EntityType* entitytype);
void tweets_distribution(Network& network, int n_users);

#endif
