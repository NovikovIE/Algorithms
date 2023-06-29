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
    Graph(size_t n, vector<pair<size_t, size_t>>& edges);

    set<pair<int, int>> find_bridges();

private:
    vector<vector<size_t>> data;
    vector<char> color;
    vector<int> t_in;
    vector<int> ret;
    set<pair<int, int>> bridges = set<pair<int, int>>();
    int timer = 0;

    void dfs(size_t v, int parent);
};

Graph::Graph(size_t n, vector<pair<size_t, size_t>>& edges)
    : data(n, vector<size_t>(0)), color(n, 'W'), t_in(n), ret(n) {
    for (auto& edge : edges) {
        data[edge.first].push_back(edge.second);
        data[edge.second].push_back(edge.first);
    }
}

set<pair<int, int>> Graph::find_bridges() {
    for (size_t i = 0; i < data.size(); ++i) {
        if (color[i] == 'W')
            dfs(i, -1);
    }
    return bridges;
}

void Graph::dfs(size_t v, int parent) {
    color[v] = 'G';
    ret[v] = t_in[v] = timer++;
    for (size_t& elem : data[v]) {
        if (parent == elem)
            continue;
        if (color[elem] == 'W') {
            dfs(elem, v);
            ret[v] = std::min(ret[v], ret[elem]);
            if (t_in[v] < ret[elem]) {
                bridges.insert({v, elem});
                bridges.insert({elem, v});
            }
        } else {
            ret[v] = std::min(t_in[elem], ret[v]);
        }
    }
    color[v] = 'B';
}
