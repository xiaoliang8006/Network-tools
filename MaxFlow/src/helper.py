import sys
class helper():
    def __init__(self):
        pass
    def init(self,infile):
        vertex = [] # adjacency list (list of list)
        cap = [] # list of dict of capacities
        flow = [] # flow of edge (a,b): also list of dict, initially set to 0
        level = [] #initialize this to length of the graph and set to zero
        path = [] #prev node of a. initially set to none
        #read in graph
        f = open(infile)
        for line in f.readlines():
            if line:
                #print(line)
                record = line.strip().split(" ")
                #print(record)
                if line[0] =='n':
                    numnodes = int(record[1]) + 2 #make space for super node and sink
                    for i in range(numnodes):
                        vertex.append([])
                        cap.append({})
                        flow.append({})
                        level.append(0)
                        path.append(None)
                else:
                    head = int(record[1])
                    tail = int(record[0])
                    vertex[head].append(tail)

                    cap[head][tail] = int(record[2])

                    flow[head][tail] = 0
        f.close()

        #support multiple sources and sinks
        #create super node if multiple sink and source
        return vertex, cap, flow, level, path

    #write output to file
    @staticmethod
    def writeGraph(self, args, vertex, flows, max_flow):
        if(args.output != None):
            with open(args.output,"w") as f:
                for i in range(1, len(vertex) -1):
                    for j in vertex[i]:
                        if j != len(vertex) -1:
                            f.write("f "+ str(i)+" "+ str(j) +" "+ str(flows[i][j])+"\n")
                f.write("max_flow: "+str(max_flow))
                f.flush()
        else: #no output file. Write to stdout
            for i in range(1, len(vertex) -1):
                for j in vertex[i]:
                    if j != len(vertex) -1:
                        print("f", str(i), str(j), str(flows[i][j]))
            print("max_flow:",str(max_flow))
