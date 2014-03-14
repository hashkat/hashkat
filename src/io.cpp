#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>

#include "dependencies/mtwist.h"
#include "analyzer.h"
#include "io.h"
#include "util.h"
#include "tweets.h"

using namespace std;


// ROOT OUTPUT ROUTINE
/* After 'analyze', print the results of the computations. */
void output_network_statistics(AnalysisState& state) {
    Network& network = state.network;
    for (int i = 0; i < min(10, network.n_entities); i++) {
        Entity& e = network[i];
        EntityType& et = state.entity_types[e.entity_type];

        printf("------------------------------------------------------------------------\n", i);
        printf("(Entity %d)\n", i);
        printf("(EntityType %s)\n", et.name.c_str());
        printf("------------------------------------------------------------------------\n", i);
        FollowerSet::Context context(state, i);
        e.follower_set.print(context);
    }
    ParsedConfig& C = state.config;
    EntityTypeVector& et_vec = state.entity_types;
    AnalysisStats& stats = state.stats;
    TweetBank& tb = state.tweet_bank;
    MostPopularTweet& mpt = state.most_pop_tweet;
    
    // Print why program stopped
    if (C.output_stdout_basic) {
        if (SIGNAL_ATTEMPTS > 0) {
            cout << "\nSimulation (Gracefully) Interrupted: ctrl-c was pressed\n";
        } else if (state.end_time >= C.max_sim_time) {
            cout << "\nSimulation Completed: desired duration reached\n";
        } else if (network.n_entities >= C.max_entities) {
            cout << "\nSimulation Completed: desired entity amount reached\n";
        } else {
            cout << "\nSimulation Completed: desired wall-clock time reached\n";
        }
        cout << "\nCreating analysis files -- press ctrl-c multiple times to abort ... \n";
    }
    int MAX_ENTITIES = C.max_entities;
    double rate_add = C.rate_add;
    int N_ENTITIES = network.n_entities;
    int initial_entities = C.initial_entities;

    // Depending on our INFILE/configuration, we may output various analysis
    if (C.output_visualize) {
        output_position(network, N_ENTITIES);
    }
    /* ADD FUNCTIONS THAT RUN AFTER NETWORK IS BUILT HERE */
    if (C.use_barabasi) {
        Categories_Check(state.tweet_ranks, state.follow_ranks, state.retweet_ranks);
    }
    if (C.output_tweet_analysis) {
        tweets_distribution(network, N_ENTITIES);
    }
    if (entity_checks(et_vec, network, state, state.add_rates, initial_entities)) {
        //cout << "Number of events are valid.\n";
    } else {
        cout << "Numbers are events are not valid, adjust the tolerance or check for errors.\n";
    }
    if (C.entity_stats) {
        whos_following_who(et_vec, network);
    }
    if (C.output_stdout_basic) {
        cout << "Analysis complete!\n";
    }
    if (C.degree_distributions) {
        degree_distributions(network);
    }
   visualize_most_popular_tweet(mpt, network);
//    retweet_analysis(stats, tb);
}

// edgelist created for R (analysis) and python executable (drawing) and gephi outputfile
void output_position(Network& network, int n_entities) {
    ofstream output1;
    output1.open("network.gexf");
    output1 << "<gexf version=\"1.2\">\n"
            << "<meta lastmodifieddate=\"2013-11-21\">\n"
            << "<creator> Kevin Ryczko </creator>\n"
            << "<description> Social Network Visualization </description>\n"
            << "</meta>\n"
            << "<graph mode=\"static\" defaultedgetype=\"directed\">\n"
            << "<nodes>\n";
    int count = 0;
    if (n_entities <= 10000) {
        for (int i = 0; i < n_entities; i++) {
                Entity& p = network[i];
                output1 << "<node id=\"" << i << "\" label=\"" << p.entity_type << "\" />\n";
        }
        output1 << "</nodes>\n" << "<edges>\n";
        for (int id = 0; id < n_entities; id++) {
            for (int id_fol : network.following_set(id)) {
                output1 << "<edge id=\"" << count << "\" source=\"" << id
                        << "\" target=\"" << id_fol << "\"/>\n";
                count++;
            }
        }
        output1 << "</edges>\n" << "</graph>\n" << "</gexf>";
        output1.close();
    }

    else {
        MTwist rng;
        rng.init_genrand(/* Fixed seed */ 2);
        int fraction_users = 10000;
        vector<int> user_ids (fraction_users);
        for (int& id : user_ids) {
            id = rng.rand_int(n_entities);
        }

        for (int& id : user_ids) {
                Entity& p = network[id];
                output1 << "<node id=\"" << id << "\" label=\"" << p.entity_type << "\" />\n";

                for (int id_fol : network.following_set(id)) {
                    Entity& p1 = network[id_fol];
                    output1 << "<node id=\"" << id_fol << "\" label=\"" << p1.entity_type << " - followed"<< "\" />\n";
                }
        }
        output1 << "</nodes>\n" << "<edges>\n";
        int count = 0;
        for (int& id : user_ids) {
            for (int id_fol : network.following_set(id)) {
                output1 << "<edge id=\"" << count << "\" source=\""
                        << id << "\" target=\"" << id_fol << "\"/>\n";
                count++;
            }
        }
        output1 << "</edges>\n" << "</graph>\n" << "</gexf>";
        output1.close();
    }

    ofstream output;
    output.open("output/network.dat");
    for (int id = 0; id < n_entities; id++) {
        for (int id_fol : network.follower_set(id)) {
            output << id << "\t" << id_fol << "\n";
        }
    }
    output.close();
} 

