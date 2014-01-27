#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>

#include "dependencies/mtwist.h"
#include "analyzer.h"
#include "io.h"
#include "util.h"

using namespace std;


// ROOT OUTPUT ROUTINE
/* After 'analyze', print the results of the computations. */
void output_network_statistics(AnalysisState& state) {
    Network& network = state.network;
    ParsedConfig& C = state.config;
    EntityTypeVector& et_vec = state.entity_types;
    AnalysisStats& stats = state.stats;
    // Print why program stopped
    if (C.output_stdout_basic) {
        if (CTRL_C_ATTEMPTS > 0) {
            cout << "\nSimulation (Gracefully) Interrupted: ctrl-c was pressed\n";
        } else if (state.end_time >= C.max_time) {
            cout << "\nSimulation Completed: desired duration reached\n";
        } else {
            cout << "\nSimulation Completed: desired entity amount reached\n";
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
    if (entity_checks(et_vec, network, state, rate_add, initial_entities)) {
        cout << "Numbers check out :-)\n";
    } else {
        cout << "Numbers are wrong :-(\n";
    }
    

    //entity_statistics(network, N_FOLLOWS,N_ENTITIES, N_ENTITIES, entity_entities);
    whos_following_who(et_vec, network);

    if (C.output_stdout_basic) {
        cout << "Analysis complete!\n";
    }
//        DATA_TIME.close();
    degree_distributions(network);

}

// edgelist created for R (analysis) and python executable (drawing) and gephi outputfile
void output_position(Network& network, int n_entities) {
    ofstream output1;
    output1.open("POSITIONS.gexf");
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
                output1 << "<node id=\"" << i << "\" label=\"" << p.entity << "\" />\n";
        }
        output1 << "</nodes>\n" << "<edges>\n";
        for (int i = 0; i < n_entities; i++) {
                for (int j = 0; j < network.n_following(i); j++) {
                        output1 << "<edge id=\"" << count << "\" source=\"" << i
                                        << "\" target=\"" << network.follow_i(i, j) << "\"/>\n";
                        count ++;
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
        for (int i = 0; i < fraction_users; i ++) {
            user_ids[i] = rng.rand_int(n_entities);
        }

        for (int i = 0; i < fraction_users; i++) {
                Entity& p = network[user_ids[i]];
                output1 << "<node id=\"" << user_ids[i] << "\" label=\"" << p.entity << "\" />\n";
                for (int j = 0; j < network.n_following(user_ids[i]); j++) {
                    Entity& p1 = network[network.follow_i(user_ids[i],j)];
                    output1 << "<node id=\"" << network.follow_i(user_ids[i],j) << "\" label=\"" << p1.entity << " - followed"<< "\" />\n";
                }
        }
        output1 << "</nodes>\n" << "<edges>\n";
        int count = 0;
        for (int i = 0; i < fraction_users; i++) {
                for (int j = 0; j < network.n_following(user_ids[i]); j++) {
                        output1 << "<edge id=\"" << count << "\" source=\"" << user_ids[i]
                                        << "\" target=\"" << network.follow_i(user_ids[i], j) << "\"/>\n";
                        count ++;
                }
        }
        output1 << "</edges>\n" << "</graph>\n" << "</gexf>";
        output1.close();
    }

    ofstream output;
    output.open("network.dat");
    for (int i = 0; i < n_entities; i ++) {
        for (int j = 0; j < network.n_followers(i); j ++) {
            output << i << "\t" << network.following_i(i,j) << "\n";
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
        if (network.n_following(i) > max_following) {
            max_following = network.n_following(i);
        }
        if (network.n_followers(i) > max_followers) {
            max_followers = network.n_followers(i);
        }
    }

    int max_degree = max_following + max_followers;

    // now that we have our boundaries lets plot our degree distributions

    // open some files
    ofstream outdd, indd, cumuldd;
    outdd.open("out-degree_distribution.dat");
    indd.open("in-degree_distribution.dat");
    cumuldd.open("cumulative_distribution.dat");

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
    output.open("Categories_Distro.dat");
    category_print(output, "Tweeting", tweeting);
    category_print(output, "Following", following);
    category_print(output, "Retweeting", retweeting);
    output.close();
}

// produces an output file with entity type statistics, makes sure things are working properly
void entity_statistics(Network& network, int n_follows, int n_entities, int max_entities, EntityType* entitytype) {
    ofstream output;
    output.open("entity_percentages.dat");
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
            if (p.entity == j) {
                entity_counts[j] ++;
                average_followers_from_network[j] += p.follower_set.size;
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
            average_followers_from_lists[i] += p.follower_set.size;
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
    tweet_output.open("tweets_distro.dat");
    retweet_output.open("retweets_distro.dat");

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
        cout << "\nNumber of follows for entity type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        return false;
    } else if (j == 2 && abs(correct_val - ets[i].n_tweets) / correct_val >= tolerence) {
        cout << "\nNumber of tweets for entity type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        return false;
    } else if (j == 3 && abs(correct_val - ets[i].n_retweets) / correct_val >= tolerence) {
        cout << "\nNumber of retweets for entity type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        return false;
    }
    return true;
}

bool entity_checks(EntityTypeVector& ets, Network& network, AnalysisState& state, double& rate_add, int& initial_entities) {
    double tolerence = 0.05;
    int final_check = 0;
    int check_count = 0;
    for (int i = 0; i < ets.size(); i ++) {
        double add_correct = network.n_entities * ets[i].prob_add;
        if (abs(add_correct - ets[i].entity_list.size()) / add_correct >= tolerence) {
            cout << "\nNumber of entity type \'" << ets[i].name << "\' is not correct. " << add_correct << " is the right number.\n";
            final_check += false;
            check_count ++;
        }
        for (int j = 1; j < ets[i].number_of_events; j ++) {
            if (rate_add == 0 && ets[i].RF[j].function_type == "constant") {
                double correct_val = ets[i].RF[j].const_val * state.time * network.n_entities * ets[i].prob_add;
                final_check += quick_rate_check(ets, correct_val, i, j);
                check_count ++;
            } else if (rate_add == 0 && ets[i].RF[j].function_type == "linear" ) {
                double correct_val = (ets[i].RF[j].y_intercept + ets[i].RF[j].slope * state.n_months()) * state.time * network.n_entities * ets[i].prob_add;
                final_check += quick_rate_check(ets, correct_val, i, j);
                check_count ++;
            } else if (rate_add == 0 && ets[i].RF[j].function_type == "exponential") {
                double correct_val = (ets[i].RF[j].amplitude*exp(ets[i].RF[j].exp_factor*state.n_months())) * state.time * network.n_entities * ets[i].prob_add;
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
            // exponential analytical solution not found yet, checks to come
            /*else if (rate_add != 0 && ets[i].RF[j].function_type == "exponential") {
                double correct_val = (ets[i].RF[j].amplitude*exp(ets[i].RF[j].exp_factor*state.n_months())) * state.time * network.n_entities * ets[i].prob_add;
                return quick_rate_check(ets, correct_val, i, j);
            }*/
        }
    }
    if (final_check == check_count) {
        return true;
    }
    return false;
}

// function that will plot degree distributions for every entity, and at the top
// of the files gives you info about the percentage of each entity they are following
void whos_following_who(EntityTypeVector& ets, Network& network) {
    for (int i = 0; i < ets.size(); i ++ ) {
        string filename = ets[i].name + "_info.dat";
        ofstream output;
        output.open(filename.c_str());
        int max_degree = 0;
        for (int j = 0; j < ets[i].entity_list.size(); j ++) {
            int degree = network.n_following(ets[i].entity_list[j]) + network.n_followers(ets[i].entity_list[j]);
            if (degree > max_degree) {
                max_degree = degree;
            }
        }        
        vector<int> entity_followers(max_degree), entity_following(max_degree), entity_degree(max_degree);
        for (int j = 0; j < max_degree; j ++) {
            entity_followers[j] = 0;
            entity_following[j] = 0;
            entity_degree[j] = 0;
        }
        vector<int> who_following(ets.size()), who_followers(ets.size());
        for (int i = 0; i < ets.size(); i ++) {
            who_following.at(i) = 0;
            who_followers.at(i) = 0;
        }
        double followers_sum = 0, following_sum = 0;
        for (int j = 0; j < ets[i].entity_list.size(); j ++) {
            int in_degree = network.n_followers(ets[i].entity_list[j]);
            int out_degree = network.n_following(ets[i].entity_list[j]);
            entity_followers[in_degree] ++;
            entity_following[out_degree] ++;
            entity_degree[in_degree + out_degree] ++;
            for (int k = 0; k < in_degree; k ++) {
                Entity& et = network[network.following_i(ets[i].entity_list[j], k)];
                who_followers[et.entity] ++;
                followers_sum ++;
            }
            for (int k = 0; k < out_degree; k ++) {
                Entity& et = network[network.follow_i(ets[i].entity_list[j], k)];
                who_following[et.entity] ++;
                following_sum ++;
            }
        }
        output << "# Entity percentages following entity type \'" << ets[i].name << "\'\n# ";
        for (int j = 0; j < ets.size(); j ++) {
            output << ets[j].name << ": " << who_followers[j] / followers_sum * 100.0 << "   ";
        }
        output << "\n# Entity percentages that entity type \'" << ets[i].name << "\' follows\n# ";
        for (int j = 0; j < ets.size(); j ++) {
            output << ets[j].name << ": " << who_following[j] / following_sum * 100.0 << "   ";
        }
        output << "\n# degree\tin_degree\tout_degree\tcumulative\tlog(degree)\tlog(in_degree)\tlog(out_degree)\tlog(cumulative)\n\n";
        for (int j = 0; j < max_degree; j ++) {
            output << j << "\t" << entity_followers[j] / (double) ets[i].entity_list.size() << "\t" << entity_following[j] / (double) ets[i].entity_list.size() << "\t" << entity_degree[j] / (double) ets[i].entity_list.size() << "\t" << log(j) << "\t" << log(entity_followers[j] / (double) ets[i].entity_list.size()) << "\t" << log(entity_following[j] / (double) ets[i].entity_list.size()) << "\t" << log(entity_degree[j] / (double) ets[i].entity_list.size()) << "\n";
        }
       output.close(); 
    }
}
