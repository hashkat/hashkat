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

#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <cstdio>
#include "dependencies/mtwist.h"
#include "analyzer.h"
#include "io.h"
#include "util.h"
#include "tweets.h"
#include "config_static.h"

#include "util/StatCalc.h"

using namespace std;


// ROOT OUTPUT ROUTINE
/* After 'analyze', print the results of the computations. */
void output_network_statistics(AnalysisState& state) {
    // uncomment to print performance statistics
    //perf_print_results();

    Network& network = state.network;
    ParsedConfig& C = state.config;
    AgentTypeVector& et_vec = state.agent_types;
    NetworkStats& stats = state.stats;
    TweetBank& tb = state.tweet_bank;
    MostPopularTweet& mpt = state.most_pop_tweet;

    //brief_agent_statistics(state);

    // Print why program stopped
    if (C.output_stdout_basic) {
        if (SIGNAL_ATTEMPTS > 0) {
            cout << "\nSimulation (Gracefully) Interrupted: ctrl-c was pressed\n";
        } else if (state.end_time >= C.max_sim_time) {
            cout << "\nSimulation Completed: desired duration reached\n";
//        } else if (network.n_agents >= C.max_agents) { // AD: No longer an exit condition
//            cout << "\nSimulation Completed: desired agent amount reached\n";
        } else if (!analyzer_real_time_check(state)) {
            cout << "\nSimulation Completed: desired wall-clock time reached\n";
        } else if (!analyzer_sim_time_check(state)) {
            cout << "\nSimulation Completed: desired simulation time reached\n";
        } else if (stats.n_steps >= C.max_analysis_steps) {
            cout << "\nSimulation Completed: desired analysis steps reached\n";
        } else if (stats.user_did_exit) {
            cout << "\nSimulation Completed: user demands exit!\n";
        } else {
            cout << "\nSimulation Completed: stagnant network (ie, no agent has anything to do) \n";
        }
        cout << "\nCreating analysis files -- press ctrl-c multiple times to abort ... \n";
    }
    int MAX_AGENTS = C.max_agents;
    double rate_add = C.rate_add;
    int N_AGENTS = network.n_agents;
    int initial_agents = C.initial_agents;

    // Depending on our INFILE/configuration, we may output various analysis
    if (C.output_visualize) {
        output_position(network, N_AGENTS);
    }
    /* ADD FUNCTIONS THAT RUN AFTER NETWORK IS BUILT HERE */
    if (C.use_barabasi) {
        Categories_Check(state.tweet_ranks, state.follow_ranks, state.retweet_ranks);
    }
    if (C.output_tweet_analysis) {
        tweets_distribution(network, N_AGENTS);
    }
    // Better to manually check distributions, for low network sizes this will most likely throw an error
    /*if (agent_checks(et_vec, network, state, state.config.add_rates, initial_agents)) {
        //cout << "Number of events are valid.\n";
    } else {
        cout << "Numbers are events are not valid, adjust the tolerance or check for errors.\n";
    }*/
    if (C.agent_stats) {
        whos_following_who(et_vec, network);
    }
    if (C.output_stdout_basic) {
        cout << "Analysis complete!\n";
    }
    if (C.degree_distributions) {
        degree_distributions(network, state);
    }
    if (C.retweet_viz) {
        visualize_most_popular_tweet(mpt, network);
    }
    if (C.main_stats) {
        network_statistics(network, stats, et_vec);
    }
    if (C.region_connection_matrix) {
        region_stats(network, state);
    }
    //fraction_of_connections_distro(network, state, stats);
    //if (C.dd_by_age) {
    //    dd_by_age(network, state, stats);
    //}

    //#### This function has already been implemented, it is in agent stats
    //if (C.dd_by_agent) {
    //   dd_by_agent(network, state, stats);
    //}
    if (C.dd_by_follow_model) {
        dd_by_follow_method(network, state, stats);
    }   
}

static void output_stat_calc(const char* name, StatCalc& calc) {
    cout << "Average " << name << ": " << calc.average << " (+-" << calc.standard_deviation() << ")" << endl;
}

