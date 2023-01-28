#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

int find_max_weight_set(int capacity, const vector<int>& weights) {
    vector<int> dp(capacity + 1);
    dp.assign(capacity + 1, 0);
    dp[0] = 1;
    for (int weight : weights) {
        for (int j = capacity; j >= weight; --j) {
            if (dp[j - weight] == 1)
                dp[j] = 1;
        }
    }
    int i = capacity;
    for (;; --i) {
        if (dp[i] > 0)
            return i;
    }
}
