#ifndef __IO_H_
#define __IO_H_

#include <fstream>

#include "state.h"

// Read the variable from the given 'input_file' and 'demand'ed variable
double load_config_var(std::string input_file, std::string demand);

void POUT(Network& network, int MAX_USERS, int N_USERS);

#endif
