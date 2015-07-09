///////////////////////////////////////////////////////////////////////////////
// This file is part of the #KAT Social Network Simulator.
//
// The #KAT Social Network Simulator is free software: you can redistribute it
// and/or modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// The #KAT Social Network Simulator is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// the #KAT Social Network Simulator. If not, see http://www.gnu.org/licenses.
//
// Addendum:
//
// Under this license, derivations of the #KAT Social Network Simulator
// typically must be provided in source form. The #KAT Social Network Simulator
// and derivations thereof may be relicensed by decision of the original
// authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case
// of a derivation, subsequent authors.
//
// build: g++ -std=c++11 test.cpp -o test

#include <array>
#include <iostream>
#include <fstream>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>

using boost::test_tools::output_test_stream;

struct INIT_TEST
{
    INIT_TEST()
    {
        files =
        {
            "Bot_info", "Celebrity_info",
            "cumulative-degree_distribution_month_000", "dd_by_follow_model",
            "in-degree_distribution_month_000","main_stats", "model_match",
            "network", "Organization_info", "out-degree_distribution_month_000",
            "region_connection_matrix_month_000", "retweets_distro",
            "Standard_info", "tweets_distro"
        };
    }

    std::array<std::string, 14> files;
};

BOOST_FIXTURE_TEST_CASE(test01, INIT_TEST)
{
    std::string folder = std::getenv("HASHKAT");
    folder += "/tests/referencefiles/test01/output/";
    //std::cout << folder << std::endl;
    for (auto file : files)
    {
        //std::cout << file << std::endl;
        std::ifstream cin(folder + file + ".ref"); // ".dat");
        output_test_stream cout(folder + file + ".ref");
        cout << cin.rdbuf();
        BOOST_CHECK(cout.match_pattern());
    }
}
