#include <algorithm>
#include <iostream>
#include <list>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::vector;

class Graph {
public:
    explicit Graph(vector<vector<long long>>& edges) : data(edges) {}
    vector<vector<long long>> get_shortest_paths();

private:
    vector<vector<long long>> data;

    void floyd();
};

vector<vector<long long>> Graph::get_shortest_paths() {
    floyd();
    return data;
}

void Graph::floyd() {
    for (auto i = 0; i < data.size(); ++i) {
        for (auto& j : data) {
            for (auto k = 0; k < data.size(); ++k) {
                j[k] = std::min(j[k], j[i] + data[i][k]);
            }
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    long long n;
    cin >> n;
    vector<vector<long long>> edges(n, vector<long long>(n));
    for (long long i = 0; i < n; ++i) {
        for (long long j = 0; j < n; ++j) {
            cin >> edges[i][j];
        }
    }
    Graph graph(edges);
    vector<vector<long long>> paths = graph.get_shortest_paths();
    for (long long i = 0; i < paths.size(); ++i) {
        for (long long j = 0; j < paths.size(); ++j) {
            cout << paths[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}