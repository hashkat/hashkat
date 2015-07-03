#!/usr/bin/env python

import filecmp
import os.path

print "The files printed below are those in which have an output different from what was expected. If no files are printed, there was no discrepancies"

listoffiles = ['Bot_info', 'Categories_Distro', 'Celebrity_info', 'cumulative-degree_distribution_month_000', 'dd_by_follow_model', 'in-degree_distribution_month_000', 'main_stats', 'model_match', 'network', 'Organization_info', 'out-degree_distribution_month_000', 'region_connection_matrix_month_000', 'retweets_distro', 'Standard_info', 'tweets_distro'] 

for file in listoffiles:

    if os.path.exists(file + '.dat') == True:

        if filecmp.cmp( file + '.dat', file + '.ref', shallow = False) == False:

            print file