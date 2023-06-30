#include <algorithm>
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

struct Node {
    ll link = -1;
    ll length = 0;
    map<char, ll> to;

    Node() = default;
};

class SuffixAutomaton {
public:
    explicit SuffixAutomaton(const string& s);
    string kth_substring(ll k);

private:
    ll last = 0;
    vector<ll> dp;
    vector<Node> nodes;

    void add_symbol(char c);
    ll dfs(ll v);
};

void SuffixAutomaton::add_symbol(char c) {
    ll current = nodes.size();
    nodes.push_back(Node());
    nodes[current].length = nodes[last].length + 1;
    ll parent = last;
    while (parent != -1 && !nodes[parent].to.count(c)) {
        nodes[parent].to[c] = current;
        parent = nodes[parent].link;
    }
    if (parent == -1) {
        nodes[current].link = 0;
    } else {
        ll q = nodes[parent].to[c];
        if (nodes[q].length == nodes[parent].length + 1) {
            nodes[current].link = q;
        } else {
            nodes.push_back(Node());
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

ll SuffixAutomaton::dfs(ll v) {
    if (dp[v] == 1) {
        for (auto to : nodes[v].to) {
            dp[v] += dfs(to.second);
        }
    }
    return dp[v];
}

SuffixAutomaton::SuffixAutomaton(const string& s) {
    nodes.emplace_back();
    for (char i : s) {
        add_symbol(i);
    }
    dp.assign(nodes.size(), 1);
    dfs(0);
}

string SuffixAutomaton::kth_substring(ll k) {
    string result;
    ll v = 0;
    if (dp[0] < k) {
        while (!nodes[v].to.empty()) {
            auto it = nodes[v].to.end();
            --it;
            result += it->first;
            v = it->second;
        }
    } else {
        while (true) {
            bool is_restart = false;
            if (v != 0 && k != 0)
                --k;
            for (auto to : nodes[v].to) {
                if (k == 0)
                    break;
                if (k <= dp[to.second]) {
                    v = to.second;
                    result += to.first;
                    is_restart = true;
                    break;
                } else {
                    k -= dp[to.second];
                }
            }
            if (is_restart) {
                continue;
            } else {
                break;
            }
        }
    }
    return result;
}
