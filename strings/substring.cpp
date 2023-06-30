#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

using ll = long long;

using std::cin;
using std::cout;
using std::string;
using std::vector;

vector<size_t> prefix_function(string s) {
    vector<size_t> prefix_f(s.size(), 0);
    for (size_t i = 1; i < s.size(); ++i) {
        int current = prefix_f[i - 1];
        while (s[i] != s[current] && current > 0) {
            current = prefix_f[current - 1];
        }
        if (s[i] == s[current])
            prefix_f[i] = current + 1;
    }
    return prefix_f;
}

vector<int> find_substrings(const string& text, const string& str) {
    string ts = text + "#" + str;
    vector<size_t> prefix_f = prefix_function(ts);

    vector<int> result;
    for (size_t i = 0; i < prefix_f.size(); ++i) {
        if (prefix_f[i] == text.size()) {
            result.push_back(i - 2 * text.size());
        }
    }

    return std::move(result);
}
