# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from Utils import Logger,List_file
import pandas as pd
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
Lf = List_file()

class Get_k_core(object):
    def __init__(self, contract_id):
        self.contract = contract_id
        self.records = Lf.eachfile("/home/zky/Code/mydata/records/" + self.contract)
        self.records.sort()
        self.data = pd.DataFrame()
    def get_data(self, i):
        print("\n")
        print(self.records[i])
        self.data = pd.read_csv(self.records[i], header=None, usecols=[0,1], dtype={0:str, 1:str})

    def calculate_degree(self, df):
        clients = pd.concat([df[0], df[1]])
        clients = pd.DataFrame(clients.value_counts())
        clients.reset_index(drop=False,inplace=True)
        clients = clients['index'].tolist()
        G = nx.Graph()
        relations = [tuple(x) for x in df[[0, 1]].values]
        # List = zip(Graph[0], Graph[1], Graph[2])
        G.add_edges_from(relations)
        G.to_undirected()
        degrees = []
        for client in clients:
            degrees.append(G.degree(client))
        result = pd.DataFrame({'clients':clients, 'degrees':degrees})
        return result

    def update_graph(self,nodes):
        self.data = self.data[(self.data[0].map(lambda x:x not in nodes)) & (self.data[1].map(lambda x:x not in nodes))]

    def run(self, i):
        self.get_data(i)
        k_core = 1
        while len(self.data):
            k_core_nodes = []
            while True:
                degree = self.calculate_degree(self.data)
                #print("客户数量：",len(degree))
                if len(self.data)==0 or degree['degrees'].min() > k_core:
                    break
                #print(degree['degrees'].min(), k_core,len(self.data))
                nodes = degree[degree['degrees']<=k_core]['clients'].tolist()
                k_core_nodes += nodes
                self.update_graph(nodes)
            print('k_core:', k_core, " 客户数量:", len(k_core_nodes), " 交易记录数量:", len(self.data))
            k_core += 1
        return k_core, k_core_nodes

if __name__ == '__main__':
    Get_k_core("i1809").run(64)  # 64  69 74


