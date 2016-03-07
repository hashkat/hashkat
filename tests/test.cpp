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
#include <cstdlib>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>

using boost::test_tools::output_test_stream;

struct INIT_TEST
{
    INIT_TEST()
    :   folder_(std::getenv("HASHKAT"))
    {
        if (!folder_.empty())
            folder_ += "/tests/referencefiles/";
        else
            std::cout << "HASHKAT environment variable is not defined\n";

        files_ =
        {
            "Bot_info", "Celebrity_info",
            "cumulative-degree_distribution_month_000", "dd_by_follow_model",
            "in-degree_distribution_month_000","main_stats", "model_match",
            "network", "Organization_info", "out-degree_distribution_month_000",
            "region_connection_matrix_month_000", "retweets_distro",
            "Standard_info", "tweets_distro"
        };
    }

    bool ready() const
    {   return !folder_.empty();  }

    void run(const std::string& test)
    {
        std::string cmd("python ");
        cmd += std::getenv("HASHKAT");
        cmd += "/hashkat_pre.py --input ";
        cmd += folder_ + test;
        cmd += "/INFILE.yaml >/dev/null 2>&1";
        //cmd += "/INFILE.yaml";
        std::system(cmd.c_str());

        cmd = "cd ";
        cmd += folder_ + test + ';';
        cmd += std::getenv("HASHKAT");
        cmd += "/build/src/hashkat >/dev/null 2>&1";
        //cmd += "/build/src/hashkat";
        std::system(cmd.c_str());
    }

    void verify(const std::string& test)
    {
        std::string folder = folder_ + test + "/output/";
        for (auto file : files_)
        {
            std::ifstream cin(folder + file + ".dat");
            //BOOST_CHECK_MESSAGE(cin, "no .dat file to read");
            if (cin)
            {
                output_test_stream cout(folder + file + ".ref");
                cout << cin.rdbuf();
                //std::cout << file << std::endl;
                BOOST_CHECK(cout.match_pattern());
            }
        }
    }

    void clean(const std::string& test)
    {
        std::string cmd("cd ");
        cmd += folder_ + test;
        cmd += ";rm -f network_state.dat INFILE.yaml-generated output/*.dat output/*.gexf output/DATA_vs_TIME";
        std::system(cmd.c_str());
    }

private:
    std::string folder_;
    std::array<std::string, 14> files_;
};

BOOST_FIXTURE_TEST_CASE(test01, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test01");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test03, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test03");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test04, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test04");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test05, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test05");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test06, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test06");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test07, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test07");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test08, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test08");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test09, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test09");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test10, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test10");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(test11, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("test11");
    run(test);
    verify(test);
    clean(test);
}

//BOOST_FIXTURE_TEST_CASE(test12, INIT_TEST)
//{
//    BOOST_REQUIRE(ready());
//    std::string test("test12");
//    run(test);
//    verify(test);
//    clean(test);
//}

//BOOST_FIXTURE_TEST_CASE(test99, INIT_TEST)
//{
//    BOOST_REQUIRE(ready());
//    std::string test("test99");
//    run(test);
//    verify(test);
//    clean(test);
//}

BOOST_FIXTURE_TEST_CASE(refactoring01, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("refactoring01");
    run(test);
    verify(test);
    clean(test);
}

BOOST_FIXTURE_TEST_CASE(refactoring02, INIT_TEST)
{
    BOOST_REQUIRE(ready());
    std::string test("refactoring02");
    run(test);
    verify(test);
    clean(test);
}
