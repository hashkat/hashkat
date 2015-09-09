#!/usr/bin/env python

# Script to visualize network simulations using networkx

import matplotlib.pyplot as plt
import networkx as nx
from operator import itemgetter

G = nx.read_edgelist('output/network.dat')
pos = nx.spring_layout(G,iterations=75)

nx.draw(G,pos)
plt.savefig('graph.svg')
plt.show()

print('Number of Nodes: ' + str(nx.number_of_nodes(G)))
print('Number of Edges: ' + str(nx.number_of_edges(G)))
print('Most Popular Agent ID and its Cumulative-Degree: ' + str(sorted(G.degree_iter(),key=itemgetter(1),reverse=True)[0:1]))
print('Most Common Cumulative-Degree and the Number of Agents with that Cumulative-Degree: ' + str(max(enumerate(nx.degree_histogram(G)), key=itemgetter(1))))
print('Graph Density: ' + str(nx.density(G)))
print('Average Shortest Path Length: ' + str(nx.average_shortest_path_length(G)))
