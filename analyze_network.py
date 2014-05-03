#!/usr/bin/env python

import matplotlib.pyplot as plt
import networkx as nx
from networkx import *
import time
import sys

t0 = time.clock()


def compute_clustering_coefficient(Directed_G, Undirected_G):
    print "Crunching numbers for clustering...\n"
    t1 = time.clock()
    print "Average clustering coefficient:\t", nx.average_clustering(Undirected_G)
    t2 = time.clock()
    print '%f seconds elapsed\n' % (t2-t1)
    
def average_shortest_path(Directed_G, Undirected_G):
    t1 = time.clock()
    print "Crunching numbers for path lengths...\n"
    path_sum = 0
    num_paths = 0
    no_paths = 0
    counter = 0
    for node0 in Undirected_G:
        counter += 1
        print "%i out of %i nodes done. (%f percent)" % (counter, len(Undirected_G), float(counter) / len(Undirected_G))
        if (num_paths != 0):
            print "Average path length: %f" % (path_sum / float(num_paths))
        for node1 in Undirected_G:
            if (nx.has_path(Undirected_G, node0, node1) and node0 != node1):
                num_paths += 1
                sp = nx.shortest_path_length(Undirected_G, node0, node1)
                path_sum += sp
            else:
                no_paths += 1
    print "Average path length: %f" % (path_sum / float(num_paths))
    print "Number of no paths: %f" % (no_paths)
    t2 = time.clock()
    print "%f seconds elapsed for average path length calculation" % (t2 - t1)

def report_final_stats():
    t6 = time.clock()
    print "\nAll done.\n" 
    print '%f seconds elapsed in total\n' % (t6-t0)

def main():
    file = str(sys.argv[1])
    f = open(file, 'r')

    edgelist = []
    print "\nReading inputfile:", file, "..."
    for line in f.readlines():
        edgelist.append((int(line.split()[0]), int(line.split()[1])))

    print "Creating directed and undirected graphs..."
    Directed_G = nx.DiGraph(edgelist)
    Undirected_G = Directed_G.to_undirected()
    print "Number of nodes:\t", Directed_G.number_of_nodes()
    print "Number of links in directed graph:\t", Directed_G.number_of_edges()
    print "Number of links in undirected graph:\t", Undirected_G.number_of_edges()
    t_final = time.clock()
    print "Time elapsed: %f" % (t_final - t0)
    
    # compute other things
    compute_clustering_coefficient(Directed_G, Undirected_G)
#    average_shortest_path(Directed_G, Undirected_G)
    report_final_stats()
if __name__ == "__main__":
    main()
