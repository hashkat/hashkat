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

static map<string, string> RAW_CONFIG;
static map<string, double> NUM_CONFIG;

// Called using a function pointer by the ini-loading library.
// We do not care about ini file sections or users.
// Stores arbitrary configuration in CONFIG.
static int loader_callback(void* unused_user, const char* unused_section,
        const char* name, const char* value) {
    stringstream string_converter(value); //** Think of this like 'cin', but instead of standard input its an arbitrary string.
    string_converter >> NUM_CONFIG[name]; //** Will adapt to the appropriate type! also like 'cin' would.
    RAW_CONFIG[name] = value;
    return 1;
}

// Read an arbitrary configuration from a file, store it in CONFIG.
bool load_config(string name) {
    int error = ini_parse(name.c_str(), loader_callback, NULL);
    if (error < 0) {
        printf("Can't read '%s'!\n", name.c_str());
        return false;
    } else if (error) {
        printf("Bad config file (first error on line %d)!\n", error);
        return false;
    }
    return true;
}

static bool has_arg(int argc, char** argv, std::string test) {
	for (int i = 1; i < argc; i++) {
		cout << argv[i] << " vs " << test << endl;
		if (argv[i] == test) {
			return true;
		}
	}
	return false;
}

int main(int argc, char** argv) {
	if (has_arg(argc, argv, "--tests")) {
		// running tests:
		return UnitTest::RunAllTests();
	} else if (load_config("INFILE")) {
		// or running analysis:
		Timer t;
		int seed = 1;
		if (has_arg(argc, argv, "--rand")) {
			time_t t;
			time(&t);
			seed = (int)t;
		}
        analyze_network(RAW_CONFIG, NUM_CONFIG, seed);
		printf("Analysis took %.2fms.\n", t.get_microseconds() / 1000.0);
		return 0;
    }

	// Abnormal execution path
	return 2;
}
