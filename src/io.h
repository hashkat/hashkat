#ifndef __IO_H_
#define __IO_H_

#include <fstream>

#include "network.h"

void POUT(Network& network, int MAX_USERS, int N_USERS, int N_FOLLOWS);
void PIN(Network& network, int MAX_USERS, int N_USERS, double R_FOLLOW_NORM);
void output_position(Network& network, int n_users);
int factorial(int input_number);
#endif
