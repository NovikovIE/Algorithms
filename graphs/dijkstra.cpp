#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <vector>

using std::cin;
using std::cout;
using std::make_pair;
using std::map;
using std::numeric_limits;
using std::pair;
using std::priority_queue;
using std::set;
using std::vector;

const long long MAX = numeric_limits<int>::max();

class Graph {
public:
    Graph(long long n, vector<vector<long long>>& edgs);

    vector<long long> find_shortest_paths(int v);

private:
    long long vertices;
    vector<vector<pair<long long, long long>>> data;
    vector<vector<long long>> edges;
    vector<long long> dist;
    set<pair<long long, long long>> current_edges;
    vector<long long> parent;

    void dijkstra(long long i);
};

Graph::Graph(long long int n, vector<vector<long long int>>& edgs) : vertices(n), data(n), edges(edgs) {
    for (auto& edge : edgs) {
        data[edge[0]].push_back(make_pair(edge[1], edge[2]));
        data[edge[1]].push_back(make_pair(edge[0], edge[2]));
    }
}

vector<long long> Graph::find_shortest_paths(int v) {
    dijkstra(v);
    return dist;
}

void Graph::dijkstra(long long int i) {
    current_edges.clear();
    parent.assign(vertices, -1);
    dist = vector<long long>(vertices, MAX);
    dist[i] = 0;
    current_edges.insert({dist[i], i});
    while (!current_edges.empty()) {
        long long vertex = current_edges.begin()->second;
        current_edges.erase(current_edges.begin());
        for (auto it = data[vertex].begin(); it != data[vertex].end(); ++it) {
            if (dist[vertex] + it->second < dist[it->first]) {
                current_edges.erase({dist[it->first], it->first});
                parent[it->first] = vertex;
                dist[it->first] = dist[vertex] + it->second;
                current_edges.insert({dist[it->first], it->first});
            }
        }
    }
}