const int POUT_CAP = 1000; // AD: Rough work

/* This is a function that gives you the probability of a entity following a
 a given number of people. If you want to use this function, simply put
 P_OUT = 1 in the INFILE. This function will generate an output file that
 can be plotted using gnuplot. */

void degree_distributions(Network& network) {
    int max_following = 0, max_followers = 0;
    for (int i = 0; i < network.n_entities; i ++) {
        if (network.n_following(i) >= max_following) {
            max_following = network.n_following(i) + 1;
        }
        if (network.n_followers(i) >= max_followers) {
            max_followers = network.n_followers(i) + 1;
        }
    }

    int max_degree = max_following + max_followers;

    // now that we have our boundaries lets plot our degree distributions

    // open some files
    ofstream outdd, indd, cumuldd;
    outdd.open("output/out-degree_distribution.dat");
    indd.open("output/in-degree_distribution.dat");
    cumuldd.open("output/cumulative_distribution.dat");

    outdd << "# This is the out-degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";
    indd << "# This is the in-degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";
    cumuldd << "# This is the cumulative degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";

    //declare and set arrays to 0
    vector<int> out_degree_distro (max_following);
    vector<int> in_degree_distro (max_followers);
    vector<int> cumulative_distro (max_degree);
    for (int i = 0; i < max_following; i ++ ) {
        out_degree_distro[i] = 0;
    }
    for (int i = 0; i < max_followers; i ++ ) {
        in_degree_distro[i] = 0;
    }
    for (int i = 0; i < max_degree; i ++ ) {
        cumulative_distro[i] = 0;
    }

    //go through network and generate distributions
    for (int i = 0; i < network.n_entities; i ++) {
        out_degree_distro[network.n_following(i)] ++;
        in_degree_distro[network.n_followers(i)] ++;
        cumulative_distro[network.n_followers(i) + network.n_following(i)] ++;
    }
    // output the distributions
    for (int i = 0; i < max_following; i ++) {
        outdd << i << "\t" << out_degree_distro[i] / (double) network.n_entities << "\t" << log(i) << "\t" << log(out_degree_distro[i] / (double)network.n_entities) << "\n";
    }
    for (int i = 0; i < max_followers; i ++) {
        indd << i << "\t" << in_degree_distro[i] / (double) network.n_entities << "\t" << log(i) << "\t" << log(in_degree_distro[i] / (double)network.n_entities) << "\n";
    }
    for (int i = 0; i < max_degree; i ++) {
        cumuldd << i << "\t" << cumulative_distro[i] / (double) network.n_entities << "\t" << log(i) << "\t" << log(cumulative_distro[i] / (double)network.n_entities) << "\n";
    }
    outdd.close();
    indd.close();
    cumuldd.close();
}

int factorial(int input_number) {
    int value = 1;
    for (int i = 1; i <= input_number; i ++) {
        value *= i;
    }
    return value;
}

static void category_print(ofstream& output, const char* name, CategoryGrouper& group) {
    output << name << " | ";
    for (int i = 0; i < group.categories.size(); i ++) {
        CategoryEntityList& c = group.categories[i];
        output << c.entities.size() << " at " << c.threshold << "|\t";
    }
    output << '\n';
}

void Categories_Check(CategoryGrouper& tweeting, CategoryGrouper& following, CategoryGrouper& retweeting) {
    ofstream output;
    output.open("output/Categories_Distro.dat");
    category_print(output, "Tweeting", tweeting);
    category_print(output, "Following", following);
    category_print(output, "Retweeting", retweeting);
    output.close();
}

