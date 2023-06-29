#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::vector;

enum Color { WHITE, GRAY, BLACK };

class Graph {
public:
    Graph(size_t n, const vector<pair<size_t, size_t>>& edges);

    // if graph is cyclic returns empty vector
    vector<size_t> topological_sort();

private:
    vector<vector<size_t>> data;
    vector<char> color;
    bool is_cyclic = false;
    vector<size_t> sorted = vector<size_t>();

    void dfs(size_t v);
};

Graph::Graph(size_t n, const vector<pair<size_t, size_t>>& edges) : data(n, vector<size_t>(0)), color(n, Color::WHITE) {
    for (auto& edge : edges) {
        data[edge.first].push_back(edge.second);
    }
}

vector<size_t> Graph::topological_sort() {
    for (size_t i = 0; i < data.size(); ++i) {
        if (is_cyclic)
            break;
        if (color[i] == Color::WHITE)
            dfs(i);
    }
    if (is_cyclic) {
        sorted.clear();
        return sorted;
    } else {
        std::reverse(sorted.begin(), sorted.end());
        return sorted;
    }
}

void Graph::dfs(size_t v) {
    color[v] = Color::GRAY;
    for (size_t& elem : data[v]) {
        if (color[elem] == Color::WHITE) {
            dfs(elem);
        } else if (color[elem] == Color::GRAY) {
            is_cyclic = true;
            return;
        }
    }
    sorted.push_back(v);
    color[v] = Color::BLACK;
}
