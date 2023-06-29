#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::vector;

enum Color { WHITE, GRAY, BLACK };

class Graph {
public:
    // n vertices
    Graph(size_t n, const vector<pair<size_t, size_t>>& edges);

    // returns empty vector if there are no cycles
    vector<size_t> find_cycle();

private:
    vector<vector<size_t>> data;
    vector<size_t> parent;
    vector<Color> color;
    int cycle_start = -1;
    int cycle_end = -1;
    void dfs(size_t v);
};

Graph::Graph(size_t n, const vector<pair<size_t, size_t>>& edges)
    : data(n, vector<size_t>(0)), parent(n), color(n, Color::WHITE) {
    for (const auto& edge : edges) {
        data[edge.first - 1].push_back(edge.second - 1);
    }
}

vector<size_t> Graph::find_cycle() {
    bool is_cycles_in_graph = false;
    for (size_t i = 0; i < data.size(); ++i) {
        if (color[i] != Color::WHITE)
            continue;
        dfs(i);
        if (cycle_start != -1) {
            is_cycles_in_graph = true;
            break;
        }
    }
    vector<size_t> cycle;
    if (is_cycles_in_graph) {
        cycle.push_back(cycle_start + 1);
        size_t temp = cycle_end;
        while (temp != cycle_start) {
            cycle.push_back(temp + 1);
            temp = parent[temp];
        }
    }
    return cycle;
}

void Graph::dfs(size_t v) {
    color[v] = Color::GRAY;
    for (size_t& elem : data[v]) {
        if (color[elem] == Color::WHITE) {
            parent[elem] = v;
            dfs(elem);
        } else if (color[elem] == Color::GRAY) {
            cycle_start = elem;
            cycle_end = v;
        }
    }
    color[v] = Color::BLACK;
}