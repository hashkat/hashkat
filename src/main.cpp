/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

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
#include "dependencies/lcommon/perf_timer.h"

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
    return default_val;
}

//** we avoid creating a header by pasting the 'test_main' prototype here:
int test_main(int argc, char** argv); // Defined in tests/main.cpp

int main(int argc, char** argv) {
#ifndef __sun
    std::locale loc("");
    std::cout.imbue(loc);
#endif

    if (has_flag(argc, argv, "--stdout-nobuffer")) {
        // Important mainly for colorization tool
        setvbuf(stdout,NULL,_IONBF,0); // Make stdout unbuffered
        cout.setf(std::ios::unitbuf);
    }

	if (has_flag(argc, argv, "--tests")) {
		// running tests:
		return test_main(argc, argv);
	} else {
	    printf("Network simulation started.\n");
	    // NOTE: We rely on INFILE.py to create a -generated version of our input file!
	    std::string INFILE = get_var_arg(argc, argv, "--input", "INFILE.yaml");
        INFILE += "-generated";
	    printf("Loading input configuration from '%s'.\n", INFILE.c_str());
        ParsedConfig config = parse_yaml_configuration(INFILE.c_str());
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

		printf("Starting simulation with seed '%d'.\n", seed);
		AnalysisState analysis_state(config, seed);

        analyzer_main(analysis_state);
        output_network_statistics(analysis_state);

		printf("Analysis took %.2fms.\n", t.get_microseconds() / 1000.0);

		return 0;
    }

	// Abnormal execution path
	return 0;
}
