#include <vector>
#include <map>
#include "tests.h"

#include "dependencies/mtwist.h"

#include "config_dynamic.h"
#include "DataReadWrite.h"
#include "entity.h"
#include "analyzer.h"

using namespace std;

SUITE(serialization) {

    TEST(basics) {
        ParsedConfig config = parse_yaml_configuration("INFILE.yaml-generated");
        AnalysisState state(config, /*seed*/ 1);
        int test_num = 12321, read_val = -1;
        const char* file_name = "output/test_serialize_basics.sav";
        {
            DataWriter writer(state, file_name);
            writer << test_num;
        }
        {
            DataReader reader(state, file_name);
            reader << read_val;
        }
        CHECK(test_num == read_val);
    }

    TEST(entity) {
        ParsedConfig config = parse_yaml_configuration("INFILE.yaml-generated");
        AnalysisState state(config, /*seed*/ 1);
        int N_GENERATED = 3;
        state.network.allocate(N_GENERATED);
        state.network.n_entities = N_GENERATED;

        for (int i = 0; i < N_GENERATED; i++) {
            Entity& e = state.network[i];
            e.creation_time = 1;
            e.entity_type = 0;
            e.humour_bin = 0;
            e.language = LANG_FRENCH;
            e.preference_class = 0;
            e.n_tweets = 3;
            e.n_retweets = 4;
        }

        FollowerSet::Context context(state, /*entity id*/ 0);
        Entity& test = state.network[0];
        test.follower_set.add(context, 1);
        test.following_set.add(state, 2);

        Entity read;

        const char* file_name = "output/test_serialize_entity.sav";
        {
            DataWriter writer(state, file_name);
            test.visit(writer, context);
        }
        {
            DataReader reader(state, file_name);
            read.visit(reader, context);
        }

        CHECK(test.creation_time == read.creation_time);
        CHECK(test.entity_type == read.entity_type);
        CHECK(test.humour_bin == read.humour_bin);
        CHECK(test.language == read.language);
        CHECK(test.preference_class == read.preference_class);
        CHECK(test.n_tweets == read.n_tweets);
        CHECK(test.n_retweets == read.n_retweets);

        for (int id_fol : test.follower_set) {
            CHECK(id_fol == 1);
        }
        for (int id_fol : test.following_set) {
            CHECK(id_fol == 2);
        }

        CHECK(read.follower_set.size() == 1);
        CHECK(read.following_set.size() == 1);
    }
}

