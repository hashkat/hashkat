#ifndef __ANALYZE_H_
#define __ANALYZE_H_

#include <string>
#include <map>
#include "dependencies/mtwist.h"

// Run a network simulation using the given input file's parameters
void simulate_network(std::map<std::string, std::string>& config, int seed);

#endif
