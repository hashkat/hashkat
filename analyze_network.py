#!/usr/bin/python

import matplotlib.pyplot as plt
import networkx as nx
from networkx import *

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
print "Average clustering coefficient:\t", nx.average_clustering(Undirected_G)
#for g in nx.connected_component_subgraphs(G):
 #   print(nx.average_shortest_path_length(g)) 
#print "Average shortest path:\t", nx.average_shortest_path_length(G)
    
print "\n\nAll done.\n" 