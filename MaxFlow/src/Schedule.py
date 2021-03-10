# -*- coding: utf-8 -*-
import sys
sys.path.append('../../')
from Utils import Logger,List_file
import ctypes
import subprocess
import signal
import time
import random
import pandas as pd


# Define signal handler function
def myHandler(signum, frame):
    exit("TimeoutError")

def processVIC():
    Vertex = pd.read_csv("../data/Vertex.txt", header=None)
    Vertex['index'] = [i for i in range(len(Vertex))]
    VIC_list = pd.read_csv("../data/VIC_list.txt", header=None)
    VIC = Vertex[Vertex[0].isin(VIC_list[0].tolist())]
    VIC.loc[:,'index'].to_csv("../data/VIC.txt", header=None, index=None)

subprocess.run("g++ ISAP.cpp -o ISAP -std=c++11", shell=True)

def run_1(source, target):
    # print("ISAP processing...")
    subprocess.run("./ISAP " + str(source)+" "+str(target), shell=True)
def run_2(source,target):
    subprocess.run("python Dinic.py " + str(source)+" "+str(target), shell=True)

def run():
    VIC = pd.read_csv("../data/VIC.txt", header=None)
    VIC = VIC[0].tolist()
    L = len(VIC)
    for i in range(0,L):
        for j in range(i+1,L):
            source = VIC[i]
            target = VIC[j]
            try:
                signal.signal(signal.SIGALRM, myHandler)
                signal.alarm(10)
                run_1(source, target)
            except:
                print("交给Dinic处理...")
                run_2(source, target)
            try:
                signal.signal(signal.SIGALRM, myHandler)
                signal.alarm(10)
                run_1(target, source)
            except:
                print("交给Dinic处理...")
                run_2(target, source)

if __name__ == '__main__':
    processVIC()
    run()

