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

vector<int> manacher_odd(vector<char>& s) {
    int n = s.size();
    vector<int> d(n, 1);
    int l = 0;
    int r = 0;
    for (int i = 1; i < n; i++) {
        if (i < r) {
            d[i] = std::min(r - i + 1, d[l + r - i]);
        }
        while (i - d[i] >= 0 && i + d[i] < n && s[i - d[i]] == s[i + d[i]]) {
            ++d[i];
        }
        if (i + d[i] - 1 > r) {
            l = i - d[i] + 1, r = i + d[i] - 1;
        }
    }
    return std::move(d);
}

vector<int> manacher_even(vector<char>& s) {
    int n = s.size();
    vector<int> d(n, 0);
    int l = -1;
    int r = -1;
    for (int i = 0; i < n - 1; i++) {
        if (i < r) {
            d[i] = std::min(r - i, d[l + r - i - 1]);
        }
        while (i - d[i] >= 0 && i + d[i] + 1 < n && s[i - d[i]] == s[i + d[i] + 1]) {
            ++d[i];
        }
        if (i + d[i] > r) {
            l = i - d[i] + 1, r = i + d[i];
        }
    }
    return std::move(d);
}

std::pair<vector<int>, vector<int>> manacher(vector<char>& s) {
    return {manacher_odd(s), manacher_even(s)};
}