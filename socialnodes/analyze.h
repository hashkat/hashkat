#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include "dependencies/mtwist.h"
#include "state.h"

// Run a network simulation using the given input file's parameters
void analyze_network(std::string input_file, int seed);

#endif
