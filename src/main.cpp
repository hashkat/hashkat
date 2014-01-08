#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <iostream>

#include "dependencies/ini.h"
#include "dependencies/UnitTest++.h"
#include "dependencies/lcommon.h"

#include "config_static.h"
#include "config_dynamic.h"

#include "network.h"
#include "analyze.h"
#include "io.h"

using namespace std;

static bool has_arg(int argc, char** argv, std::string test) {
	for (int i = 1; i < argc; i++) {
		if (argv[i] == test) {
			return true;
		}
	}
	return false;
}

//** we avoid creating a header by pasting the 'test_main' prototype here:
int test_main(int argc, char** argv); // Defined in tests/main.cpp
int uv_main(int argc, char** argv); // Defined in tests/libuv-learning.cpp

int main(int argc, char** argv) {
	if (has_arg(argc, argv, "--uv")) {
	    return uv_main(argc, argv);
	} else if (has_arg(argc, argv, "--tests")) {
		// running tests:
		return test_main(argc, argv);
	} else {
        ParsedConfig config = parse_yaml_configuration("INFILE.yaml");
		// or running analysis:
		Timer t;
		int seed = 1;
		if (has_arg(argc, argv, "--rand")) {
			time_t t;
			time(&t);
			seed = (int)t;
		}

		AnalysisState analysis_state(config, seed);

        simulate_network(analysis_state);
        output_network_statistics(analysis_state);

		printf("Analysis took %.2fms.\n", t.get_microseconds() / 1000.0);

		return 0;
    }

	// Abnormal execution path
	return 2;
}
