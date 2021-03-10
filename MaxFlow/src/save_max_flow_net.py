# -*- coding: utf-8 -*-
import sys
sys.path.append('../../')
from Utils import Logger,List_file
import pandas as pd
import subprocess
import numpy as np
import warnings
import csv,gc
warnings.filterwarnings("ignore")


class max_flow_net(object):
    def __init__(self):
        self.relations = pd.read_csv("../data/relations.txt", header=None, usecols=[0, 1, 2], dtype={0: str, 1: str}, sep=" ")
        self.nodes_list = pd.concat([self.relations[0], self.relations[1]])
        self.nodes_list.drop_duplicates(inplace=True)
        self.nodes_list.sort_values(inplace=True)
        self.nodes_list.to_csv("../data/Vertex.txt", mode='w+', header=0, index=0)
        self.nodes_list = self.nodes_list.tolist()
    def save_to_file(self):
        indexs = np.arange(0, len(self.nodes_list), 1).tolist()
        print("关系数量:", len(indexs))
        # 获取关系
        self.relations[0].replace(self.nodes_list, indexs, inplace=True)
        print("第一列替换完毕")
        self.relations[1].replace(self.nodes_list, indexs, inplace=True)
        print("第二列替换完毕\n")
        f = open("../data/Graph.txt", "w+")
        f.write("n "+str(len(self.nodes_list)) + "\n")
        f.close()
        self.relations.to_csv("../data/Graph.txt", mode='a+', header=0, index=0, sep=" ")
        del self.relations, indexs
        gc.collect()


if __name__ == '__main__':
    max_flow_net().save_to_file()

