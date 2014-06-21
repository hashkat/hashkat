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


void output_position(Network& network, int end_time);
void brief_entity_statistics(AnalysisState& state);
void output_network_statistics(AnalysisState& state);

int factorial(int input_number);
void Categories_Check(CategoryGrouper& tweeting, CategoryGrouper& following, CategoryGrouper& retweeting);
void entity_statistics(Network& network,int n_follows, int n_entities, int max_entities, EntityType* entitytype);
void tweets_distribution(Network& network, int n_users);
int rand_int(int max);
void degree_distributions(Network& network);
bool quick_rate_check(EntityTypeVector& ets, double& correct_val, int& i, int& j);
bool entity_checks(EntityTypeVector& ets, Network& network, AnalysisState& state, Add_Rates& add_rates, int& initial_entities);
void whos_following_who(EntityTypeVector& ets, Network& network);
void visualize_most_popular_tweet(MostPopularTweet& mpt, Network& network);
void network_statistics(Network& n, NetworkStats& stats, EntityTypeVector& etv);
bool region_stats(Network& n, AnalysisState& state);

#endif
