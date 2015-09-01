#!/usr/bin/env python

# Script to visualize network simulations using networkx

import matplotlib.pyplot as plt
import networkx as nx

G = nx.read_edgelist('output/network.dat')

nx.draw(G)
plt.savefig('graph.svg')
plt.show()
