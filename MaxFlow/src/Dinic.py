import sys
sys.path.append('../../')
from Utils import Logger,List_file
from collections import deque
import sys
import argparse
from helper import helper
import time
import pandas as pd
'''
@Author: liangfh
Email: liangfh8006@gmail.com
Dinic algorithm
'''

class MaxFLowSolve(object):
	def __init__(self):
		self.VIC_list = pd.read_csv("../data/VIC.txt", header=None)
		self.VIC_list = self.VIC_list[0].tolist()


	#creates level graph using bfs search
	def createLevelGraph(self, source, sink):
		queue = deque([])
		queue.append(source)
		for l in range(len(self.level)):
			self.level[l] = 0
		self.level[source] = 1
		while queue:
			cur = queue.popleft()
			for i in range(len(self.vertex[cur])):
				nb = self.vertex[cur][i] #ith neighbor
				if nb!=source and nb!=sink and (nb in self.VIC_list):
					continue
				if((self.cap[cur][nb] - self.flow[cur][nb] > 0) and self.level[nb] == 0 ):
					queue.append(nb)
					self.level[nb] = self.level[cur] + 1
		return 	self.level[sink] != 0

	#compute blocking flows with DFS
	def createBlockingFlow(self, source, sink):
		fl = 0
		stack = [] #using a list with stack operations
		self.visited_nodes = []
		for l in range(len(self.vertex)):
			self.visited_nodes.append(0)
		stack.append(source)
		run = 0
		while stack:
			cur = stack[-1] #top
			if(cur != sink ):#advance step
				j = 0
				while j < len(self.vertex[cur]) and stack[-1] == cur:
					nb = self.vertex[cur][j]
					if self.visited_nodes[nb] == 1 or self.level[nb] != self.level[cur] + 1:
						j = j + 1
						continue
					if self.cap[cur][nb] - self.flow[cur][nb] > 0:
						stack.append(nb)
						self.path[nb] = cur
					j = j + 1
				if stack[-1] == cur: #at already visited node. Need this to avoid infinite loop
					stack.pop()
					self.visited_nodes[cur] = 1
			else:#augment step
				F = 1000000000
				bottleneck = None
				c = sink
				#find min(p)
				while c != source:
					F = min(F, (self.cap[self.path[c]][c] - self.flow[self.path[c]][c]) if (self.path[c] >= 0) else self.flow[c][-self.path[c]])
					c = abs(self.path[c])

				c = sink
				#update p as f = f + f'
				while c != source:
					self.flow[self.path[c]][c] = self.flow[self.path[c]][c] + F
					if (self.cap[self.path[c]][c] - self.flow[self.path[c]][c] == 0):
						bottleneck = self.path[c]
					c = abs(self.path[c])
				#retreat step
				while (stack and stack[len(stack) -1] != bottleneck):
					stack.pop();
				fl = fl+F
			run = run + 1
		return fl

	#main module
	def Dinic(self, source, sink):
		self.count = set()
		f  = 0
		while self.createLevelGraph(source, sink):
			f = f + self.createBlockingFlow(source, sink)
		for i in range(1, len(self.vertex) - 1):
			for j in self.vertex[i]:
				if j != len(self.vertex) - 1 and self.flow[i][j]!=0:
					self.count.add(i)
					self.count.add(j)
		return f

	def run(self,source,target):
		#initialize adjacency list and other program params
		#flag=0 means QTY, flag=1 means trade_times
		self.vertex, self.cap, self.flow, self.level, self.path = helper().init("../data/Graph.txt")
		return self.Dinic(source,target),len(self.count)


if __name__ == '__main__':
	start = time.time()
	source, target = int(sys.argv[1]), int(sys.argv[2])
	max_flow, count = MaxFLowSolve().run(source,target)
	end = time.time()
	t = round(end - start, 2)
	Vertex = pd.read_csv("../data/Vertex.txt", header=None, dtype={0: str})
	Vertex = Vertex[0].tolist()
	relation = pd.DataFrame(columns=['source', 'target', 'QTY', 'NODE_NUM'])  # 生成空的pandas表
	relation.loc[0] = [Vertex[source], Vertex[target], max_flow, count]
	relation.to_csv("../flow_res.txt", mode='a+', header=0, index=0)
	print(Vertex[source], "->", Vertex[target], " 最大流:", max_flow, " 耗时:", t, " 经过的小节点数:",count)
