import sys
sys.path.append('../')
# from Utils import Logger, List_file
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
import pandas as pd
from scipy.optimize import curve_fit
from sklearn import linear_model
matplotlib.use('Agg')
# Lf = List_file()

##### config start #####
# graph_files = ["./facebook_combined.txt"]
graph_files = ["../../../Final/mydata/f5.csv"]
# graph_files = Lf.eachfile("./data")
##### config end #####

def fund(x, a, b):
    return a / (x**b)

def show(name):
    for graph_file in graph_files:
        # date = graph_file.split("_")[1].split(".")[0]
        data = pd.read_csv(graph_file, header=None, sep=",")
        print(data)
        data = pd.DataFrame(pd.concat([data[0], data[1]], axis=0))
        data.reset_index(drop=True, inplace=True)
        result = pd.DataFrame(data[0].value_counts())
        result = pd.DataFrame(result[0].value_counts())
        result.reset_index(drop=False, inplace=True)
        # print(result)

        print(result)
        plt.figure(figsize=(12, 8))
        plt.scatter(result['index'], result[0] / result[0].sum())
        popt, pcov = curve_fit(fund, result['index'], result[0] / result[0].sum())
        print(popt)

        plt.title("degree distribution of " + name)
        x = np.linspace(0,1000,1000)
        # plt.plot(x, popt[0]/(x**popt[1]))
        plt.loglog()
        plt.ylim(0.00001, 1)
        plt.xlim(0, 100000)
        plt.xlabel('degrees')
        plt.ylabel('P(degrees)')
        plt.savefig("./degree distribution of " + name + ".png",dpi=400)
        # plt.show()
        
def show2(name):
    for graph_file in graph_files:
        # date = graph_file.split("_")[1].split(".")[0]
        data = pd.read_csv(graph_file, header=None, sep=",")
        # print(data)
        data = pd.DataFrame(pd.concat([data[0], data[1]], axis=0))
        data.reset_index(drop=True, inplace=True)
        result = pd.DataFrame(data[0].value_counts())
        result = pd.DataFrame(result[0].value_counts())
        result.reset_index(drop=False, inplace=True)
        result = result.sort_values(by='index', ascending=True)
        # print(result)

        # ratio
        degree_counts = result[0].tolist()
        degree_sums = [0] * len(degree_counts)
        degree_sums[-1] = degree_counts[-1]
        for i in range(len(degree_counts)-2, -1, -1):
            degree_sums[i] = degree_sums[i+1] + degree_counts[i]
        result['calm'] = degree_sums 

        plt.figure(figsize=(12, 8))
        plt.scatter(result['index'], result['calm'] / result['calm'].sum())
        popt, pcov = curve_fit(fund, result['index'], result['calm'] / result['calm'].sum())
        print(popt)

        plt.title("degree distribution of " + name)
        # x = np.linspace(0,1000,1000)
        # plt.plot(x, popt[0]/(x**popt[1]))
        plt.loglog()
        plt.ylim(0.00001, 1)
        plt.xlim(0, 100000)
        plt.xlabel('degrees')
        plt.ylabel('P(degrees)')
        plt.savefig("./degree distribution of " + name + ".png",dpi=400)
        # plt.show()

show("test")
