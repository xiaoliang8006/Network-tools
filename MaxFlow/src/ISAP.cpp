#include<iostream>
#include<algorithm>
#include<math.h>
#include<cstring>
#include<queue>
#include <fstream>
#include<set>
#include <vector>
#include <chrono>   //测量时间
using namespace std;
const int inf=0x7f7f7f7f;
const int maxn=500000; ////点数
const int maxm=3000000;    ////边数
vector<int> VIC;  //高频客户
vector<string> Vertex;  //所有客户
set<int> Num;
class Maxflow
{
public:
	int cnt;
	int src,sink;          	// 源点     汇点
	int pre[maxn];    		// 可增广路上的上一条弧的编号
	int num[maxn];  		// 和 t 的最短距离等于 i 的节点数量
	int cur[maxm*2+10];  		// 当前弧下标
	int d[maxn];    		// 残量网络中节点 i 到汇点 t 的最短距离
	int Head[maxm*2+10]; 		//邻接表头
	int visit[maxn];
	bool visited[maxn];
	struct Edge{
		int from,to,val,nxt;
	}edge[maxm*2+10];
	// 预处理, 反向 BFS 构造 d 数组

	void addedge(int u,int v,int w)
	{
		cnt++;
		edge[cnt].from=u;edge[cnt].to=v;edge[cnt].val=w;edge[cnt].nxt=Head[u];
		//edge[cnt]=(Edge){u,v,w,Head[u]};
		Head[u]=cnt;

		cnt++;
		edge[cnt].from=v;edge[cnt].to=u;edge[cnt].val=0;edge[cnt].nxt=Head[v];
		//edge[cnt]=(Edge){v,u,0,Head[v]};
		Head[v]=cnt;
	}

	void init()
	{
		//读取关键节点
		ifstream read_VIC;string line;
		read_VIC.open("../data/VIC.txt", ios::binary);
		while(getline(read_VIC, line)){
			VIC.push_back(stoi(line));
		}
		memset(visit,0,sizeof(visit));
		memset(Head,0,sizeof(Head));
		cnt=1;
        char temp;int na,u,v,val;
		//读取网络
        ifstream read_Graph;
        read_Graph.open("../data/Graph.txt", ios::binary);
        read_Graph >> temp >> na;
        while(read_Graph >> v >> u >> val){
            addedge(u,v,val);
		}
		////加边
		//读取节点
		ifstream read_Vertex;
		read_Vertex.open("../data/Vertex.txt", ios::binary);
		while(getline(read_Vertex, line)){
			Vertex.push_back(line);
		}
	}

	bool bfs()
	{
	    memset(visited, 0, sizeof(visited));
	    queue<int> Q;
	    Q.push(sink);
	    visited[sink] = 1;
	    d[sink] = 0;
	    while (!Q.empty())
		{
	        int u = Q.front();
	        Q.pop();
	        for (int i = Head[u]; i ; i=edge[i].nxt)
			{
	            Edge &e = edge[i^1]; ////引用反边

	            if (!visited[e.from]) ////未访问&&有残量
				{
					if(e.from!=src && e.from!=sink){
						vector<int>::iterator it = find(VIC.begin(), VIC.end(), e.from);
						if (it != VIC.end()) continue;
					}
	                visited[e.from] = true;
	                d[e.from] = d[u] + 1;
	                Q.push(e.from);
	            }
	        }
	    }
	    return visited[src];
	}

	// 增广
	int augment()
	{
	    int u = sink, df = inf;
	    // 从汇点到源点通过 p 追踪增广路径, df 为一路上最小的残量
	    while (u != src)
		{

	        df = min(df, edge[pre[u]].val);
	        u = edge[pre[u]].from;
	    }
	    u = sink;
	    // 从汇点到源点更新流量
	    while (u != src)
		{
			Num.insert(u);
	        edge[pre[u]].val -= df;
	        edge[pre[u]^1].val += df;
	        u = edge[pre[u]].from;
	    }
	    return df;
	}

	int maxflow()
	{
	    int flow = 0;
	    memset(num, 0, sizeof(num));
	    for(int i=0;i<=maxn;i++)cur[i]=Head[i];
	    bfs();
	    for (int i = 0; i < maxn; i++) num[d[i]]++; ////gap优化

	    int u = src;
	    while (d[src] < maxn)
		{
	        if (u == sink)
			{
	            flow += augment();
	            u = src;
	        }

	        bool advanced = false; ////判断是否增广成功
	        for (int i = cur[u]; i ; i=edge[i].nxt)
			{

	            Edge& e = edge[i];
	            if (e.val && d[u] == d[e.to] + 1)
				{
	                advanced = true;
	                pre[e.to] = i;
	                cur[u] = i; ////弧优化
	                u = e.to;
	                break;
	            }
	        }
	        if (!advanced)
			{ // retreat
	            int m = maxn - 1;
	            for (int i = Head[u]; i ; i=edge[i].nxt)
	            {
	                if (edge[i].val)  ////如果有残量
					{
						m = min(m, d[edge[i].to]);
					}
	            }

	            if (--num[d[u]] == 0) break; // gap 优化
	            num[d[u] = m+1]++;

	            cur[u] = Head[u]; //弧优化部分
	            if (u != src) u = edge[pre[u]].from;
	        }
	    }
	    return flow;
	}
}TIM;


int main(int argc, char*argv[])
{

    //写文件
    ofstream write_file("../flow_res.txt", ios::app);
    auto start = chrono::high_resolution_clock::now();
	TIM.init();
    TIM.src=stoi(argv[1]); TIM.sink=stoi(argv[2]);
    int ans=TIM.maxflow();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
	
    if(ans){
        write_file << Vertex[TIM.src] << "," << Vertex[TIM.sink] << "," << ans << "," << Num.size() << endl;
        cout << Vertex[TIM.src]<<" -> "<<Vertex[TIM.sink]<<"  最大流:" << ans << "  耗时:" << elapsed.count() <<"  经过的小节点数:"<< Num.size()<<endl;
    }else{
        cout << Vertex[TIM.src]<<" -> "<<Vertex[TIM.sink]<<" 没有连边！" << endl;
    }
    return 0;
}