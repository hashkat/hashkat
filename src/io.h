#ifndef __IO_H_
#define __IO_H_

#include <fstream>

#include "network.h"

void POUT(Network& network, int MAX_ENTITIES, int N_ENTITIES, int N_FOLLOWS);
void PIN(Network& network, int MAX_ENTITIES, int N_ENTITIES, double R_FOLLOW_NORM);
void output_position(Network& network, int n_entities);
int factorial(int input_number);
void Categories_Check(CategoryGroup& tweeting, CategoryGroup& following, CategoryGroup& retweeting);
void Cumulative_Distro(Network& network, int MAX_ENTITIES, int N_ENTITIES, int N_FOLLOWS);
void entity_statistics(Network& network,int n_follows, int n_entities, int max_entities, EntityType* entitytype);
#endif