// produces an output file with entity type statistics, makes sure things are working properly
void entity_statistics(Network& network, int n_follows, int n_entities, int max_entities, EntityType* entitytype) {
    ofstream output;
    output.open("output/entity_percentages.dat");
    vector<int> entity_counts(max_entities);
    vector<int> average_followers_from_network(max_entities);
    vector<int> average_followers_from_lists(max_entities);
    for (int i = 0 ; i < max_entities; i ++) {
        entity_counts[i] = 0;
        average_followers_from_network[i] = 0;
        average_followers_from_lists[i] = 0;
    }
    for (int i = 0 ; i < n_entities; i ++) {
        Entity& p = network[i];
        for (int j = 0; j < max_entities; j ++) {
            if (p.entity_type == j) {
                entity_counts[j] ++;
                average_followers_from_network[j] += p.follower_set.size();
            }
        }
    }
    output << "\n ****** Info regarding numbers of entity types - BASED ON NETWORK ARRAY ****** \n\n";
    for (int i = 0; i < max_entities; i ++) {
        output << "Number of entity " << i << " : " << entity_counts[i] << "    Fraction: " << entity_counts[i]/double(n_entities) << "\n";
    }
    output << "\n ****** Info regarding numbers of entity types - BASED ON USER_LISTS ****** \n THIS SHOULD BE THE SAME AS ABOVE \n\n";
    for (int i = 0; i < max_entities; i ++) {
        output << "Number of entity " << i << " : " << entitytype[i].entity_list.size() << "    Fraction: " << entitytype[i].entity_list.size()/double(n_entities) << "\n";
    }
    output << "\n\n ****** Info regarding following certain entity types - BASED ON NETWORK ARRAY ****** \n\n";
    double sum = 0;
    for (int i = 0; i < max_entities; i ++) {
        sum += average_followers_from_network[i]/double(entity_counts[i]);
    }
    for (int i = 0; i < max_entities; i ++) {
        output << "Average number of follows for entity "
               << i << ": " << average_followers_from_network[i]/double(entity_counts[i])
               << "     Fraction: " << average_followers_from_network[i]/double(n_follows) << "\n";
    }
    for (int i = 0; i < max_entities; i ++){
        for (int j = 0; j < entitytype[i].entity_list.size(); j++) {
            Entity& p = network[entitytype[i].entity_list[j]];
            average_followers_from_lists[i] += p.follower_set.size();
        }
    }
    output << "\n\n ****** Info regarding following certain entity types - BASED ON USER_LISTS ****** \n      SHOULD BE THE SAME AS ABOVE\n\n";
    double sum1 = 0;
    for (int i = 0; i < max_entities; i ++) {
        sum1 += average_followers_from_lists[i] / double(entitytype[i].entity_list.size());
    }
    for (int i = 0; i < max_entities; i ++) {
        output << "Average number of follows for entity "
               << i << ": " << average_followers_from_lists[i]/double(entitytype[i].entity_list.size())
               << "     Fraction: " << average_followers_from_network[i]/double(n_follows) << "\n";
    }
    output.close();
}

// function to see the distribution of tweets and retweets
void tweets_distribution(Network& network, int n_users) {
    ofstream tweet_output, retweet_output;
    tweet_output.open("output/tweets_distro.dat");
    retweet_output.open("output/retweets_distro.dat");

    int max_tweets = 0, max_retweets = 0;
    double tweets_sum = 0, retweets_sum = 0;
    for (int i = 0; i < n_users; i ++) {
        Entity& e = network[i];
        tweets_sum += e.n_tweets;
        retweets_sum += e.n_retweets;
        if (e.n_tweets > max_tweets) {
            max_tweets = e.n_tweets;
        }
        if (e.n_retweets > max_retweets) {
            max_retweets = e.n_retweets;
        }
    }
    vector<int> tweets_distro (max_tweets + 1), retweets_distro (max_retweets + 1);
    for (int i = 0; i < max_tweets; i ++) {
        tweets_distro[i] = 0;
    }
    for (int i = 0; i < max_retweets; i ++) {
        retweets_distro[i] = 0;
    }
    for (int i = 0; i < n_users; i ++) {
        Entity& e = network[i];
        tweets_distro.at(e.n_tweets) ++;
        retweets_distro.at(e.n_retweets) ++;
    }
    tweet_output << "# n_tweets\tdistro\n\n";
    for (int i = 0; i < max_tweets; i ++) {
        tweet_output << i << "\t" << tweets_distro.at(i) / tweets_sum << "\n";
    }
    retweet_output << "# n_retweets\tdistro\n\n";
    for (int i = 0; i < max_retweets; i ++) {
        retweet_output << i << "\t" << retweets_distro.at(i) / retweets_sum << "\n";
    }
    tweet_output.close();
    retweet_output.close();
}

