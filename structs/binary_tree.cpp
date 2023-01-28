#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::max;
using std::min;
using std::pair;
using std::vector;

const size_t MAX_WIDTH_SIZE = 100'000;
const size_t MAX_TREE_SIZE = MAX_WIDTH_SIZE * 4;

class BinaryTree {
public:
    explicit BinaryTree(vector<int>& values) { build(values, 0, MAX_WIDTH_SIZE, 1); }
    int get_min(int l, int r) const { return get_min(1, 0, MAX_WIDTH_SIZE, l, r); }
    int get_max(int l, int r) const { return get_max(1, 0, MAX_WIDTH_SIZE, l, r); }
    void update(int pos, int value) { update(1, 0, MAX_WIDTH_SIZE, pos, value); }

private:
    vector<pair<int, int>> nodes = vector<pair<int, int>>(MAX_TREE_SIZE);

    void update(int v, int tl, int tr, int pos, int value);
    int get_min(int pos, int tl, int tr, int l, int r) const;
    int get_max(int pos, int tl, int tr, int l, int r) const;
    void build(vector<int>& values, int tl, int tr, int pos);
    void min_max_update(int pos);
};

void BinaryTree::build(vector<int>& values, int tl, int tr, int pos) {
    if (tl == tr)
        nodes[pos].first = nodes[pos].second = values[tl];
    else {
        int tm = (tl + tr) >> 1;
        build(values, tl, tm, pos * 2);
        build(values, tm + 1, tr, pos * 2 + 1);
        min_max_update(pos);
    }
}

void BinaryTree::update(int v, int tl, int tr, int pos, int value) {
    if (tl == tr)
        nodes[v].first = nodes[v].second = value;
    else {
        int tm = (tl + tr) >> 1;
        if (pos <= tm)
            update(2 * v, tl, tm, pos, value);
        else
            update(2 * v + 1, tm + 1, tr, pos, value);
        min_max_update(v);
    }
}

int BinaryTree::get_min(int pos, int tl, int tr, int l, int r) const {
    if (l > r)
        return std::numeric_limits<int>::max();
    if (tl == l && tr == r) {
        return nodes[pos].first;
    }
    int tm = (tl + tr) >> 1;
    int res1 = get_min(pos * 2, tl, tm, l, min(tm, r));
    int res2 = get_min(pos * 2 + 1, tm + 1, tr, max(tm + 1, l), r);
    return min(res1, res2);
}

int BinaryTree::get_max(int pos, int tl, int tr, int l, int r) const {
    if (l > r)
        return std::numeric_limits<int>::min();
    if (tl == l && tr == r) {
        return nodes[pos].second;
    }
    int tm = (tl + tr) >> 1;
    int res1 = get_max(pos * 2, tl, tm, l, min(tm, r));
    int res2 = get_max(pos * 2 + 1, tm + 1, tr, max(tm + 1, l), r);
    return max(res1, res2);
}

void BinaryTree::min_max_update(int pos) {
    nodes[pos].first = min(nodes[pos * 2].first, nodes[pos * 2 + 1].first);
    nodes[pos].second = max(nodes[pos * 2].second, nodes[pos * 2 + 1].second);
}
