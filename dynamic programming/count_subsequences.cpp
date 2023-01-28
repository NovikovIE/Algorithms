#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

const int MOD = 1'000'000'007;
const int MAX_LAST_SIZE = 1'000'000;

int count_subsequences(const vector<int>& data) {
    vector<int> last(MAX_LAST_SIZE, -1);
    vector<int> dp(data.size() + 1, 1);
    for (size_t i = 1; i <= data.size(); ++i) {
        if (last[data[i - 1]] != -1) {
            dp[i] = 2 * dp[i - 1] - dp[last[data[i - 1]]];
            if (dp[i] < 0)
                dp[i] += MOD;
            dp[i] %= MOD;
        } else {
            dp[i] = 2 * dp[i - 1] % MOD;
        }
        last[data[i - 1]] = i - 1;
    }
    return dp[data.size()] - 1;
}