void brief_agent_statistics(AnalysisState& state) {
    Network& network = state.network;
    ParsedConfig& config = state.config;

    for (int i = 0; i < min(/*For now:*/2, network.n_agents); i++) {
       Agent& e = network[i];
       AgentType& et = state.agent_types[e.agent_type];

       printf("------------------------------------------------------------------------\n");
       printf("(Agent %d)\n", i);
       printf("(AgentType %s)\n", et.name.c_str());
       printf("------------------------------------------------------------------------\n");
       e.follower_set.print();
    }

    DiscreteDist langs, ideos, regions, prefs;
    StatCalc follows, followers, tweets, retweets;
    for (Agent& e : network) {
        // Discrete options:
        langs.add_element(e.language);
        ideos.add_element(e.ideology_bin);
        regions.add_element(e.region_bin);
        prefs.add_element(e.preference_class);

        // General statistics:
        follows.add_element(e.follower_set.size());
        followers.add_element(e.following_set.size());
        tweets.add_element(e.n_tweets);
        retweets.add_element(e.n_retweets);
    }

    cout << "Language statistics:" << endl;
    for (int i = 0; i < N_LANGS; i++) {
        langs.print_bin(language_name(i), i);
    }

    cout << "Ideology statistics:" << endl;
    for (int i = 0; i < config.ideologies.size(); i++) {
        ideos.print_bin(config.ideologies[i].name, i);
    }

    cout << "Agent stats distributions:" << endl;
    output_stat_calc("Follows", follows);
    output_stat_calc("Followers", followers);
    output_stat_calc("Tweets", tweets);
    output_stat_calc("Retweets", retweets);

    cout << "Final tweet stats:" << endl;

    int n_humourous = 0, n_musical = 0, n_ideological = 0, n_plain = 0, n_total = 0;
    for (Tweet tweet : state.tweet_bank.as_vector()) {
        TweetType type = tweet.content->type;
        n_total++;
        if (type == TWEET_STANDARD) {
            n_plain++;
        } else if (type == TWEET_IDEOLOGICAL) {
            n_ideological++;
        } else if (type == TWEET_MUSICAL) {
            n_musical++;
        } else if (type == TWEET_HUMOUROUS) {
            n_humourous++;
        } else {
            ASSERT(false, "LOGIC ERROR!");
        }
    }
    cout << "n_humourous: " << n_humourous << endl
        << "n_musical: " << n_musical << endl
        << "n_ideological: " << n_ideological << endl
        << "n_plain: " << n_plain << endl
        << "n_total: " << n_total << endl;
}

