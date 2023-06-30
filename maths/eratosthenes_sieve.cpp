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

using ll = long long;

ll eratosthenes_sieve(ll n) {
    vector<bool> is_prime(n + 1, true);
    ll result = 0;
    for (ll i = 2; i <= n; ++i)
        if (is_prime[i]) {
            for (ll j = 2 * i; j <= n; j += i) {
                if (is_prime[j]) {
                    result += i;
                    is_prime[j] = false;
                }
            }
        }
    return result;
}
