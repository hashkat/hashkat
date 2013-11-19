#ifndef __IO_H_
#define __IO_H_

#include <fstream>

#include "state.h"

void POUT(Network& network, int MAX_USERS, int N_USERS);
void PIN(Network& network, int MAX_USERS, int N_USERS);
void output_position(Network& network, int n_users);
#endif
