#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

int first_less_or_equal(vector<int>& dp, int key) {
    int left = 0;
    int right = dp.size();
    int middle;

    while (left < right) {
        middle = (left + right) >> 1;
        if (dp[middle] <= key)
            right = middle;
        else
            left = middle + 1;
    }
    return right;
}

vector<int> find_non_increasing_subsequence(vector<int>& a) {
    //dp[i] - the number at which a non-increasing sequence of a.size() i ends
    vector<int> dp(a.size() + 1, int(std::numeric_limits<int>::min()));
    vector<int> pos(a.size() + 1, -1);
    vector<int> index(a.size(), -1);

    int lastIndex = 0;
    dp[0] = int(std::numeric_limits<int>::max());
    pos[0] = -1;

    for (int i = 0; i < a.size(); ++i) {
        int j = first_less_or_equal(dp, a[i]);
        if (dp[j] < a[i] && dp[j - 1] >= a[i]) {
            dp[j] = a[i];
            pos[j] = i;
            index[i] = pos[j - 1];
            lastIndex = std::max(j, lastIndex);
        }
    }
    vector<int> result;
    int i = pos[lastIndex];
    while (i >= 0) {
        result.push_back(i + 1);
        i = index[i];
    }
    std::reverse(result.begin(), result.end());
    return result;
}