// group the quick_rate_check and entity_checks together, they're dependent on one another
bool quick_rate_check(EntityTypeVector& ets, double& correct_val, int& i, int& j) {
    double tolerence = 0.05;
    if (j == 1 && abs(correct_val - ets[i].n_follows) / correct_val >= tolerence) {
        //cout << "\nNumber of follows for entity type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        //cout << "Was: " << ets[i].n_follows << endl;
        return false;
    } else if (j == 2 && abs(correct_val - ets[i].n_tweets) / correct_val >= tolerence) {
        //cout << "\nNumber of tweets for entity type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        //cout << "Was: " << ets[i].n_tweets << endl;
        return false;
    } else if (j == 3 && abs(correct_val - ets[i].n_retweets) / correct_val >= tolerence) {
        //cout << "\nNumber of retweets for entity type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        //cout << "Was: " << ets[i].n_retweets << endl;
        return false;
    }
    return true;
}

bool entity_checks(EntityTypeVector& ets, Network& network, AnalysisState& state, Add_Rates& add_rates, int& initial_entities) {
    double tolerence = 0.05;
    int final_check = 0;
    int check_count = 0;
    double rate_add = add_rates.RF.monthly_rates[state.n_months()];
    for (int i = 0; i < ets.size(); i ++) {
        double add_correct = network.n_entities * ets[i].prob_add;
        if (abs(add_correct - ets[i].entity_list.size()) / add_correct >= tolerence) {
            //cout << "\nNumber of entity type \'" << ets[i].name << "\' is not correct. " << (int) add_correct << " is the right number.\n";
            //cout << "was: " << ets[i].entity_list.size() << "\n";
            final_check += false;
            check_count ++;
        }
        for (int j = 0; j < number_of_diff_events; j ++) {
            if (rate_add == 0 && ets[i].RF[j].function_type == "constant") {
                double correct_val = ets[i].RF[j].const_val * state.time * network.n_entities * ets[i].prob_add;
                final_check += quick_rate_check(ets, correct_val, i, j);
                check_count ++;
            } else if (rate_add == 0 && ets[i].RF[j].function_type == "linear" ) {
                double correct_val = (ets[i].RF[j].y_intercept + 0.5 * ets[i].RF[j].slope * state.n_months()) * state.time * network.n_entities * ets[i].prob_add;
                final_check += quick_rate_check(ets, correct_val, i, j);
                check_count ++;
            } else if (rate_add != 0 && ets[i].RF[j].function_type == "constant") {
                double correct_val = 0.5 * (ets[i].RF[j].const_val / rate_add) * ((rate_add * rate_add) * ets[i].prob_add * (state.time * state.time) + (rate_add * state.time) - (initial_entities - 1) * initial_entities);
                final_check += quick_rate_check(ets, correct_val, i, j);
                check_count ++;
            } else if (rate_add != 0 && ets[i].RF[j].function_type == "linear" ) {
                double correct_val = (0.5 * ets[i].RF[j].y_intercept * rate_add * ets[i].prob_add * state.time * state.time) + 0.25 * ets[i].RF[j].slope * state.n_months() * state.time + 0.3333 * rate_add * ets[i].RF[j].slope * state.n_months() * ets[i].prob_add * state.time * state.time * 0.5 + 0.5 * ets[i].RF[j].y_intercept * state.time - (initial_entities - 1) * initial_entities;
                final_check += quick_rate_check(ets, correct_val, i, j);
                check_count ++;
                
            } 
        }
    }
    if (final_check == check_count) {
        return true;
    }
    return false;
}

