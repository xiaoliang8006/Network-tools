# -*- coding: utf-8 -*-
import sys
sys.path.append('../')
from gexf import Gexf
import pandas as pd
from Utils import Logger, List_file
import json

class Gexf_gene(object):
    def __init__(self, date):
        self.gexf = Gexf("Gephi.org","A Web network")
        self.graph=self.gexf.addGraph("directed", "slice", "timestamp", date)
        f = open('config.json', 'r')
        self.config = json.load(f)
        f.close()

        # node attribyte
        self.node_attrs = []
        for i in range(len(self.config['NodeAttr'])):
            self.node_attrs.append(self.graph.addNodeAttribute(self.config['NodeAttr'][i], type=self.config['NodeAttrType'][i]))

        # edge attribyte
        self.edge_attrs = []
        for i in range(len(self.config['EdgeAttr'])):
            self.edge_attrs.append(self.graph.addEdgeAttribute(self.config['EdgeAttr'][i], self.config['EdgeAttrDefaultValue'][i]))

    def import_to_gephi(self, Vertex, Graph):
        # import node
        for i in range(len(Vertex)):
            line = Vertex.iloc[i]
            node_info = self.graph.addNode(line[0], line[0])
            for i in range(len(self.config['NodeAttr'])):
                node_info.addAttribute(self.node_attrs[i], line[i+1])
        # import edge
        for i in range(len(Graph)):
            line = Graph.iloc[i]
            edge_info = self.graph.addEdge(str(i), line[1], line[0])
            for i in range(len(self.config['EdgeAttr'])):
                edge_info.addAttribute(self.edge_attrs[i], line[i+2])



if __name__ == '__main__':
    Lf = List_file()
    graphs = Lf.eachfile("data/Graph")
    vertexs = Lf.eachfile("data/Vertex")
    days = len(graphs)

    for i in range(0, days):
        date = graphs[i].split("_")[-1].split(".")[0]
        gg = Gexf_gene(str(i))
        Vertex = pd.read_csv(vertexs[i], header=None, dtype=str, sep=" ")
        Graph = pd.read_csv(graphs[i], header=None, dtype=str, sep=" ")
        gg.import_to_gephi(Vertex, Graph)
        output_file = open("timeline/data_" + date + ".gexf", "wb+")
        gg.gexf.write(output_file)
