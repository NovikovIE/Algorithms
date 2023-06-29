#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::vector;

const long long MAX = std::numeric_limits<long long>::max();

class Graph {
public:
    Graph(size_t n, vector<vector<long long>>& edges);
    vector<long long> paths(long long v);

private:
    vector<vector<long long>> data;
    vector<vector<long long>> dfs_data;
    vector<long long> dist;
    vector<long long> parent;
    vector<bool> cycles;
    vector<char> color;

    void ford_bellman();
    void dfs(long long v);
};

Graph::Graph(size_t n, vector<vector<long long int>>& edges)
    : data(edges), dfs_data(n), dist(n, MAX), parent(n, -1), cycles(n, false) {
    for (auto& edge : data) {
        dfs_data[edge[0]].push_back(edge[1]);
    }
}

// finds the shortest paths from v
// result == MAX  => path does not exist
// result == -MAX => shortest path does not exist
vector<long long> Graph::paths(long long v) {
    dist[v] = 0;
    ford_bellman();
    for (size_t i = 0; i < dist.size(); ++i) {
        if (cycles[i]) {
            dist[i] = -MAX;
        }
    }
    return dist;
}

void Graph::ford_bellman() {
    for (long long i = 0; i < dist.size(); ++i) {
        for (auto& edge : data) {
            if (dist[edge[0]] < MAX && dist[edge[1]] > dist[edge[0]] + edge[2]) {
                dist[edge[1]] = std::max(-MAX, dist[edge[0]] + edge[2]);
                parent[edge[1]] = edge[0];
            }
        }
    }
    while (true) {
        long long temp = -1;
        for (auto& edge : data) {
            if (!cycles[edge[1]] && dist[edge[0]] < MAX && dist[edge[1]] > dist[edge[0]] + edge[2]) {
                dist[edge[1]] = std::max(-MAX, dist[edge[0]] + edge[2]);
                parent[edge[1]] = edge[0];
                temp = edge[1];
            }
        }
        if (temp == -1) {
            break;
        }
        long long a = temp;
        for (long long i = 0; i < temp; ++i) {
            a = parent[a];
        }
        temp = parent[a];
        dist[a] = -MAX;
        cycles[a] = true;
        if (temp == a) {
            color.assign(dist.size(), 'W');
            dfs(temp);
        }
        while (temp != a) {
            color.assign(dist.size(), 'W');
            dfs(temp);
            temp = parent[temp];
        }
    }
}

void Graph::dfs(long long v) {
    color[v] = 'G';
    for (auto& elem : dfs_data[v]) {
        if (color[elem] == 'W') {
            dfs(elem);
        }
    }
    cycles[v] = true;
    dist[v] = -MAX;
    color[v] = 'B';
}
