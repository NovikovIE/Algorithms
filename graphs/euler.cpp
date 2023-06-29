#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

using std::cin;
using std::cout;
using std::list;
using std::pair;
using std::vector;

class Graph {
public:
    Graph(size_t n, const vector<vector<bool>>& edges);

    vector<pair<size_t, size_t>> find_euler_cycle(size_t start);

private:
    vector<list<size_t>> data;
    vector<size_t> euler_path;

    void euler(size_t v);
};

Graph::Graph(size_t n, const vector<vector<bool>>& edges) : data(n, list<size_t>()) {
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (!edges[i][j] && i != j) {
                data[i].push_back(j);
            }
        }
    }
}

vector<pair<size_t, size_t>> Graph::find_euler_cycle(size_t start) {
    euler(start);
    vector<pair<size_t, size_t>> euler_path_edges;
    std::reverse(euler_path.begin(), euler_path.end());
    for (size_t i = 0; i < euler_path.size() - 1; ++i) {
        euler_path_edges.emplace_back(euler_path[i], euler_path[i + 1]);
    }
    return euler_path_edges;
}

void Graph::euler(size_t v) {
    while (!data[v].empty()) {
        size_t u = *data[v].begin();
        data[v].erase(data[v].begin());
        euler(u);
    }
    euler_path.push_back(v + 1);
}
