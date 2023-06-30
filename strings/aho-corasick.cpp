#include <cstring>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::queue;
using std::string;
using std::vector;

const size_t SIGMA = 26;

class Trie {
private:
    struct Node {
        bool term = false;
        int to[SIGMA]{};
        vector<int> lines_index_ending_here;
        int parent = -1;
        int character = -1;
        Node() { memset(to, 0, sizeof(to)); }
    };
    vector<Node> nodes_;

public:
    void add(const string& s, int index);
    vector<vector<int>> aho_corasick(const string& text, vector<vector<int>>& result);
};

void Trie::add(const string& s, int index) {
    if (nodes_.empty()) {
        nodes_.emplace_back();
    }
    int v = 0;
    for (char i : s) {
        if (nodes_[v].to[i - 'a'] == 0) {
            nodes_[v].to[i - 'a'] = static_cast<int>(nodes_.size());
            nodes_.emplace_back();
            nodes_[nodes_.size() - 1].parent = v;
            nodes_[nodes_.size() - 1].character = i - 'a';
        }
        v = nodes_[v].to[i - 'a'];
    }
    nodes_[v].lines_index_ending_here.push_back(index);
    nodes_[v].term = true;
}

vector<vector<int>> Trie::aho_corasick(const string& text, vector<vector<int>>& result) {
    vector<int> link(nodes_.size(), -1);
    queue<int> q;
    for (int& c : nodes_[0].to) {
        if (c != 0) {
            q.push(c);
        } else {
            c = 0;
        }
    }
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        if (!nodes_[v].parent || !v) {
            link[v] = 0;
        } else {
            link[v] = nodes_[link[nodes_[v].parent]].to[nodes_[v].character];
        }
        for (int c = 0; c < SIGMA; ++c) {
            if (nodes_[v].to[c] != 0) {
                q.push(nodes_[v].to[c]);
            } else {
                nodes_[v].to[c] = nodes_[link[v]].to[c];
            }
        }
    }
    int cur = 0;
    for (int i = 0; i < text.size(); ++i) {
        cur = nodes_[cur].to[text[i] - 'a'];
        int temp = cur;
        while (temp > 0) {
            if (nodes_[temp].term) {
                for (int to : nodes_[temp].lines_index_ending_here) {
                    result[to].push_back(i);
                }
            }
            temp = link[temp];
        }
    }
    return result;
}