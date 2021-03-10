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
import matplotlib.ticker as ticker

Lf = List_file()

class Process_profit(object):
    def __init__(self, CONTRACT_ID):
        self.posis = Lf.eachfile("/home/zky/data/posi")
        self.posis.sort()
        self.contract_id = CONTRACT_ID
    def get_profit(self,k,H,P,B,I,T,S,M):
        df = pd.read_csv(self.posis[k], encoding='utf-8', header=None, dtype={1:str}, usecols=[1, 3, 20, 21])
        # 提取指定contract_id的交易记录
        data = df[df[3] == self.contract_id].loc[:, [1, 20, 21]]
        #print(len(data))
        H_profit = data[data[1].isin(H)][20].sum() + data[data[1].isin(H)][21].sum()
        P_profit = data[data[1].isin(P)][20].sum() + data[data[1].isin(P)][21].sum()
        B_profit = data[data[1].isin(B)][20].sum() + data[data[1].isin(B)][21].sum()
        I_profit = data[data[1].isin(I)][20].sum() + data[data[1].isin(I)][21].sum()
        T_profit = data[data[1].isin(T)][20].sum() + data[data[1].isin(T)][21].sum()
        S_profit = data[data[1].isin(S)][20].sum() + data[data[1].isin(S)][21].sum()
        M_profit = data[data[1].isin(M)][20].sum() + data[data[1].isin(M)][21].sum()
        #print(G_res,C_res,T_res,H_res,P_res,O_res,"总的：",G_res+C_res+T_res+H_res+P_res+O_res,"实际的：",data[20].sum() + data[21].sum())
        return H_profit,P_profit,B_profit,I_profit,T_profit,S_profit,M_profit

class Process_client_info(object):
    def __init__(self, CONTRACT_ID):
        self.contract_id = CONTRACT_ID
        self.posis = Lf.eachfile("/home/zky/data/posi")
        self.posis.sort()
        self.index = []
        self.H_nums,self.P_nums,self.B_nums,self.I_nums,self.T_nums,self.S_nums,self.M_nums = [],[],[],[],[],[],[] #每天客户数量
        self.H_profits,self.P_profits,self.B_profits,self.I_profits,self.T_profits,self.S_profits,self.M_profits = [],[],[],[],[],[],[] #每天客户盈利
        self.pp = Process_profit(self.contract_id)
        HPBITSM_clients = pd.read_csv("/home/zky/Code/mydata/posi/"+self.contract_id+"_HPBITSM_list.csv", dtype=str)
        self.H_list = HPBITSM_clients['H'].dropna().tolist()
        self.P_list = HPBITSM_clients['P'].dropna().tolist()
        self.B_list = HPBITSM_clients['B'].dropna().tolist()
        self.I_list = HPBITSM_clients['I'].dropna().tolist()
        self.T_list = HPBITSM_clients['T'].dropna().tolist()
        self.S_list = HPBITSM_clients['S'].dropna().tolist()
        self.M_list = HPBITSM_clients['M'].dropna().tolist()

    def get_class(self,k):
        print(self.posis[k])
        self.index.append(self.posis[k][-12:-4])
        H_profit,P_profit,B_profit,I_profit,T_profit,S_profit,M_profit = self.pp.get_profit(k,self.H_list,self.P_list,self.B_list,self.I_list,self.T_list,self.S_list,self.M_list)
        self.H_profits.append(H_profit)
        self.P_profits.append(P_profit)
        self.B_profits.append(B_profit)
        self.I_profits.append(I_profit)
        self.T_profits.append(T_profit)
        self.S_profits.append(S_profit)
        self.M_profits.append(M_profit)

    def draw_client_profit(self):
        #开始画图
        plt.figure(figsize=(18,7))
        plt.title(self.contract_id+" client profit changes")
        plt.plot(self.index, self.H_profits, label='H')
        plt.plot(self.index, self.P_profits, label='P')
        plt.plot(self.index, self.B_profits, label='B')
        plt.plot(self.index, self.I_profits, label='I')
        plt.plot(self.index, self.T_profits, label='T')
        plt.plot(self.index, self.S_profits, label='S')
        plt.plot(self.index, self.M_profits, label='M')
        plt.legend() #显示图例
        plt.xlabel("date")
        plt.ylabel("profit")
        plt.xticks(rotation=90)
        plt.gca().xaxis.set_major_locator(ticker.MultipleLocator(8))
        plt.gcf().autofmt_xdate()
        plt.savefig(self.contract_id+" client profit changes.jpg",dpi=200)

    def run(self,start,end):
        for k in range(start,end):
            self.get_class(k)
        self.draw_client_profit()

if __name__ == '__main__':
    Process_client_info("i1809").run(0,160)

