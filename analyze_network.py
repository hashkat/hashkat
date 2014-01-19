#!/usr/bin/python

import matplotlib.pyplot as plt
import networkx as nx
from networkx import *
import time
t0 = time.clock()
f = open('network.dat', 'r')

edgelist = []
print "\nReading inputfile..."
#print len(f.readlines())
for line in f.readlines():
    edgelist.append((int(line.split()[0]), int(line.split()[1])))

print "Creating directed and undirected graphs..."
Directed_G = nx.DiGraph(edgelist)
Undirected_G = Directed_G.to_undirected()
print "Number of nodes:\t", Directed_G.number_of_nodes()
print "Number of links in directed graph:\t", Directed_G.number_of_edges()
print "Number of links in undirected graph:\t", Undirected_G.number_of_edges()
#pos=nx.spring_layout(G,k=0.1,scale=5,iterations=50)
#nx.draw(G,pos, node_size=30, alpha=0.5,node_color="blue", with_labels=False, width=0.1)
#plt.show()
print "Computing values...this may take a while\n"
t1 = time.clock()
print "Average clustering coefficient:\t", nx.average_clustering(Undirected_G)
t2 = time.clock()
print '%f seconds elapsed\n' % (t2-t1)
t3 = time.clock()
centrality = nx.eigenvector_centrality_numpy(Directed_G)
t4 = time.clock()
print '%f seconds elapsed for centrality calculation\n' % (t4-t3)
t5 = time.clock()
print "The average shortest path is:\t", nx.average_shortest_path_length(Undirected_G)
t6 = time.clock()
print '%f seconds elapsed for average path length calculation\n' % (t6-t5)


#for g in nx.connected_component_subgraphs(G):
 #   print(nx.average_shortest_path_length(g)) 
#print "Average shortest path:\t", nx.average_shortest_path_length(G)
    
print "\nAll done.\n" 
print '%f seconds elapsed in total\n' % (t6-t0)
