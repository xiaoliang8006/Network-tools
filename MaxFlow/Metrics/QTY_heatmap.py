# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from Utils import Logger,List_file
import networkx as nx
import pandas as pd
import numpy as np
import time
import seaborn as sns
import matplotlib.pyplot as plt
from get_HPBITSM_clientNum_profit import Process_profit
Lf = List_file()

class Process_client_info(object):
    def __init__(self, CONTRACT_ID):
        self.contract_id = CONTRACT_ID
        self.records = Lf.eachfile("/home/zky/Code/mydata/records/" + self.contract_id)
        HPBITSM_clients = pd.read_csv("/home/zky/Code/mydata/posi/"+self.contract_id+"_HPBITSM_list.csv", dtype=str)
        self.records.sort()
        self.pp = Process_profit(self.contract_id)

        self.H_list = HPBITSM_clients['H'].dropna().tolist()
        self.P_list = HPBITSM_clients['P'].dropna().tolist()
        self.B_list = HPBITSM_clients['B'].dropna().tolist()
        self.I_list = HPBITSM_clients['I'].dropna().tolist()
        self.T_list = HPBITSM_clients['T'].dropna().tolist()
        self.S_list = HPBITSM_clients['S'].dropna().tolist()
        self.M_list = HPBITSM_clients['M'].dropna().tolist()

    def get_flow(self,k):
        M = [("H", self.H_list), ("P", self.P_list), ("B",self.B_list), ("I",self.I_list), ("T",self.T_list), ("S", self.S_list), ("M",self.M_list)]
        self.graph = np.zeros((7,7), dtype=np.int)
        data = pd.read_csv(self.records[k], header=None, dtype={0: str,1:str})
        for i in range(len(M)):
            for j in range(len(M)):
                temp = data[data[0].isin(M[j][1]) & data[1].isin(M[i][1])]
                QTY = temp[2].sum()
                #print(M[i][0],"->",M[j][0],"QTY:",QTY,"trade_times:",trade_times)
                self.graph[i][j] = QTY

    def run(self):
        for k in range(len(self.records)):
            print(k)
            date = self.records[k][-12:-4]
            self.get_flow(k)
            self.graph = self.graph/self.graph.sum()
            plt.figure(figsize=(18,10))
            plt.title(self.contract_id+" QTY heatmap   date:"+ date)
            sns.set()
            cmap = sns.cm.rocket_r
            sns.heatmap(self.graph, annot=True, fmt="f",cmap=cmap)
            plt.xticks([0.5,1.5,2.5,3.5,4.5,5.5,6.5],['H','P','B','I','T','S','M'])
            plt.yticks([0.5,1.5,2.5,3.5,4.5,5.5,6.5],['H','P','B','I','T','S','M'])
            plt.xlabel("buyer")
            plt.ylabel("seller")
            plt.savefig("./QTY_heatmap/"+self.contract_id+"/"+self.contract_id+" QTY heatmap-"+ date +".jpg",dpi=200)
            #plt.show()

if __name__ == '__main__':
    Process_client_info("i1809").run()
    #Process_client_info("i1901").run()