static void whos_following_who(EntityTypeVector& types, EntityType& type, Network& network) {
    string filename = "output/" + type.name + "_info.dat";
    ofstream output;
    output.open(filename.c_str());
    int max_degree = 0;
    for (int i = 0; i < type.entity_list.size(); i ++) {
        int degree = network.n_following(type.entity_list[i]) + network.n_followers(type.entity_list[i]);
        if (degree > max_degree) {
            max_degree = degree;
        }
    }

    /* Declare 0-filled counting-vectors. */
    vector<int> entity_followers(max_degree + 1 /* AD: Needed one past max stored*/, 0);
    vector<int> entity_following(max_degree + 1, 0);
    vector<int> entity_degree(max_degree + 1, 0);
    vector<int> who_following(types.size(), 0);
    vector<int> who_followers(types.size(), 0);

    double followers_sum = 0, following_sum = 0;
    for (int i = 0; i < type.entity_list.size(); i ++) {
        int id = type.entity_list[i];
        int in_degree = network.n_followers(id);
        int out_degree = network.n_following(id);
        entity_followers[in_degree] ++;
        entity_following[out_degree] ++;
        entity_degree[in_degree + out_degree] ++;

        // Analyze ins == followers
        for (int id_fol : network.follower_set(id)) {
            Entity& et = network[id_fol];
            who_following[et.entity_type] ++;
            following_sum ++;
        }

        // Analyze outs == follows
        for (int id_fol : network.following_set(id)) {
            Entity& et = network[id_fol];
            who_followers[et.entity_type] ++;
            followers_sum ++;
        }
    }
    output << "# Entity percentages following entity type \'" << type.name << "\'\n# ";
    for (int i = 0; i < types.size(); i ++) {
        output << types[i].name << ": " << who_following[i] / following_sum * 100.0 << "   ";
    }
    output << "\n# Entity percentages that entity type \'" << type.name << "\' follows\n# ";
    for (int i = 0; i < types.size(); i ++) {
        output << types[i].name << ": " << who_followers[i] / followers_sum * 100.0 << "   ";
    }
    output << "\n# degree\tin_degree\tout_degree\tcumulative\tlog(degree)\tlog(in_degree)\tlog(out_degree)\tlog(cumulative)\n\n";
    for (int i = 0; i < max_degree; i++) {
        output << i << "\t"
                << entity_followers[i] / (double) type.entity_list.size() << "\t"
                << entity_following[i] / (double) type.entity_list.size() << "\t"
                << entity_degree[i] / (double) type.entity_list.size()
                << "\t" << log(i) << "\t"
                << log(entity_followers[i] / (double) type.entity_list.size()) << "\t"
                << log(entity_following[i] / (double) type.entity_list.size()) << "\t"
                << log(entity_degree[i] / (double) type.entity_list.size()) << "\n";
    }
    output.close();
}

// function that will plot degree distributions for every entity, and at the top
// of the files gives you info about the percentage of each entity they are following
void whos_following_who(EntityTypeVector& types, Network& network) {
    for (int i = 0; i < types.size(); i ++ ) {
        whos_following_who(types, types[i], network);
    }
}

void visualize_most_popular_tweet(MostPopularTweet& mpt, Network& network) {
    ofstream output;
    output.open("output/retweet_viz.gexf");
    Tweet& t = mpt.most_popular_tweet;
    if (t.content.empty()) {
        return; // Nothing to see here
    }
    
    output << "<gexf xmlns=\"http://www.gexf.net/1.2draft\" xmlns:viz=\"http://www.gexf.net/1.2draft/viz\">\n"
            << "<meta lastmodifieddate=\"2013-11-21\">\n"
            << "<creator> Kevin Ryczko </creator>\n"
            << "<description> Social Network Visualization </description>\n"
            << "</meta>\n"
            << "<graph mode=\"static\" defaultedgetype=\"directed\">\n"
            << "<nodes>\n";
            
    UsedEntities& used_set = t.content->used_entities;
    for (int id_used : used_set) {
        output << "<node id=\"" << id_used << "\" label=\"" << "Retweeters" << "\">\n";
        output << "<viz:size value=\"2.5\"/>\n";
        output << "</node>\n";
        for (int id_fol : network.follower_set(id_used)) {
            Entity& e = network[id_fol];
            output << "<node id=\"" << id_fol << "\" label=\"" << "Non-Retweeters" << "\" >\n";
            output << "<viz:size value=\"2.0\"/>\n";
            output << "</node>\n";
        }
    }
    output << "<node id=\"" << t.id_tweeter << "\" label=\"" << "Main-Tweeter" << "\" >\n";
    output << "<viz:size value=\"3.0\"/>\n";
    output << "</node>\n";
    int count = 0;
    output << "</nodes>\n" << "<edges>\n";

    for (int id : network.follower_set(t.id_tweeter)) {
        output << "<edge id=\"" << count << "\" source=\"" << id
                << "\" target=\"" << t.id_tweeter << "\"/>\n";
        count ++;
    }
    for (int id_used : used_set) {
        for (int id_fol : network.follower_set(id_used)) {
            output << "<edge id=\"" << count << "\" source=\"" << id_fol
                    << "\" target=\"" << id_used << "\"/>\n";
            count ++;
        }
    }
    output << "</edges>\n" << "</graph>\n" << "</gexf>";
    output.close();
}
