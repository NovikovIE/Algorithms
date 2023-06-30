#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cin;
using std::cout;
using std::queue;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct Node {
    int link = -1;
    int length = 0;
    unordered_map<int, int> to;
    Node() = default;
};

class SuffixAutomaton {
public:
    SuffixAutomaton() { nodes.emplace_back(); }
    void add_string(const string& s);
    void common_substrings(vector<int>& res);

private:
    int last = 0;
    vector<Node> nodes;
    int hash_counter = 0;
    vector<unordered_set<int>> dp;

    void add_symbol(int c);
    void dfs(int vertex);
};

void SuffixAutomaton::add_symbol(int c) {
    int current = nodes.size();
    nodes.emplace_back();
    nodes[current].length = nodes[last].length + 1;
    int parent = last;
    while (parent != -1 && !nodes[parent].to.count(c)) {
        nodes[parent].to[c] = current;
        parent = nodes[parent].link;
    }
    if (parent == -1) {
        nodes[current].link = 0;
    } else {
        int q = nodes[parent].to[c];
        if (nodes[q].length == nodes[parent].length + 1) {
            nodes[current].link = q;
        } else {
            nodes.emplace_back();
            nodes[nodes.size() - 1].to = nodes[q].to;
            nodes[nodes.size() - 1].length = nodes[parent].length + 1;
            nodes[nodes.size() - 1].link = nodes[q].link;
            while (parent != -1 && nodes[parent].to[c] == q) {
                nodes[parent].to[c] = nodes.size() - 1;
                parent = nodes[parent].link;
            }
            nodes[q].link = nodes[current].link = nodes.size() - 1;
        }
    }
    last = current;
}

void SuffixAutomaton::dfs(int vertex) {
    if (!dp[vertex].empty()) {
        return;
    }
    for (auto& it : nodes[vertex].to) {
        if (it.first < 256) {
            dfs(it.second);
        } else {
            dp[vertex].insert(it.first);
        }
    }
    for (auto& it : nodes[vertex].to) {
        for (auto it1 : dp[it.second]) {
            dp[vertex].insert(it1);
        }
    }
}

void SuffixAutomaton::add_string(const string& s) {
    for (char i : s) {
        add_symbol(i - 'a');
    }
    add_symbol(256 + hash_counter++);
}

void SuffixAutomaton::common_substrings(vector<int>& res) {
    dp.assign(nodes.size(), unordered_set<int>());
    dfs(0);
    for (size_t i = 1; i < nodes.size(); ++i) {
        if (dp[i].size() < 2) {
            continue;
        }
        res[dp[i].size() - 2] = std::max(res[dp[i].size() - 2], nodes[i].length);
    }
}

// 2≤i≤K L_i = the maximum length of a string that occurs as a substring in at least i strings from the initial set
