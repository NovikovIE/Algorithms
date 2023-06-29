#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <vector>

using std::cin;
using std::cout;
using std::numeric_limits;
using std::pair;
using std::priority_queue;
using std::set;
using std::string;
using std::vector;

const long long MAX = numeric_limits<int>::max();

class Graph {
public:
    Graph(long long n, vector<vector<long long>>& edges);
    long long mst();

private:
    long long number_of_vertices;

    vector<vector<pair<long long, long long>>> data;
};

Graph::Graph(long long int n, vector<vector<long long int>>& edges) : number_of_vertices(n), data(n) {
    for (auto& edge : edges) {
        data[edge[0]].push_back({edge[1], edge[2]});
        data[edge[1]].push_back({edge[0], edge[2]});
    }
}

long long Graph::mst() {
    vector<bool> used(number_of_vertices, false);
    vector<long long> smallest_edges(number_of_vertices, MAX);
    smallest_edges[0] = 0;
    priority_queue<pair<long long, long long>> current_edges;
    current_edges.push({0, 0});
    while (!current_edges.empty()) {
        auto edge = current_edges.top();
        current_edges.pop();
        used[edge.second] = true;
        for (auto& edge_to : data[edge.second]) {
            if (!used[edge_to.first] && edge_to.second < smallest_edges[edge_to.first]) {
                smallest_edges[edge_to.first] = edge_to.second;
                current_edges.push({-smallest_edges[edge_to.first], edge_to.first});
            }
        }
    }
    long long result = 0;
    for (size_t i = 0; i < number_of_vertices; ++i) {
        result += smallest_edges[i];
    }
    return result;
}
