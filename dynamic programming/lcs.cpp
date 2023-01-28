#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;

string lcs(const string& s1, const string& s2) {
    vector<vector<bool>> getAnswer(s1.size() + 1, vector<bool>(s2.size() + 1, false));
    vector<vector<int>> dp(2, vector<int>(s2.size() + 1, 0));
    vector<int> dp1(s1.size() + 1, 0);
    vector<int> dp2(s2.size() + 1, 0);
    for (size_t i = 1; i <= s1.size(); ++i) {
        dp1 = dp2;
        for (size_t j = 1; j <= s2.size(); ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp2[j] = dp1[j - 1] + 1;
            } else if (dp1[j] < dp2[j - 1]) {
                dp2[j] = dp2[j - 1];
                getAnswer[i][j] = true;
            } else {
                dp2[j] = dp1[j];
                getAnswer[i][j] = false;
            }
        }
    }
    string answer;
    int i = s1.size();
    int j = s2.size();
    while (i > 0 && j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            answer += s2[j - 1];
            --i;
            --j;
        } else if (!getAnswer[i][j]) {
            --i;
        } else {
            --j;
        }
    }
    std::reverse(answer.begin(), answer.end());
    return answer;
}