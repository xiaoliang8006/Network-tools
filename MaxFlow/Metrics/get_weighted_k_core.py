# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from Utils import Logger,List_file
import pandas as pd
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
Lf = List_file()


# 计算带权重的交易度
class Get_weighted_k_core(object):
    def __init__(self,contract_id):
        self.contract = contract_id
        self.records = Lf.eachfile("/home/zky/Code/mydata/records/" + self.contract)
        self.records.sort()
        self.posis = Lf.eachfile("/home/zky/Code/mydata/posi/" + self.contract)
        self.posis.sort()
        self.data = pd.DataFrame()
        GCTHPO_clients = pd.read_csv("/home/zky/Code/mydata/posi/"+self.contract+"_GCTHPO_list.csv", header=None, dtype=str)
        self.G_list = GCTHPO_clients[0].dropna().tolist()
        self.C_list = GCTHPO_clients[1].dropna().tolist()
        self.T_list = GCTHPO_clients[2].dropna().tolist()
        self.H_list = GCTHPO_clients[3].dropna().tolist()
        self.P_list = GCTHPO_clients[4].dropna().tolist()
        self.O_list = GCTHPO_clients[5].dropna().tolist()
    def get_data(self,i):
        print("\n")
        print(self.records[i])
        self.data = pd.read_csv(self.records[i], header=None, usecols=[0,1,2,3], dtype={0:str, 1:str})
        self.posi = pd.read_csv(self.posis[i], header=None, dtype={0:str})
        self.posi[8] = 1  # weighted degree

    def calculate_degree(self,df):
        df[4] = 1
        data = pd.concat([df.loc[:, [0, 2, 3, 4]], df.loc[:, [1, 2, 3, 4]].rename(columns={1: 0})])
        result = data.groupby(0).sum()
        result.reset_index(drop=False, inplace=True)
        tmp = np.sqrt((np.log2(result[2])+1) * result[4])
        result['k_'] = np.rint(tmp)
        #print(result)
        result = pd.DataFrame({'clients':result[0].tolist(),'degrees':result['k_'].tolist()})
        return result

    def update_graph(self,nodes):
        self.data = self.data[(self.data[0].map(lambda x:x not in nodes)) & (self.data[1].map(lambda x:x not in nodes))]

    def run(self,i):
        self.get_data(i)
        k_core = 1
        while len(self.data):
            Len = len(self.data)
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
            print('k_core:', k_core, " 客户数量:", len(k_core_nodes), " 交易记录数量:", Len-len(self.data))
            self.posi.loc[self.posi[0].isin(k_core_nodes), 8] = k_core
            k_core += 1
        # file_name = "/home/zky/Code/mydata/posi/" + self.contract + "/posi_" + self.records[i].split("_")[-1]
        #self.posi.to_csv(file_name, mode='w+', header=None, index=None)


if __name__ == '__main__':
    t1 = Get_weighted_k_core("i1809")
    #for i in range(0,160):
    t1.run(69)




