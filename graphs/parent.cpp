#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

enum Color { WHITE, GRAY, BLACK };

class Graph {
public:
    // The first line of the input file contains a natural number n (1≤n≤100000) — the number of vertices in the tree.
    // The second line contains n numbers.
    // In this case, the i-th number of the second line determines the immediate parent of the vertex with number i.
    // If the parent number is zero, then the vertex is the root of the tree.
    explicit Graph(const vector<size_t>& ancestors);

    // is u an ancestor of v
    bool is_ancestor(size_t u, size_t v) const { return t_in[u] < t_in[v] && t_out[u] > t_out[v]; }

private:
    vector<vector<size_t>> data;
    vector<size_t> t_in;
    vector<size_t> t_out;
    size_t timer;
    vector<Color> color;
    size_t root;
    void dfs(size_t v);
};

Graph::Graph(const vector<size_t>& ancestors)
    : data(ancestors.size(), vector<size_t>()),
      t_in(ancestors.size()),
      t_out(ancestors.size()),
      color(ancestors.size(), Color::WHITE) {
    for (size_t i = 0; i < ancestors.size(); ++i) {
        if (!ancestors[i])
            root = i;
        else
            data[ancestors[i] - 1].push_back(i);
    }
    timer = 0;
    dfs(root);
}

void Graph::dfs(size_t v) {
    t_in[v] = timer++;
    color[v] = Color::GRAY;
    for (size_t& elem : data[v]) {
        if (color[elem] != Color::WHITE)
            continue;
        dfs(elem);
    }
    t_out[v] = timer++;
    color[v] = Color::BLACK;
}