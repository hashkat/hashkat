# this is an r script to create a graph from the network.dat file

library(igraph)
mydata <- read.table("network.dat")
my.matrix <- as.matrix(mydata)
my.matrix[,1] <- as.character(my.matrix[,1])
my.matrix[,2] <- as.character(my.matrix[,2])
g <- graph.edgelist(my.matrix)
