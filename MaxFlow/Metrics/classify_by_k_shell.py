# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from Utils import Logger,List_file
import pandas as pd
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
Lf = List_file()

class Classify_by_k_shell(object):
    def __init__(self,contract_id):
        self.contract_id = contract_id
        self.posis = Lf.eachfile("/home/zky/Code/mydata/posi/" + self.contract_id)
        self.posis.sort()
        self.records = Lf.eachfile("/home/zky/Code/mydata/records/" + self.contract_id)
        self.records.sort()
        HPBITSM_clients = pd.read_csv("/home/zky/Code/mydata/posi/"+self.contract_id+"_HPBITSM_list.csv", dtype=str)
        self.H_list = HPBITSM_clients['H'].dropna().tolist()
        self.P_list = HPBITSM_clients['P'].dropna().tolist()
        self.B_list = HPBITSM_clients['B'].dropna().tolist()
        self.I_list = HPBITSM_clients['I'].dropna().tolist()
        self.T_list = HPBITSM_clients['T'].dropna().tolist()
        self.S_list = HPBITSM_clients['S'].dropna().tolist()
        self.M_list = HPBITSM_clients['M'].dropna().tolist()
    #计算层之间的流动，  按60% 30% 10%比例分割
    def flow_between_layers(self,k):
        df = pd.read_csv(self.posis[k], encoding='utf-8', header=None, dtype={0:str}, usecols=[0, 4, 8])
        record = pd.read_csv(self.records[k], encoding='utf-8', header=None, dtype={0:str,1:str}, usecols=[0, 1, 2])
        SUM = record[2].sum()
        cut_1 = df[8].max()*0.6
        cut_2 = df[8].max()*0.9
        self.level_1 = df[df[8] <= cut_1][0].tolist()
        self.level_2 = df[(df[8] > cut_1) & (df[8] < cut_2)][0].tolist()
        self.level_3 = df[df[8] >= cut_2][0].tolist()

        M = [("level_1", self.level_1), ("level_2", self.level_2), ("level_3", self.level_3)]
        for i in range(len(M)):
            for j in range(len(M)):
                temp = record[record[0].isin(M[j][1]) & record[1].isin(M[i][1])]
                QTY = temp[2].sum()
                print(M[i][0],"->",M[j][0],"QTY:",round(QTY/SUM,6))

    # 计算层内每类占比
    def Intra_layer_ratio(self,CLASS,name):
        r1 = len(set(CLASS) & set(self.H_list))
        r2 = len(set(CLASS) & set(self.P_list))
        r3 = len(set(CLASS) & set(self.B_list))
        r4 = len(set(CLASS) & set(self.I_list))
        r5 = len(set(CLASS) & set(self.T_list))
        r6 = len(set(CLASS) & set(self.S_list))
        r7 = len(set(CLASS) & set(self.M_list))
        #plt.rcParams['font.sans-serif'] = 'SimHei' #设置中文显示
        plt.figure(figsize=(10,10))
        label = ['H','P','B','I','T','S','M']
        explode = [0.01, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01]
        plt.pie([r1,r2,r3,r4,r5,r6,r7], explode=explode, labels=label, autopct='%1.2f%%') #绘制饼图
        plt.title(name)
        plt.savefig("./Intra_layer_ratio/"+name+'.png')


    def show_ratio(self,k):
        date = self.posis[k][-12:-4]
        self.Intra_layer_ratio(self.level_1, self.contract_id+"/level_1/"+self.contract_id+"_"+date)
        self.Intra_layer_ratio(self.level_2, self.contract_id+"/level_2/"+self.contract_id+"_"+date)
        self.Intra_layer_ratio(self.level_3, self.contract_id+"/level_3/"+self.contract_id+"_"+date)

    def run(self,start,end):
        for k in range(start,end):
            self.flow_between_layers(k)
            self.show_ratio(k)


if __name__ == '__main__':
    Classify_by_k_shell("i1809").run(0,160)
    # Classify_by_k_shell("i1901").run(150,151)
    Classify_by_k_shell("i1905").run(0,220)