#!/usr/bin/python

import matplotlib.pyplot as plt
import networkx as nx
from networkx import *

f = open('network.dat', 'r')

edgelist = []

for line in f.readlines():
    edgelist.append((int(line.split()[0]), int(line.split()[1])))


G = nx.Graph(edgelist)
pos=nx.spring_layout(G,k=0.1,scale=2,iterations=50)
nx.draw(G,pos, node_size=30, alpha=0.5,node_color="blue", with_labels=False, width=0.1)
plt.show()
