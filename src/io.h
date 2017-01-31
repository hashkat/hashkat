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

#ifndef __IO_H_
#define __IO_H_

#include <fstream>

#include "analyzer.h"
#include "network.h"


void output_position(Network& network);
void brief_agent_statistics(AnalysisState& state);
void output_network_statistics(AnalysisState& state);

int factorial(int input_number);
void Categories_Check(CategoryGrouper& tweeting, CategoryGrouper& following, CategoryGrouper& retweeting);
void agent_statistics(Network& network,int n_follows, int n_agents, int max_agents, AgentType* agenttype);
void tweets_distribution(Network& network);
int rand_int(int max);
void degree_distributions(Network& network, AnalysisState& state);
bool quick_rate_check(AgentTypeVector& ets, double& correct_val, int& i, int& j);
bool agent_checks(AgentTypeVector& ets, Network& network, AnalysisState& state, Add_Rates& add_rates, int& initial_agents);
void whos_following_who(AgentTypeVector& ets, Network& network);
void visualize_most_popular_tweet(MostPopularTweet& mpt, Network& network);
void network_statistics(Network& n, NetworkStats& stats, AgentTypeVector& etv);
bool region_stats(Network& n, AnalysisState& state);
void fraction_of_connections_distro(Network& network, AnalysisState& state, NetworkStats& net_stats);
void dd_by_age(Network& n, AnalysisState& as, NetworkStats& ns);
void dd_by_agent(Network& n, AnalysisState& as, NetworkStats& ns);
void dd_by_follow_method(Network& n, AnalysisState& as, NetworkStats& ns);
void most_popular_tweet_content(MostPopularTweet& mpt, Network& network);
void tweet_info(std::vector<Tweet>&);
void n_agents_in_regions(Network& n);
#endif
