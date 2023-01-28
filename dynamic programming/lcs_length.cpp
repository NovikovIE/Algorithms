#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::max;
using std::vector;

// finds length of lcs
int get_largest_common_subsequence(const vector<int>& sequence1, const vector<int>& sequence2) {
    // dp[i][j] - solution for prefixes of given sequences,
    // ending in elements with numbers i and j
    vector<vector<int>> dp(sequence1.size(), vector<int>(sequence2.size()));
    dp[0][0] = 0;
    for (size_t i = 1; i < sequence1.size(); ++i) {
        for (size_t j = 1; j < sequence2.size(); ++j) {
            if (sequence1[i - 1] == sequence2[j - 1])
                dp[i][j] = max(dp[i - 1][j], max(dp[i][j - 1], dp[i - 1][j - 1] + 1));
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
}