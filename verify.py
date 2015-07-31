#!/usr/bin/env python

'''
This script checks to see whether the current build of hashkat matches the reference data in referencefiles/testXX/output
'''

import filecmp
import os.path

listoffiles = ['Bot_info', 'Categories_Distro', 'Celebrity_info', 'cumulative-degree_distribution_month_000', 'dd_by_follow_model', 'in-degree_distribution_month_000', 'main_stats', 'model_match', 'network', 'Organization_info', 'out-degree_distribution_month_000', 'region_connection_matrix_month_000', 'retweets_distro', 'Standard_info', 'tweets_distro'] 

no_differences_detected = True

for file in listoffiles:

    if os.path.exists(file + '.dat') == True:

        if filecmp.cmp( file + '.dat', file + '.ref', shallow = False) == False:
            no_differences_detect = False
            print 'Test failed, difference between test and reference data'
            print file

if (no_differences_detected == True):
    print 'All tests passed'
