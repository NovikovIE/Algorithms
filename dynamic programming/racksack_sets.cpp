#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

bool bit(long long mask, int pos) {
    return (mask >> pos) & 1;
}

vector<long long> find(vector<long long>& cost) {
    auto len = (long long)(pow(2, cost.size()));
    vector<long long> result(len);
    for (long long mask = 0; mask < len; ++mask) {
        for (int u = 0; u < cost.size(); ++u) {
            if (bit(mask, u)) {
                result[mask] += cost[u];
            }
        }
    }
    return result;
}

long long find_racksack_solution_ways_number() {
    long long n;
    cin >> n;
    long long len = n / 2 + n % 2;
    vector<long long> first(len);
    vector<long long> second(n - len);
    for (size_t i = 0; i < len; ++i) {
        cin >> first[i];
    }
    for (size_t i = 0; i < n - len; ++i) {
        cin >> second[i];
    }
    size_t capacity;
    cin >> capacity;
    vector<long long> result1 = find(first);
    vector<long long> result2 = find(second);
    std::sort(result1.begin(), result1.end());
    std::sort(result2.begin(), result2.end());
    long long result = 0;
    for (long long i : result1) {
        long long left = 0;
        long long right = result2.size();
        while (left < right) {
            long long middle = (left + right) >> 1;
            if (i + result2[middle] > capacity)
                right = middle;
            else
                left = middle + 1;
        }
        if (i + result2[right - 1] <= capacity)
            result += right;
    }
    return result;
}
