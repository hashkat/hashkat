#!/usr/bin/env python

import filecmp
import os.path

print "The files printed below are those in which have an output different from what was expected. If no files are printed, there was no discrepancies"

if filecmp.cmp('output/Bot_info.dat', 'output/Bot_info.ref', shallow = False) == False:

    print "Bot_info.dat"

if os.path.exists('output/Categories_Distro.dat') == True:
    
    if filecmp.cmp('output/Categories_Distro.dat', 'output/Categories_Distro.ref', shallow = False) == False:    

        print "Categories_Distro.dat"

if filecmp.cmp('output/Celebrity_info.dat', 'output/Celebrity_info.ref', shallow = False) == False:

    print "Celebrity_info.dat"

if filecmp.cmp('output/cumulative-degree_distribution_month_000.dat', 'output/cumulative-degree_distribution_month_000.ref', shallow = False) == False:

    print "cumulative-degree_distribution_month_000.dat"

if filecmp.cmp('output/dd_by_follow_model.dat', 'output/dd_by_follow_model.ref', shallow = False) == False:

    print "dd_by_follow_model.dat"

if filecmp.cmp('output/in-degree_distribution_month_000.dat', 'output/in-degree_distribution_month_000.ref', shallow = False) == False:

    print "in-degree_distribution_month_000.dat"

if filecmp.cmp('output/main_stats.dat', 'output/main_stats.ref', shallow = False) == False:

    print "main_stats.dat"

if filecmp.cmp('output/model_match.dat', 'output/model_match.ref', shallow = False) == False:

    print "model_match.dat"

if filecmp.cmp('output/network.dat', 'output/network.ref', shallow = False) == False:

    print "network.dat"

if filecmp.cmp('output/Organization_info.dat', 'output/Organization_info.ref', shallow = False) == False:

    print "Organization_info.dat"

if filecmp.cmp('output/out-degree_distribution_month_000.dat', 'output/out-degree_distribution_month_000.ref', shallow = False) == False:

    print "out-degree_distribution_month_000.dat"

if filecmp.cmp('output/region_connection_matrix_month_000.dat', 'output/region_connection_matrix_month_000.ref', shallow = False) == False:

    print "region_connection_matrix_month_000.dat"

if filecmp.cmp('output/retweets_distro.dat', 'output/retweets_distro.ref', shallow = False) == False:

    print "retweets_distro.dat"

if filecmp.cmp('output/Standard_info.dat', 'output/Standard_info.ref', shallow = False) == False:

    print "Standard_info.dat"

if filecmp.cmp('output/tweets_distro.dat', 'output/tweets_distro.ref', shallow = False) == False:

    print "tweets_distro.dat"