// edgelist created for R (analysis) and python executable (drawing) and gephi outputfile
void output_position(Network& network, int n_agents) {
    ofstream output1;
    output1.open("output/network.gexf");
    output1 << "<gexf version=\"1.2\">\n"
            << "<meta lastmodifieddate=\"2013-11-21\">\n"
            << "<creator> Kevin Ryczko </creator>\n"
            << "<description> Social Network Visualization </description>\n"
            << "</meta>\n"
            << "<graph mode=\"static\" defaultedgetype=\"directed\">\n"
            << "<nodes>\n";
    int count = 0;
    if (n_agents <= 10000) {
        for (int i = 0; i < n_agents; i++) {
                Agent& p = network[i];
                output1 << "<node id=\"" << i << "\" label=\"" << p.agent_type << "\" />\n";
        }
        output1 << "</nodes>\n" << "<edges>\n";
        for (int id = 0; id < n_agents; id++) {
            for (int id_fol : network.following_set(id).as_vector()) {
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
            id = rng.rand_int(n_agents);
        }

        for (int& id : user_ids) {
                Agent& p = network[id];
                output1 << "<node id=\"" << id << "\" label=\"" << p.agent_type << "\" />\n";

                for (int id_fol : network.following_set(id).as_vector()) {
                    Agent& p1 = network[id_fol];
                    output1 << "<node id=\"" << id_fol << "\" label=\"" << p1.agent_type << " - followed"<< "\" />\n";
                }
        }
        output1 << "</nodes>\n" << "<edges>\n";
        int count = 0;
        for (int& id : user_ids) {
            for (int id_fol : network.following_set(id).as_vector()) {
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
    for (int id = 0; id < n_agents; id++) {
        for (int id_fol : network.follower_set(id).as_vector()) {
            output << id << "\t" << id_fol << "\n";
        }
    }
    output.close();
} 


void model_match(Network& network, vector<int> & counts, int max_degree) {
    int sum_k = 0;
    for (int i = 0; i < network.n_agents; i ++) {
       sum_k += network.n_followers(i);
       sum_k += network.n_followings(i); 
    }
    double average_degree = (double) sum_k / (double) network.n_agents;
    double sum_jN = 0;
    for (int j = 0; j < counts.size(); j++) {
        sum_jN += j*counts[j];
    }
    vector<double> q(max_degree - 1);
    for(int i = 0; i < q.size(); i++) {
        q[i] = (double) (i+1) * counts[i+1] / (double) sum_jN;
    }
    double H = 0;
    for (auto& qval : q) {
        if (qval) {
            H += qval * log(qval);
        }
    }
    H *= -1;
    ofstream output;
    output.open("output/model_match.dat");
    output << "This file is generated to calculate parameters for a general P(k) distribution.\n\n";
    output << "<k> = " << average_degree << "\n";
    output << "H = " << H << "\n";
}
/* This is a function that gives you the probability of a agent following a
 a given number of people. If you want to use this function, simply put
 P_OUT = 1 in the INFILE. This function will generate an output file that
 can be plotted using gnuplot. */

void degree_distributions(Network& network,AnalysisState& state) {
    int max_following = 0, max_followers = 0;
    for (int i = 0; i < network.n_agents; i ++) {
        if (network.n_followings(i) >= max_following) {
            max_following = network.n_followings(i) + 1;
        }
        if (network.n_followers(i) >= max_followers) {
            max_followers = network.n_followers(i) + 1;
        }
    }

    int max_degree = max_following + max_followers;

    // now that we have our boundaries lets plot our degree distributions

    // open some files
    ofstream outdd, indd, cumuldd;//, scaled;
    char out[100], in[100], cumul[100], scale[100];
    sprintf(out, "output/out-degree_distribution_month_%03d.dat", state.n_months());
    sprintf(in, "output/in-degree_distribution_month_%03d.dat", state.n_months());
    sprintf(cumul, "output/cumulative-degree_distribution_month_%03d.dat", state.n_months());
    //sprintf(scale, "output/scaled-degree_distribution_month_%03d.dat", state.n_months());
    
    string out_s = out;
    string in_s = in;
    string cumul_s = cumul;
    //string scale_s = scale;

    outdd.open(out_s.c_str());
    indd.open(in_s.c_str());
    cumuldd.open(cumul_s.c_str());
    //scaled.open(scale_s.c_str());

    outdd << "# This is the out-degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";
    indd << "# This is the in-degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";
    cumuldd << "# This is the cumulative degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";
    //scaled << "# This is the scaled degree distribution. The data order is:\n# degree, normalized probability, log of degree, log of normalized probability\n\n";

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
    for (int i = 0; i < network.n_agents; i ++) {
        out_degree_distro[network.n_followings(i)] ++;
        in_degree_distro[network.n_followers(i)] ++;
        cumulative_distro[network.n_followers(i) + network.n_followings(i)] ++;
    }
    
    double max = 0;
    for (auto& count : cumulative_distro) {
        if (count > max) {
            max = count;
        }
    }
    
    model_match(network, cumulative_distro, max_degree);
    // output the distributions
    for (int i = 0; i < max_following; i ++) {
        outdd << i << "\t" << out_degree_distro[i] / (double) network.n_agents << "\t" << log(i) << "\t" << log(out_degree_distro[i] / (double)network.n_agents) << "\n";
    }
    for (int i = 0; i < max_followers; i ++) {
        indd << i << "\t" << in_degree_distro[i] / (double) network.n_agents << "\t" << log(i) << "\t" << log(in_degree_distro[i] / (double)network.n_agents) << "\n";
    }
    for (int i = 0; i < max_degree; i ++) {
        cumuldd << i << "\t" << cumulative_distro[i] / (double) network.n_agents << "\t" << log(i) << "\t" << log(cumulative_distro[i] / (double)network.n_agents) << "\n";
    }
    //for (int i = 0; i < max_degree; i ++) {
     //   scaled << i / (double) max_degree << "\t" << cumulative_distro[i] / (double) max << "\t" << log(i / (double) max_degree) << "\t" << log(cumulative_distro[i] / (double) max) << "\n";
    //}
    
    outdd.close();
    indd.close();
    cumuldd.close();
    //scaled.close();
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
        CategoryAgentList& c = group.categories[i];
        output << c.agents.size() << " at " << c.threshold << "|\t";
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

// produces an output file with agent type statistics, makes sure things are working properly
void agent_statistics(Network& network, int n_follows, int n_agents, int max_agents, AgentType* agenttype) {
    ofstream output;
    output.open("output/agent_percentages.dat");
    vector<int> agent_counts(max_agents);
    vector<int> average_followers_from_network(max_agents);
    vector<int> average_followers_from_lists(max_agents);
    for (int i = 0 ; i < max_agents; i ++) {
        agent_counts[i] = 0;
        average_followers_from_network[i] = 0;
        average_followers_from_lists[i] = 0;
    }
    for (int i = 0 ; i < n_agents; i ++) {
        Agent& p = network[i];
        for (int j = 0; j < max_agents; j ++) {
            if (p.agent_type == j) {
                agent_counts[j] ++;
                average_followers_from_network[j] += p.follower_set.size();
            }
        }
    }
    output << "\n ****** Info regarding numbers of agent types - BASED ON NETWORK ARRAY ****** \n\n";
    for (int i = 0; i < max_agents; i ++) {
        output << "Number of agent " << i << " : " << agent_counts[i] << "    Fraction: " << agent_counts[i]/double(n_agents) << "\n";
    }
    output << "\n ****** Info regarding numbers of agent types - BASED ON USER_LISTS ****** \n THIS SHOULD BE THE SAME AS ABOVE \n\n";
    for (int i = 0; i < max_agents; i ++) {
        output << "Number of agent " << i << " : " << agenttype[i].agent_list.size() << "    Fraction: " << agenttype[i].agent_list.size()/double(n_agents) << "\n";
    }
    output << "\n\n ****** Info regarding following certain agent types - BASED ON NETWORK ARRAY ****** \n\n";
    double sum = 0;
    for (int i = 0; i < max_agents; i ++) {
        sum += average_followers_from_network[i]/double(agent_counts[i]);
    }
    for (int i = 0; i < max_agents; i ++) {
        output << "Average number of follows for agent "
               << i << ": " << average_followers_from_network[i]/double(agent_counts[i])
               << "     Fraction: " << average_followers_from_network[i]/double(n_follows) << "\n";
    }
    for (int i = 0; i < max_agents; i ++){
        for (int j = 0; j < agenttype[i].agent_list.size(); j++) {
            Agent& p = network[agenttype[i].agent_list[j]];
            average_followers_from_lists[i] += p.follower_set.size();
        }
    }
    output << "\n\n ****** Info regarding following certain agent types - BASED ON USER_LISTS ****** \n      SHOULD BE THE SAME AS ABOVE\n\n";
    double sum1 = 0;
    for (int i = 0; i < max_agents; i ++) {
        sum1 += average_followers_from_lists[i] / double(agenttype[i].agent_list.size());
    }
    for (int i = 0; i < max_agents; i ++) {
        output << "Average number of follows for agent "
               << i << ": " << average_followers_from_lists[i]/double(agenttype[i].agent_list.size())
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
        Agent& e = network[i];
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
        Agent& e = network[i];
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

// group the quick_rate_check and agent_checks together, they're dependent on one another
bool quick_rate_check(AgentType& et, double& correct_val, int j) {
    double tolerence = 0.05;
    if (j == 1 && abs(correct_val - et.stats.n_follows) / correct_val >= tolerence) {
        //cout << "\nNumber of follows for agent type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        //cout << "Was: " << ets[i].n_follows << endl;
        return false;
    } else if (j == 2 && abs(correct_val - et.stats.n_tweets) / correct_val >= tolerence) {
        //cout << "\nNumber of tweets for agent type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        //cout << "Was: " << ets[i].n_tweets << endl;
        return false;
    } else if (j == 3 && abs(correct_val - et.stats.n_retweets) / correct_val >= tolerence) {
        //cout << "\nNumber of retweets for agent type \'" << ets[i].name << "\' is not correct. " << correct_val << " is the right number.\n";
        //cout << "Was: " << ets[i].n_retweets << endl;
        return false;
    }
    return true;
}

bool agent_checks(AgentTypeVector& ets, Network& network, AnalysisState& state, Add_Rates& add_rates, int& initial_agents) {
    double tolerence = 0.05;
    int final_check = 0;
    int check_count = 0;
    double rate_add = add_rates.RF.monthly_rates[state.n_months()];
    for (auto& et : ets) {
        double add_correct = network.n_agents * et.prob_add;
        if (abs(add_correct - et.agent_list.size()) / add_correct >= tolerence) {
            //cout << "\nNumber of agent type \'" << ets[i].name << "\' is not correct. " << (int) add_correct << " is the right number.\n";
            //cout << "was: " << ets[i].agent_list.size() << "\n";
            final_check += false;
            check_count ++;
        }
        for (int j = 0; j < number_of_diff_events; j ++) {
            if (rate_add == 0 && et.RF[j].function_type == "constant") {
                double correct_val = et.RF[j].const_val * state.time * network.n_agents * et.prob_add;
                final_check += quick_rate_check(et, correct_val, j);
                check_count ++;
            } else if (rate_add == 0 && et.RF[j].function_type == "linear" ) {
                double correct_val = (et.RF[j].y_intercept + 0.5 * et.RF[j].slope * state.n_months()) * state.time * network.n_agents * et.prob_add;
                final_check += quick_rate_check(et, correct_val, j);
                check_count ++;
            } else if (rate_add != 0 && et.RF[j].function_type == "constant") {
                double correct_val = 0.5 * (et.RF[j].const_val / rate_add) * ((rate_add * rate_add) * et.prob_add * (state.time * state.time) + (rate_add * state.time) - (initial_agents - 1) * initial_agents);
                final_check += quick_rate_check(et, correct_val, j);
                check_count ++;
            } else if (rate_add != 0 && et.RF[j].function_type == "linear" ) {
                double correct_val = (0.5 * et.RF[j].y_intercept * rate_add * et.prob_add * state.time * state.time) + 0.25 * et.RF[j].slope * state.n_months() * state.time + 0.3333 * rate_add * et.RF[j].slope * state.n_months() * et.prob_add * state.time * state.time * 0.5 + 0.5 * et.RF[j].y_intercept * state.time - (initial_agents - 1) * initial_agents;
                final_check += quick_rate_check(et, correct_val, j);
                check_count ++;
                
            } 
        }
    }
    if (final_check == check_count) {
        return true;
    }
    return false;
}

static void whos_following_who(AgentTypeVector& types, AgentType& type, Network& network) {
    string filename = "output/" + type.name + "_info.dat";
    ofstream output;
    output.open(filename.c_str());
    int max_degree = 0;
    for (int i = 0; i < type.agent_list.size(); i ++) {
        int degree = network.n_followings(type.agent_list[i]) + network.n_followers(type.agent_list[i]);
        if (degree > max_degree) {
            max_degree = degree;
        }
    }

    /* Declare 0-filled counting-vectors. */
    vector<int> agent_followers(max_degree + 1 /* AD: Needed one past max stored*/, 0);
    vector<int> agent_following(max_degree + 1, 0);
    vector<int> agent_degree(max_degree + 1, 0);
    vector<int> who_following(types.size(), 0);
    vector<int> who_followers(types.size(), 0);

    double followers_sum = 0, following_sum = 0;
    for (int i = 0; i < type.agent_list.size(); i ++) {
        int id = type.agent_list[i];
        int in_degree = network.n_followers(id);
        int out_degree = network.n_followings(id);
        agent_followers[in_degree] ++;
        agent_following[out_degree] ++;
        agent_degree[in_degree + out_degree] ++;

        // Analyze ins == followers
        for (int id_fol : network.follower_set(id).as_vector()) {
            Agent& et = network[id_fol];
            who_following[et.agent_type] ++;
            following_sum ++;
        }

        // Analyze outs == follows
        for (int id_fol : network.following_set(id).as_vector()) {
            Agent& et = network[id_fol];
            who_followers[et.agent_type] ++;
            followers_sum ++;
        }
    }
    output << "# Agent percentages following agent type \'" << type.name << "\'\n# ";
    for (int i = 0; i < types.size(); i ++) {
        output << types[i].name << ": " << who_following[i] / following_sum * 100.0 << "   ";
    }
    output << "\n# Agent percentages that agent type \'" << type.name << "\' follows\n# ";
    for (int i = 0; i < types.size(); i ++) {
        output << types[i].name << ": " << who_followers[i] / followers_sum * 100.0 << "   ";
    }
    output << "\n# degree\tin_degree\tout_degree\tcumulative\tlog(degree)\tlog(in_degree)\tlog(out_degree)\tlog(cumulative)\n\n";
    for (int i = 0; i < max_degree; i++) {
        output << i << "\t"
                << agent_followers[i] / (double) type.agent_list.size() << "\t"
                << agent_following[i] / (double) type.agent_list.size() << "\t"
                << agent_degree[i] / (double) type.agent_list.size()
                << "\t" << log(i) << "\t"
                << log(agent_followers[i] / (double) type.agent_list.size()) << "\t"
                << log(agent_following[i] / (double) type.agent_list.size()) << "\t"
                << log(agent_degree[i] / (double) type.agent_list.size()) << "\n";
    }
    output.close();
}

// function that will plot degree distributions for every agent, and at the top
// of the files gives you info about the percentage of each agent they are following
void whos_following_who(AgentTypeVector& types, Network& network) {
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
            
    UsedAgents& used_set = t.content->used_agents;
    for (int id_used : used_set.as_vector()) {
        output << "<node id=\"" << id_used << "\" label=\"" << "Retweeters" << "\">\n";
        output << "<viz:size value=\"2.5\"/>\n";
        output << "</node>\n";
        for (int id_fol : network.follower_set(id_used).as_vector()) {
            Agent& e = network[id_fol];
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

    for (int id : network.follower_set(t.id_tweeter).as_vector()) {
        output << "<edge id=\"" << count << "\" source=\"" << id
                << "\" target=\"" << t.id_tweeter << "\"/>\n";
        count ++;
    }
    for (int id_used : used_set.as_vector()) {
        for (int id_fol : network.follower_set(id_used).as_vector()) {
            output << "<edge id=\"" << count << "\" source=\"" << id_fol
                    << "\" target=\"" << id_used << "\"/>\n";
            count ++;
        }
    }
    output << "</edges>\n" << "</graph>\n" << "</gexf>";
    output.close();
}

void network_statistics(Network& n, NetworkStats& net_stats, AgentTypeVector& etv) {
    ofstream output;
    output.open("output/main_stats.dat");
    output << "--------------------\n| MAIN NETWORK STATS |\n--------------------\n\n";
    output << "USERS\n_____\n\n";
    output << "Total: " << n.n_agents << "\n";
    for (auto& et : etv) {
        output << et.name << ": " << et.agent_list.size() << "\t(" << 100*et.agent_list.size() / (double) n.n_agents << "% of total agents)\n";
    }

    AgentStats& stats = net_stats.global_stats;

    output << "\nTWEETS\n_____\n\n";
    output << "Total: " << stats.n_tweets << "\n";
    double hashtag_ratio = (stats.n_tweets == 0 ? 0.0 : stats.n_hashtags / (double) stats.n_tweets);
    output << "Hashtags: " << stats.n_hashtags << "\t(" << 100*hashtag_ratio << "% of total tweets)\n";
    for (auto& et : etv) {
        double tweet_ratio = (stats.n_tweets == 0 ? 0.0 : et.stats.n_tweets / (double) stats.n_tweets);
        output << et.name << ": " << et.stats.n_tweets << "\t("
               << 100*tweet_ratio << "% of total tweets)\n";
    }
    output << "\nRETWEETS\n_______\n\n";
    output << "Total: " << stats.n_retweets << "\n";
    for (auto& et : etv) {
        double retweet_ratio = (stats.n_retweets == 0 ? 0.0 : et.stats.n_retweets / (double) stats.n_retweets);
        output << et.name << ": "
                << et.stats.n_retweets
                << "\t(" << 100*retweet_ratio << "% of total retweets)\n";
    }
    double total_follow_calls = stats.n_random_follows + stats.n_preferential_follows + stats.n_agent_follows + stats.n_pref_agent_follows + stats.n_retweet_follows + stats.n_hashtag_follows + stats.n_followback;
    output << "\nFOLLOWS\n_______\n\n";
    output << "Total follows: " << stats.n_follows << "\n";
    output << "Total follow attempts: " << total_follow_calls << "\n";
    output << "Random: " << stats.n_random_follows << "\t(" << 100*stats.n_random_follows / total_follow_calls << "% of total follow attempts)\n";
    output << "Preferential: " << stats.n_preferential_follows << "\t(" << 100*stats.n_preferential_follows / total_follow_calls << "% of total follow attempts)\n";
    output << "Agent: " << stats.n_agent_follows << "\t(" << 100*stats.n_agent_follows / total_follow_calls << "% of total follows attempts)\n";
    output << "Preferential-agent: " << stats.n_pref_agent_follows << "\t(" << 100*stats.n_pref_agent_follows / total_follow_calls << "% of total follow attempts)\n";
    output << "Retweet: " << stats.n_retweet_follows << "\t(" << 100*stats.n_retweet_follows / total_follow_calls << "% of total follows attempts)\n";
    output << "Hashtag: " << stats.n_hashtag_follows << "\t(" << 100*stats.n_hashtag_follows / total_follow_calls << "% of total follows attempts)\n";
    output << "Followbacks: " << stats.n_followback << "\t (" << 100*stats.n_followback / total_follow_calls << "% of total follow attempts)\n";
    for (auto& et : etv) {
        output << et.name << ": "
                << et.stats.n_follows << "\t("
                << 100*et.stats.n_follows / (double) stats.n_follows << "% of total follows)\n";
    }
    output.close();
}

struct holder {
    vector<int> ids;
};

bool region_stats(Network& n, AnalysisState& state) {
    holder region_self[N_BIN_REGIONS];
    int connections[N_BIN_REGIONS][N_BIN_REGIONS] = {};
    ofstream output;
    char out[100];
    sprintf(out, "output/region_connection_matrix_month_%03d.dat", state.n_months());
    output.open(((string) out).c_str());
    
    for (int i = 0; i < n.n_agents; i ++) {
        Agent& e = n[i];
        int reg = e.region_bin;
        region_self[reg].ids.push_back(i);
        vector<int> following = e.following_set.as_vector();
        for (int j = 0; j < following.size(); j ++) {
            Agent& followee = n[following[j]];
            connections[reg][followee.region_bin] ++;
        }
        
    }
    double follow_counts[N_BIN_REGIONS] = {};
    for (int i = 0; i < N_BIN_REGIONS; i ++) {
        int count = 0;
        for (int j = 0; j < N_BIN_REGIONS; j ++) {
            count += connections[i][j];
        }
        follow_counts[i] = count;
    }
    output << "Connections:\n";
    for (int i = 0; i < N_BIN_REGIONS; i ++) {
        output << "\t" << i;
    }
    output << "\n";
    for (int i = 0; i < N_BIN_REGIONS; i ++) {
        for (int j = 0; j < N_BIN_REGIONS; j ++) {
            if (j == 0) {
                output << "\n" << i;
            }
            double connections_ratio = (follow_counts[i] == 0 ? 0.0 : connections[i][j] / follow_counts[i]);
            output << "\t" << 100.0 * connections_ratio << "\%";
            if (j+1 == N_BIN_REGIONS) {
                output << "\n";
            }
            
        }
    }
    return true;
}

void fraction_of_connections_distro(Network& network, AnalysisState& state, NetworkStats& net_stats) {
    AgentStats& stats = net_stats.global_stats;
    int bin_grid = 1000000; // fraction up to 0.00001
    int agent_counts[bin_grid];
    for (auto& bin : agent_counts) {
        bin = 0;
    }
    int total_follows = stats.n_follows;
    if (!total_follows){
        return;
    }
    for (int i = 0; i < network.n_agents; i ++) {
        // frac_connect_bin -> fcb
        int fcb = (double) network.n_followers(i) / (double) total_follows * (double) bin_grid;
        agent_counts[fcb] ++;
        
    } 
    ofstream output;
    output.open("output/connections_vs_nodes.dat");
    for (int i = 0; i < bin_grid; i ++) {
        output << i / (double) bin_grid << "\t" << (double) agent_counts[i] / (double) network.n_agents << "\t" << log(i / (double) bin_grid) << "\t" << log((double) agent_counts[i] / (double) network.n_agents) <<"\n";
    }
    output.close();  
}

struct Year {
    vector<int> agent_ids;
    vector<double> dd;
    int count = 0;
};

typedef vector<Year> YearVector;

void dd_by_age(Network& n, AnalysisState& as, NetworkStats& ns) {
    
    int year =  12 * 30 * 60 * 24;
    const int length = as.time / year;
    if (!length) {
        return;
    }
    
    YearVector years(length);
    
    int max_following = 0, max_followers = 0;
    for (int i = 0; i < n.n_agents; i ++) {
        
        Agent& e = n[i];
        int age = e.creation_time / year;
        years[age].agent_ids.push_back(e.id);
        
        if (n.n_followings(i) >= max_following) {
            max_following = n.n_followings(i) + 1;
        }
        if (n.n_followers(i) >= max_followers) {
            max_followers = n.n_followers(i) + 1;
        }
    }

    int max_degree = max_following + max_followers;
    for (auto& year : years) {
        year.dd.resize(max_degree);
        for (int i = 0; i < year.dd.size(); i ++) {
            year.dd[i] = 0;
        }
        
    }
    //cout << "AGENT COUNTS CHECK:\n";
    int count = 0;
    for (auto& year : years) {
        for (int i = 0; i < year.agent_ids.size(); i ++) {
            //cout << "YEAR: " << count << "\tCOUNT: " << year.agent_ids.size() << "\n";
                int id = year.agent_ids[i];
                year.dd[n.n_followers(id) + n.n_followings(id)] ++;
                year.count ++;
        }
        count ++;
    }
    ofstream output;
    output.open("output/dd_by_year.dat");

    for (int i = 0; i < max_degree; i ++) {
        output << i << "\t" << log(i);
        for (auto& year : years) {
            output << "\t" << year.dd[i] / n.n_agents << "\t" << log(year.dd[i] / n.n_agents);
        }
        output << "\n";
    }
    output.close();
    
    
}


void dd_by_agent(Network& n, AnalysisState& as, NetworkStats& ns) {
    
    int n_ent_types = as.agent_types.size();
    vector<Year> agent_types(n_ent_types);
    
    int max_following = 0, max_followers = 0;
    for (int i = 0; i < n.n_agents; i ++) {
        
        Agent& e = n[i];
        int ent_type = e.agent_type;
        agent_types[ent_type].agent_ids.push_back(e.id);
        
        if (n.n_followings(i) >= max_following) {
            max_following = n.n_followings(i) + 1;
        }
        if (n.n_followers(i) >= max_followers) {
            max_followers = n.n_followers(i) + 1;
        }
    }
    int max_degree = max_following + max_followers;
    for (auto& ent_type : agent_types) {
        ent_type.dd.resize(max_degree);
        for (int i = 0; i < ent_type.dd.size(); i ++) {
            ent_type.dd[i] = 0;
        }
        
    }
    
    ofstream output;
    output.open("output/dd_by_agent_type.dat");
    
    for (int i = 0; i < agent_types.size(); i ++) {
        output << "# Agent type: " << i << "  Size: " << agent_types[i].agent_ids.size() << "\n";
    }
    for (auto& ent_type : agent_types) {
        for (int i = 0; i < ent_type.agent_ids.size(); i ++) {
            int id = ent_type.agent_ids[i];
            ent_type.dd[n.n_followers(id) + n.n_followings(id)] ++;
        }
    }
    for (int i = 0; i < max_degree; i ++) {
        output << i << "\t" << log(i);
        for (auto& ent_type : agent_types) {
            output << "\t" << ent_type.dd[i] / n.n_agents << "\t" << log(ent_type.dd[i] / n.n_agents);
        }
        output << "\n";
    }
    output.close();
}

void dd_by_follow_method(Network& n, AnalysisState& as, NetworkStats& ns) {
    vector<Year> follow_models(N_FOLLOW_MODELS + 2); // + 2 for retweeting and followback
    int max_following = 0, max_followers = 0;
    for (int i = 0; i < n.n_agents; i ++) {
        if (n.n_followings(i) >= max_following) {
            max_following = n.n_followings(i) + 1;
        }
        if (n.n_followers(i) >= max_followers) {
            max_followers = n.n_followers(i) + 1;
        }
    }
    int max_degree = max_following + max_followers;
    for (auto& follow_model : follow_models) {
        follow_model.dd.resize(max_degree);
        for (int i = 0; i < follow_model.dd.size(); i ++) {
            follow_model.dd[i] = 0;
        }
    }
    for (int i = 0; i < n.n_agents; i ++) {
        for (int j = 0; j < N_FOLLOW_MODELS + 2; j ++) {
            Agent& e = n[i];
            int degree = e.following_method_counts[j] + e.follower_method_counts[j];
            follow_models[j].dd[degree] ++; 
        }
    }
    
    ofstream output;
    output.open("output/dd_by_follow_model.dat");
    
    output << "This is the degree distribution by follow model. The data order is:\n# degree\tlog_of_degree\tRandom-normalized_probability\tRandom-log_of_normalized_probability\t"
    "Preferential-normalized_probability\tPreferential-log_of_normalized_probability\t"
    "Agent-normalized_probability\tAgent-log_of_normalized_probability\tPreferential-agent-normalized_probability\tPreferential-agent-log_of_normalized_probability\t"
    "Hashtag-normalized_probability\tHashtag-log_of_normalized_probability\t"
    "Twitter-normalized_probability\tTwitter-log_of_normalized_probability\tFollowbacks-normalized_probability\tFollowbacks-log_of_normalized_probability\n\n";

    for (int i = 0; i < max_degree; i ++) {
        output << i << "\t" << log(i);
        for (auto& ent_type : follow_models) {
            output << "\t" << ent_type.dd[i] / n.n_agents << "\t" << log(ent_type.dd[i] / n.n_agents);
        }
        output << "\n";
    }
    
    output.close();
}














