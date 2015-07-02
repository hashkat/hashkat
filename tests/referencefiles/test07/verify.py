#!/usr/bin/env python

import filecmp

print "If output is True, files are the same, if False, files are different."

print "Bot_info - " + str(filecmp.cmp('output/Bot_info.dat', 'output/Bot_info.ref', shallow = False))

print "Celebrity_info - " + str(filecmp.cmp('output/Celebrity_info.dat', 'output/Celebrity_info.ref', shallow = False))

print "cumulative-degree_distribution_month_000.dat - " + str(filecmp.cmp('output/cumulative-degree_distribution_month_000.dat', 'output/cumulative-degree_distribution_month_000.ref', shallow = False))

print "dd_by_follow_model - " + str(filecmp.cmp('output/dd_by_follow_model.dat', 'output/dd_by_follow_model.ref', shallow = False))

print "in-degree_distribution_month_000.dat - " + str(filecmp.cmp('output/in-degree_distribution_month_000.dat', 'output/in-degree_distribution_month_000.ref', shallow = False))

print "main_stats - " + str(filecmp.cmp('output/main_stats.dat', 'output/main_stats.ref', shallow = False))

print "model_match - " + str(filecmp.cmp('output/model_match.dat', 'output/model_match.ref', shallow = False))

print "network - " + str(filecmp.cmp('output/network.dat', 'output/network.ref', shallow = False))

print "Organization_info - " + str(filecmp.cmp('output/Organization_info.dat', 'output/Organization_info.ref', shallow = False))

print "out-degree_distribution_month_000 - " + str(filecmp.cmp('output/out-degree_distribution_month_000.dat', 'output/out-degree_distribution_month_000.ref', shallow = False))

print "region_connection_matrix_month_000 - " + str(filecmp.cmp('output/region_connection_matrix_month_000.dat', 'output/region_connection_matrix_month_000.ref', shallow = False))

print "retweets_distro - " + str(filecmp.cmp('output/retweets_distro.dat', 'output/retweets_distro.ref', shallow = False))

print "Standard_info - " + str(filecmp.cmp('output/Standard_info.dat', 'output/Standard_info.ref', shallow = False))

print "tweets_distro - " + str(filecmp.cmp('output/tweets_distro.dat', 'output/tweets_distro.ref', shallow = False))

