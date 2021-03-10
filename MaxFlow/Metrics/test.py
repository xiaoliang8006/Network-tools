# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from Utils import Logger, List_file
import networkx as nx
import pandas as pd
import time
import matplotlib.pyplot as plt


# G = nx.random_graphs.barabasi_albert_graph(1000,3)   #生成一个n=1000，m=3的BA无标度网络
G = nx.random_graphs.erdos_renyi_graph(20000, 0.2)
G = G.to_undirected()
degrees = dict(nx.degree(G))
max_degree = degrees[max(degrees, key=degrees.get)]
print("max_degree:", max_degree)

print("clustering:", nx.average_clustering(G))

print("path_len:", nx.average_shortest_path_length(G))

