#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::set;
using std::vector;

class Graph {
public:
    Graph(int n, vector<pair<int, int>>& edges);

    set<int> find_articulation_points();

private:
    vector<vector<int>> data;
    vector<char> color;
    vector<int> t_in;
    vector<int> ret;
    set<int> points = set<int>();
    int timer = 0;

    void dfs(int v, int parent);
};

Graph::Graph(int n, vector<pair<int, int>>& edges) : data(n, vector<int>(0)), color(n, 'W'), t_in(n), ret(n) {
    for (auto& edge : edges) {
        data[edge.first].push_back(edge.second);
        data[edge.second].push_back(edge.first);
    }
}

set<int> Graph::find_articulation_points() {
    dfs(0, -1);
    return points;
}

void Graph::dfs(int v, int parent) {
    color[v] = 'G';
    int count = 0;
    ret[v] = t_in[v] = timer++;
    for (int& elem : data[v]) {
        if (parent == elem) {
            continue;
        }
        if (color[elem] == 'W') {
            ++count;
            dfs(elem, v);
            ret[v] = std::min(ret[v], ret[elem]);
            if (parent != -1 && t_in[v] <= ret[elem]) {
                points.insert(v);
            }
        } else {
            ret[v] = std::min(t_in[elem], ret[v]);
        }
    }
    color[v] = 'B';
    if (parent == -1 && count > 1) {
        points.insert(v);
    }
}
