# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from Utils import Logger, List_file
import networkx as nx
import pandas as pd
import time
import matplotlib.pyplot as plt


#G = nx.random_graphs.barabasi_albert_graph(1000,3)   #生成一个n=1000，m=3的BA无标度网络
graph_files = Lf.eachfile("./data")
graph_files.sort()

def get_status(para):
    res = []
    Lf = List_file()

    for graph_file in graph_files:
        G = nx.DiGraph()
        Graph = pd.read_csv(graph_file, header=None, usecols=[0,1], dtype={0:str,1:str}, sep=" ")
        relations = [tuple(x) for x in Graph[[0,1]].values]
        #List = zip(Graph[0], Graph[1], Graph[2])
        G.add_edges_from(relations)

        if para=="density": # 网络密度
            res.append(nx.density(G))  
        if para=="clustering": #平均聚团系数
            G = G.to_undirected()
            res.append(nx.average_clustering(G)) 
        if para == "diameter": # 网络直径
            try:
                res.append(nx.diameter(G))
            except:
                res.append(-1)
                print("Found infinite path length because the digraph is not strongly connected")
        if para == "average_shortest_path_length": #平均最短路径
            res.append(nx.average_shortest_path_length(G)) 
        if para == "closeness_centrality": #接近度中心性
            closeness_centrality = nx.closeness_centrality(G)
            res.append(sum(closeness_centrality.values())/len(closeness_centrality)) 
        if para == "betweenness_centrality": #节点之间的最短路径中间性
            betweenness_centrality = nx.betweenness_centrality(G) 
            res.append(sum(betweenness_centrality.values())/len(betweenness_centrality))
        if para == "modularity": #模块度
            part = community.best_partition(G)
            modularity = community.modularity(part,G)
            res.append(modularity)

    return res


def show(title, para):
    plt.figure(figsize=(18, 7))
    plt.title(title)

    status = get_status(para)
    index = [i for i in range(1, len(status)+1)]
    #开始画图
    plt.plot(index, status, color='green', label=para)
    plt.legend() #显示图例
    plt.xlabel("Date")
    plt.ylabel("Value")
    plt.savefig("./statistics_result/" + title + ".jpg", dpi=400)
    # plt.show()

show("Density of Full Network", "density")
show("Diameter of Full Network", "diameter")
show("Clusting Coefficient of Full Network", "clustering")
show("Average Shortest Path Length of Full Network", "average_shortest_path_length")
show("Closeness Centrality of Full Network", "closeness_centrality")
show("Betweenness Centrality of Full Network", "betweenness_centrality")