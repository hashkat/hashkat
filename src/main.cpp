#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <iostream>

#include "dependencies/ini.h"
#include "dependencies/UnitTest++.h"
#include "dependencies/lcommon.h"

#include "dependencies/lcommon/typename.h"

#include "config_static.h"
#include "config_dynamic.h"

#include "network.h"
#include "analyzer.h"
#include "io.h"

using namespace std;

// Check for a command-line flag
static bool has_flag(int argc, char** argv, std::string test) {
	for (int i = 1; i < argc; i++) {
		if (argv[i] == test) {
			return true;
		}
	}
	return false;
}

// Get a flag's argument
const char* get_var_arg(int argc, char** argv, std::string test, const char* default_val) {
    for (int i = 1; i < argc - 1; i++) {
        if (argv[i] == test) {
            return argv[i+1];
        }
    }
    return NULL;
}

//** we avoid creating a header by pasting the 'test_main' prototype here:
int test_main(int argc, char** argv); // Defined in tests/main.cpp
int uv_main(int argc, char** argv); // Defined in tests/libuv-learning.cpp

int main(int argc, char** argv) {
	if (has_flag(argc, argv, "--uv")) {
	    return uv_main(argc, argv);
	} else if (has_flag(argc, argv, "--tests")) {
		// running tests:
		return test_main(argc, argv);
	} else {
	    const char* INFILE = get_var_arg(argc, argv, "--input-file", "INFILE-generated.yaml");
        ParsedConfig config = parse_yaml_configuration("INFILE-generated.yaml");
        if (has_flag(argc, argv, "--handle-ctrlc")) {
            config.handle_ctrlc = true;
        }
		// or running analysis:
		Timer t;
		int seed = 1;
		if (has_flag(argc, argv, "--rand")) {
			time_t t;
			time(&t);
			seed = (int)t;
		}

		AnalysisState analysis_state(config, seed);

        analyzer_main(analysis_state);
        output_network_statistics(analysis_state);

		printf("Analysis took %.2fms.\n", t.get_microseconds() / 1000.0);

		return 0;
    }

	// Abnormal execution path
	return 2;
}
