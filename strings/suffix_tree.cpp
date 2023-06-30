#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

using ll = long long;

using std::cin;
using std::cout;
using std::map;
using std::queue;
using std::string;
using std::vector;

class SuffixTree {
private:
    struct Node {
        ll left = 0;
        ll right = 0;
        ll parent = -1;
        ll link = -1;
        map<char, ll> to;

        ll length() { return right - left; }

        Node(ll new_left, ll new_right, ll new_par) : left(new_left), right(new_right), parent(new_par) {}
        Node() = default;

        ll& get(char c);
    };

    struct State {
        ll vertex = 0;
        ll position = 0;
        State(ll v, ll pos) : vertex(v), position(pos) {}
        State() = default;
    };

    State pointer_;
    vector<Node> nodes_;
    string str_;

    State go(ll left, ll right, State state);
    ll split(State state);
    ll link(ll vertex);
    void extend_with_symbol(ll position);
    void dfs(ll vertex, ll& counter, ll parent);

public:
    explicit SuffixTree(const string& s);
    void show_tree();
};

ll& SuffixTree::Node::get(char c) {
    if (to.find(c) == to.end()) {
        to[c] = -1;
    }
    return to[c];
}

SuffixTree::State SuffixTree::go(ll left, ll right, SuffixTree::State state) {
    while (left < right) {
        if (nodes_[state.vertex].length() != state.position) {
            if (str_[state.position + nodes_[state.vertex].left] != str_[left]) {
                return {-1, -1};
            }
            if (nodes_[state.vertex].length() > state.position + right - left) {
                return {state.vertex, state.position + right - left};
            }
            left += nodes_[state.vertex].length() - state.position;
            state.position = nodes_[state.vertex].length();
        } else {
            state = State(nodes_[state.vertex].get(str_[left]), 0);
            if (state.vertex == -1) {
                return state;
            }
        }
    }
    return state;
}

ll SuffixTree::split(SuffixTree::State state) {
    if (state.position == nodes_[state.vertex].length()) {
        return state.vertex;
    }
    if (state.position == 0) {
        return nodes_[state.vertex].parent;
    }
    Node vertex = nodes_[state.vertex];
    nodes_.emplace_back(vertex.left, vertex.left + state.position, vertex.parent);
    nodes_[vertex.parent].get(str_[vertex.left]) = static_cast<ll>(nodes_.size()) - 1;
    nodes_[state.vertex].parent = static_cast<ll>(nodes_.size()) - 1;
    nodes_[state.vertex].left += state.position;
    nodes_[nodes_.size() - 1].get(str_[vertex.left + state.position]) = state.vertex;
    return static_cast<ll>(nodes_.size()) - 1;
}

ll SuffixTree::link(ll vertex) {
    if (nodes_[vertex].link != -1) {
        return nodes_[vertex].link;
    }
    if (nodes_[vertex].parent == -1) {
        return 0;
    }
    if (nodes_[vertex].parent == 0) {
        nodes_[vertex].link =
            split(go(nodes_[vertex].left + 1, nodes_[vertex].right,
                     State(link(nodes_[vertex].parent), nodes_[link(nodes_[vertex].parent)].length())));
    } else {
        nodes_[vertex].link =
            split(go(nodes_[vertex].left, nodes_[vertex].right,
                     State(link(nodes_[vertex].parent), nodes_[link(nodes_[vertex].parent)].length())));
    }
    return nodes_[vertex].link;
}

void SuffixTree::extend_with_symbol(ll position) {
    ll middle = -1;
    while (middle) {
        State new_pointer_ = go(position, position + 1, pointer_);
        if (new_pointer_.vertex != -1) {
            pointer_ = new_pointer_;
            break;
        }
        middle = split(pointer_);
        nodes_.emplace_back(position, str_.size(), middle);
        nodes_[middle].get(str_[position]) = static_cast<ll>(nodes_.size()) - 1;
        pointer_.vertex = link(middle);
        pointer_.position = nodes_[pointer_.vertex].length();
    }
}

void SuffixTree::dfs(ll vertex, ll& counter, ll parent) {
    if (vertex != 0) {
        cout << parent << " " << nodes_[vertex].left << " " << nodes_[vertex].right << "\n";
    }
    ++counter;
    ll temp = counter;
    for (auto to : nodes_[vertex].to) {
        dfs(to.second, counter, temp);
    }
}

SuffixTree::SuffixTree(const string& s) : str_(s) {
    nodes_.emplace_back();
    for (ll i = 0; i < s.size(); ++i) {
        extend_with_symbol(i);
    }
}

void SuffixTree::show_tree() {
    cout << nodes_.size() << "\n";
    ll counter = -1;
    dfs(0, counter, 0);
}