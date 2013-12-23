#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <sstream>
#include <iostream>

#include "dependencies/ini.h"
#include "dependencies/UnitTest++.h"
#include "dependencies/lcommon.h"

#include "analyze.h"

using namespace std;

// Called using a function pointer by the ini-loading library.
// We do not care about ini file sections or entities.
// Stores arbitrary configuration in CONFIG.
static int loader_callback(void* vCONFIG, const char* unused_section,
        const char* name, const char* value) {
    //** This is a low-level API, we have to convert to our desired type
    map<string, string>& CONFIG = *(map<string, string>*)vCONFIG;
    CONFIG[name] = value;
    return 1;
}

// Read an arbitrary configuration from a file, store it in CONFIG.
bool load_config(map<string, string>& config, string file_name) {
    //** This is a low-level API, we have to pass a generic void* type
    int error = ini_parse(file_name.c_str(), loader_callback, (void*)&config);
    if (error < 0) {
        printf("Can't read '%s'!\n", file_name.c_str());
        return false;
    } else if (error) {
        printf("Bad config file (first error on line %d)!\n", error);
        return false;
    }
    return true;
}

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

int main(int argc, char** argv) {
    std::map<string, string> config;
	if (has_arg(argc, argv, "--tests")) {
		// running tests:
		return test_main(argc, argv);
	} else if (load_config(config, "INFILE")) {
		// or running analysis:
		Timer t;
		int seed = 1;
		if (has_arg(argc, argv, "--rand")) {
			time_t t;
			time(&t);
			seed = (int)t;
		}
        simulate_network(config, seed);
		printf("Analysis took %.2fms.\n", t.get_microseconds() / 1000.0);
		return 0;
    }

	// Abnormal execution path
	return 2;
}
