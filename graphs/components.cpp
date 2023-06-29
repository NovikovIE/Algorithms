#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::vector;

class Graph {
public:
    Graph(size_t n, vector<pair<size_t, size_t>>& edges);

    // returns pair
    // left : vector of belonging to a connected component
    // right : number of components
    pair<vector<int>, int> components();

private:
    vector<vector<size_t>> data;
    vector<int> color;

    void dfs(size_t v, int clr);
};

Graph::Graph(size_t n, vector<pair<size_t, size_t>>& edges) : data(n, vector<size_t>(0)), color(n, -1) {
    for (auto& edge : edges) {
        data[edge.first].push_back(edge.second);
        data[edge.second].push_back(edge.first);
    }
}

pair<vector<int>, int> Graph::components() {
    int clr = 0;
    for (size_t i = 0; i < data.size(); ++i) {
        if (color[i] == -1) {
            dfs(i, clr++);
        }
    }
    return {color, clr};
}

void Graph::dfs(size_t v, int clr) {
    color[v] = clr;
    for (size_t& elem : data[v]) {
        if (elem == v) {
            continue;
        }
        if (color[elem] == -1) {
            dfs(elem, clr);
        }
    }
}
