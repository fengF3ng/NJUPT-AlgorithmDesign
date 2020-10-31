#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>
#include<cstring>
using namespace std;
const int maxn = 1e4;

class BipartiteGraph{
	/***
	*** 1.vertex can be 3,4,9,10,'a'
	*** 2.reset all_matched 
	***/
	private:
		int vertex_num,edge_num;
		bool all_matched = false,right_denied = false;
		vector<int>edge[maxn];
		vector<int>matched_edge[maxn];
	public:
		BipartiteGraph();
		~BipartiteGraph();
		/*
		void add_edge();
		void add_vertex();
		void delete_vertex();
		void delete_edge();*/
		void vector_delete(vector<int> &,int);
		void print_graph();
		void print_bipartite_graph();
		
		bool judge_bipartite_graph(int);
		bool augmenting_path(int,bool*,bool*,vector<int>*,vector<int>*,int);
		void hungarian_algorithm();
		void hopcroft_karp();
		
};

BipartiteGraph::BipartiteGraph(){
	//memset(matched,false,sizeof(matched));
	cout<<"Please input the num of vertexs:"<<endl;
	cin>>vertex_num;
	cout<<"Please input the num of edges:"<<endl;
	cin>>edge_num;
	cout<<"Note that do not input repeat edges and input by format of 3,4 3,7"<<endl;
	for(int i=0;i<edge_num;i++){
		int x,y;
		cout<<"Please input the "<<i+1<<" th edge:"<<endl;
		cin>>x;
		getchar();
		cin>>y;
		edge[x].push_back(y);
		edge[y].push_back(x);
	}
	for(int i=0;i<edge_num;i++){
		if(!judge_bipartite_graph(i)){
			cout<<"The graph you input is not a bipartitegraph!"<<endl;
			right_denied = true;
			break;
		}
	}
}
BipartiteGraph::~BipartiteGraph(){
	return ;
}
void BipartiteGraph::vector_delete(vector<int> &my_vector,int num){
	for(auto iter = my_vector.begin();iter!=my_vector.end();iter++){
		if(*iter==num){
			my_vector.erase(iter);
			break;
		}
	}
}
void BipartiteGraph::print_graph(){
	return ;
}
void BipartiteGraph::print_bipartite_graph(){
	return ;
}
bool BipartiteGraph::judge_bipartite_graph(int start = 0){
	/*
	用染色法判断是否为二分图
	非联通情况下无法完全判断
	*/
	int color[maxn];
	queue<int>my_queue;
	memset(color,0,sizeof(color));
	my_queue.push(start);
	color[start] = 1;
	while(!my_queue.empty()){
		int tmp_vertex = my_queue.front();
		my_queue.pop();
		for(auto iter = edge[tmp_vertex].begin();iter!=edge[tmp_vertex].end();iter++){
			int next_vertex = *iter;
			if(!color[next_vertex]){
				color[next_vertex] = -color[tmp_vertex];
				my_queue.push(next_vertex);
			}else{
				if(color[next_vertex]+color[tmp_vertex]!=0)return false;
			}
		}
	}
	return true;
}
bool BipartiteGraph::augmenting_path(int tmp_vertex,bool *visit,bool *marked,vector<int> *un_matched,vector<int> *matched,int flag=1){
	if(flag==1){
		/*
		寻找未匹配边
		*/
		for(auto iter = un_matched[tmp_vertex].begin();iter!=un_matched[tmp_vertex].end();iter++){
			/*
			寻找到未匹配点或存在增广路
			*/
			visit[*iter] = true;
			if(!marked[*iter] || augmenting_path(*iter,visit,marked,un_matched,matched,-flag)){
				//cout<<tmp_vertex<<" "<<*iter<<endl;
				marked[*iter] = true;
				marked[tmp_vertex] = true;
				matched[tmp_vertex].push_back(*iter);
				matched[*iter].push_back(tmp_vertex);
				vector_delete(un_matched[*iter],tmp_vertex);
				vector_delete(un_matched[tmp_vertex],*iter);
				return true;
			}
			visit[*iter] = false;
		} 
		return false;
	}else{
		/*
		寻找匹配边
		*/
		for(auto iter = matched[tmp_vertex].begin();iter!=matched[tmp_vertex].end();iter++){
			visit[*iter] = true;
			if(augmenting_path(*iter,visit,marked,un_matched,matched,-flag)){
				//cout<<tmp_vertex<<" "<<*iter<<endl;
				marked[*iter] = true;
				marked[tmp_vertex] = true;
				un_matched[tmp_vertex].push_back(*iter);
				un_matched[*iter].push_back(tmp_vertex);
				vector_delete(matched[*iter],tmp_vertex);
				vector_delete(matched[tmp_vertex],*iter);
				return true;
			}
			visit[*iter] = false;
		}
		return false;
	}
}
void BipartiteGraph::hungarian_algorithm(){
	/*
	匈牙利算法二分匹配
	*/
	bool visit[maxn],marked[maxn];
	vector<int> un_matched[maxn];
	vector<int> matched[maxn];
	int cnt = 0;
	memset(marked,0,sizeof(marked));
	for(int i=0;i<vertex_num;i++){
		for(auto iter = edge[i].begin();iter!=edge[i].end();iter++){
			un_matched[i].push_back(*iter);
		}
	}
	for(int i=0;i<vertex_num;i++){
		memset(visit,0,sizeof(visit));
		if(marked[i])continue;
		cnt += augmenting_path(i,visit,marked,un_matched,matched);
	}
	memset(visit,0,sizeof(visit));
	cout<<cnt<<endl;
	for(int i=0;i<vertex_num;i++){
		if(visit[i])continue;
		visit[i] = true;
		for(auto iter = matched[i].begin();iter!=matched[i].end();iter++){
			visit[*iter] = true;
			cout<<i<<"<---->"<<*iter<<endl;
		}
	}
}
void BipartiteGraph::hopcroft_karp(){
	return ;
}
int main(){
	BipartiteGraph a = BipartiteGraph();
	a.hungarian_algorithm();
	system("pause");
} 
