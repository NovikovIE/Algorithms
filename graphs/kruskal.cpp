#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::numeric_limits;
using std::sort;
using std::vector;

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(long long n) : parent(vector<long long>(n, -1)), size(n, 1) {}
    long long find_set(long long a);
    void unite_sets(long long a, long long b);

private:
    vector<long long> parent;
    vector<long long> size;
};

long long DisjointSetUnion::find_set(long long int a) {
    if (parent[a] == -1) {
        return a;
    }
    return parent[a] = find_set(parent[a]);
}

void DisjointSetUnion::unite_sets(long long int a, long long int b) {
    long long corner1 = find_set(a);
    long long corner2 = find_set(b);
    if (corner1 == corner2) {
        return;
    }
    if (size[corner1] < size[corner2]) {
        std::swap(corner1, corner2);
    }
    parent[corner2] = corner1;
    size[corner1] += size[corner2];
}

class Graph {
public:
    Graph(long long n, vector<vector<long long>>& edges) : number_of_vertices(n), data(edges) {}

    long long mst() const;

private:
    long long number_of_vertices;
    vector<vector<long long>> data;
};

long long Graph::mst() const {
    DisjointSetUnion dsu(number_of_vertices);
    long long result = 0;
    for (auto& edge : data) {
        if (dsu.find_set(edge[1]) != dsu.find_set(edge[2])) {
            result += edge[0];
            dsu.unite_sets(edge[1], edge[2]);
        }
    }
    return result;
}
