#include <vector>
#include <map>
#include "tests.h"

#include "dependencies/mtwist.h"

#include "config_dynamic.h"
#include "serialization.h"
#include "agent.h"
#include "analyzer.h"

using namespace std;

SUITE(serialization) {

    TEST(basics) {
        ParsedConfig config = parse_yaml_configuration("INFILE.yaml-generated");
        AnalysisState state(config, /*seed*/ 1);
        int test_num = 12321, read_val = -1;
        const char* file_name = "output/test_serialize_basics.dat";
        {
            std::ofstream file(file_name);
            JsonWriter writer(state, file);
            writer(NVP(test_num));
        }
        {
            std::ifstream file(file_name);
            JsonReader reader(state, file);
            reader(cereal::make_nvp("test_num", read_val));
        }
        CHECK(test_num == read_val);
    }

    void check_eq(Agent& test, Agent& read) {
        CHECK(test.creation_time == read.creation_time);
        CHECK(test.agent_type == read.agent_type);
        CHECK(test.ideology_bin == read.ideology_bin);
        CHECK(test.region_bin == read.region_bin);
        CHECK(test.language == read.language);
        CHECK(test.preference_class == read.preference_class);
        CHECK(test.n_tweets == read.n_tweets);
        CHECK(test.n_retweets == read.n_retweets);

        CHECK(read.follower_set.size() == test.follower_set.size());
        CHECK(read.following_set.size() == read.following_set.size());
    }

    TEST(network) {
        ParsedConfig config = parse_yaml_configuration("INFILE.yaml-generated");
        AnalysisState state(config, /*seed*/ 1);
        int N_GENERATED = 3;
        state.network.allocate(N_GENERATED);
//        state.network.n_agents = N_GENERATED; // removed for refactoring

        for (int i = 0; i < N_GENERATED; i++) {
            state.network.grow();
            Agent& e = state.network[i];
            e.id = i;
            e.creation_time = 1;
            e.agent_type = 0;
            e.language = LANG_FRENCH;
            e.region_bin = 0;
            e.ideology_bin = 0;
            e.preference_class = 0;
            e.n_tweets = 3;
            e.n_retweets = 4;
        }

        Agent& test = state.network[0];
        test.follower_set.add(state.network[1]);
        test.following_set.add(state, 2);

        Agent read;

        const char* file_name = "output/test_serialize_agent.json";
        {
            std::ofstream file(file_name);
            JsonWriter writer(state, file);
            writer(test);
        }
        {
            std::ifstream file(file_name);
            JsonReader reader(state, file);
            reader(read);
            read.post_load(state);
        }

        check_eq(test, read);

        for (int id_fol : test.follower_set.as_vector()) {
            CHECK(id_fol == 1);
        }
        for (int id_fol : test.following_set.as_vector()) {
            CHECK(id_fol == 2);
        }

        /*Test network serialization:*/ {
            std::ofstream file(file_name);
            JsonWriter writer(state, file);
            writer(state.network);
        }
        {
            std::ifstream file(file_name);
            JsonReader reader(state, file);
            Network network;
            reader(network);
            CHECK(network.size() == state.network.size());
            CHECK(network.max_size() == state.network.max_size());
            for (int i = 0; i < network.size(); i++) {
                check_eq(network[i], state.network[i]);
            }
        }
    }
}

