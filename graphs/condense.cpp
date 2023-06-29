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
    // graph can contain multiple edges and loops
    Graph(size_t n, vector<pair<size_t, size_t>>& edges);

    pair<int, set<pair<size_t, size_t>>> condense();

private:
    vector<vector<size_t>> data;
    vector<vector<size_t>> inverted_data;

    vector<int> color;
    vector<bool> used;

    vector<size_t> order = vector<size_t>();

    void dfs(size_t v);

    void dfs_for_inverted(size_t v, int clr);
};

Graph::Graph(size_t n, vector<pair<size_t, size_t>>& edges)
    : data(n, vector<size_t>(0)), inverted_data(n, vector<size_t>(0)), color(n, -1), used(n, false) {
    for (auto& edge : edges) {
        data[edge.first].push_back(edge.second);
        inverted_data[edge.second].push_back(edge.first);
    }
}

pair<int, set<pair<size_t, size_t>>> Graph::condense() {
    for (size_t i = 0; i < data.size(); ++i) {
        if (!used[i])
            dfs(i);
    }
    int clr = 0;
    for (size_t i = 0; i < inverted_data.size(); ++i) {
        if (color[order[inverted_data.size() - i - 1]] == -1)
            dfs_for_inverted(order[inverted_data.size() - i - 1], clr++);
    }
    set<pair<size_t, size_t>> new_edges;
    for (size_t i = 0; i < data.size(); ++i) {
        for (auto item : data[i]) {
            if (color[i] != color[item])
                new_edges.insert({color[i], color[item]});
        }
    }
    return {clr, new_edges};
}

void Graph::dfs(size_t v) {
    used[v] = true;
    for (size_t& elem : data[v]) {
        if (!used[elem])
            dfs(elem);
    }
    order.push_back(v);
}

void Graph::dfs_for_inverted(size_t v, int clr) {
    color[v] = clr;
    for (size_t& elem : inverted_data[v]) {
        if (color[elem] == -1)
            dfs_for_inverted(elem, clr);
    }
}
