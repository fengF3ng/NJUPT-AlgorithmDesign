#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int maxn = 1e4;

class BipartiteGraph {
    /**
     * Class of Bipartite Graph
     **/
  private:
    int vertex_num, edge_num;  // save the num of vertexes and edges
    bool right_denied = false; // if the graph is not a Bipartite Graph then
                               // refuse to avtive hungarian_algorithm
    vector<int> edge[maxn];    // save the edge by vector array
    void vector_delete(vector<int> &, int); // delete a itme from vector
    bool judge_bipartite_graph(int);        // judge if a graph is bipartite
    bool augmenting_path(int, bool *, bool *, vector<int> *, vector<int> *,
                         int); // try to make a augmenting path

  public:
    BipartiteGraph();           // constructor
    ~BipartiteGraph();          // destructor
    void hungarian_algorithm(); // match edge with hungarian algorithm
};

int main() {
    while (true) {
        BipartiteGraph a = BipartiteGraph();
        a.hungarian_algorithm();
    }
    system("pause");
}

BipartiteGraph::BipartiteGraph() {
    cout << "Please input the num of vertexs:" << endl;
    cin >> vertex_num;
    cout << "Please input the num of edges:" << endl;
    cin >> edge_num;
    cout << "Note that do not input repeat edges and input by format of 3,4 3,7"
         << endl;
    for (int i = 0; i < edge_num; i++) {
        int x, y;
        cout << "Please input the " << i + 1 << " th edge:" << endl;
        cin >> x;
        getchar();
        cin >> y;
        edge[x].push_back(y);
        edge[y].push_back(x);
    }
    right_denied = false;
    for (int i = 0; i < edge_num; i++) {
        if (!judge_bipartite_graph(i)) {
            cout << "The graph you input is not a bipartitegraph!" << endl;
            right_denied = true; // refuse to active hungarian algorithm
            break;
        }
    }
}

BipartiteGraph::~BipartiteGraph() { return; }

void BipartiteGraph::vector_delete(vector<int> &my_vector, int num) {
    /**
     * delete a item from vector
     **/
    for (auto iter = my_vector.begin(); iter != my_vector.end(); iter++) {
        if (*iter == num) {
            my_vector.erase(iter);
            break;
        }
    }
}

bool BipartiteGraph::judge_bipartite_graph(int start = 0) {
    /**
     * judge if the graph is bipartite graph by staining test
     * if the graph is not connected
     **/
    int color[maxn];
    queue<int> my_queue;
    memset(color, 0, sizeof(color));
    my_queue.push(start);
    color[start] = 1;
    while (!my_queue.empty()) {
        int tmp_vertex = my_queue.front();
        my_queue.pop();
        for (auto iter = edge[tmp_vertex].begin();
             iter != edge[tmp_vertex].end(); iter++) {
            int next_vertex = *iter;
            if (!color[next_vertex]) {
                color[next_vertex] = -color[tmp_vertex];
                my_queue.push(next_vertex);
            } else { // one vertex has been colored twice
                if (color[next_vertex] + color[tmp_vertex] != 0)
                    return false;
            }
        }
    }
    return true;
}

bool BipartiteGraph::augmenting_path(int tmp_vertex, bool *visit, bool *marked,
                                     vector<int> *un_matched,
                                     vector<int> *matched, int flag = 1) {
    /**
     * try aumenting path
     **/
    if (flag == 1) {
        /*
            search for unmatched edge
        */
        for (auto iter = un_matched[tmp_vertex].begin();
             iter != un_matched[tmp_vertex].end(); iter++) {
            /*
                has found unmatched vertex or augmenting path
            */
            visit[*iter] = true;
            if (!marked[*iter] || augmenting_path(*iter, visit, marked,
                                                  un_matched, matched, -flag)) {
                marked[*iter] = true;
                marked[tmp_vertex] = true;
                matched[tmp_vertex].push_back(*iter);
                matched[*iter].push_back(tmp_vertex);
                vector_delete(un_matched[*iter], tmp_vertex);
                vector_delete(un_matched[tmp_vertex], *iter);
                return true;
            }
            visit[*iter] = false;
        }
        return false;
    } else {
        /*
            search for matched edge
        */
        for (auto iter = matched[tmp_vertex].begin();
             iter != matched[tmp_vertex].end(); iter++) {
            visit[*iter] = true;
            if (augmenting_path(*iter, visit, marked, un_matched, matched,
                                -flag)) {
                marked[*iter] = true;
                marked[tmp_vertex] = true;
                un_matched[tmp_vertex].push_back(*iter);
                un_matched[*iter].push_back(tmp_vertex);
                vector_delete(matched[*iter], tmp_vertex);
                vector_delete(matched[tmp_vertex], *iter);
                return true;
            }
            visit[*iter] = false;
        }
        return false;
    }
}

void BipartiteGraph::hungarian_algorithm() {
    /**
     * match edge with hungarian algorithm
     **/
    if (right_denied) {
        cout << "Cant run with hungarian algorithm for the graph is not a "
                "Bipartite Graph!"
             << endl;
        return;
    }
    /*
        init
    */
    bool visit[maxn], marked[maxn];
    vector<int> un_matched[maxn];
    vector<int> matched[maxn];
    int cnt = 0;
    memset(marked, 0, sizeof(marked));
    for (int i = 0; i < vertex_num; i++) {
        for (auto iter = edge[i].begin(); iter != edge[i].end(); iter++) {
            un_matched[i].push_back(*iter);
        }
    }
    /*
        match edge if
        vertex not matched: try to match by augmenting path
        vertex matched: continue
    */
    for (int i = 0; i < vertex_num; i++) {
        memset(visit, 0, sizeof(visit));
        if (marked[i])
            continue;
        cnt += augmenting_path(i, visit, marked, un_matched, matched);
    }
    /*
        print the result
    */
    memset(visit, 0, sizeof(visit));
    cout << "The most match edges num is " << cnt << endl;
    for (int i = 0; i < vertex_num; i++) {
        if (visit[i])
            continue;
        visit[i] = true;
        for (auto iter = matched[i].begin(); iter != matched[i].end(); iter++) {
            visit[*iter] = true;
            cout << i << "<---->" << *iter << endl;
        }
    }
}