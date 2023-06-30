#include <iostream>
#include <vector>

using ll = long long;

using std::cin;
using std::cout;
using std::string;
using std::vector;

const ll eps = 256;

void build_suffix_array(const string& s, vector<ll>& p) {
    ll n = s.size();
    vector<ll> count(eps, 0);
    vector<ll> classes(n);
    for (ll i = 0; i < n; ++i) {
        ++count[s[i]];
    }
    for (ll i = 1; i < eps; ++i) {
        count[i] += count[i - 1];
    }
    for (ll i = 0; i < n; ++i) {
        p[--count[s[i]]] = i;
    }
    classes[p[0]] = 0;

    ll classes_count = 1;
    for (ll i = 1; i < n; ++i) {
        if (s[p[i]] != s[p[i - 1]]) {
            ++classes_count;
        }
        classes[p[i]] = classes_count - 1;
    }

    vector<ll> pn(n);
    vector<ll> classes_n(n);
    for (ll h = 0; (1ll << h) < n; ++h) {
        for (ll i = 0; i < n; ++i) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0) {
                pn[i] += n;
            }
        }
        count.assign(classes_count, 0);
        for (ll i = 0; i < n; ++i) {
            ++count[classes[pn[i]]];
        }
        for (ll i = 1; i < classes_count; ++i) {
            count[i] += count[i - 1];
        }
        for (ll i = n - 1; i >= 0; --i) {
            p[--count[classes[pn[i]]]] = pn[i];
        }
        classes_n[p[0]] = 0;
        classes_count = 1;
        for (ll i = 1; i < n; ++i) {
            if (classes[p[i]] != classes[p[i - 1]] ||
                classes[(p[i] + (1ll << h)) % n] != classes[(p[i - 1] + (1ll << h)) % n]) {
                ++classes_count;
            }
            classes_n[p[i]] = classes_count - 1;
        }
        classes = classes_n;
    }
}

void kasai(const string& s, const vector<ll>& sa, vector<ll>& lcp) {
    int n = s.length();
    std::vector<int> rank(n);
    for (ll i = 0; i < n; ++i) {
        rank[sa[i]] = i;
    }
    for (ll i = 0, k = 0; i < n; ++i) {
        if (k > 0) {
            --k;
        }
        if (rank[i] == n - 1) {
            lcp[n - 1] = -1;
            k = 0;
            continue;
        }
        ll j = sa[rank[i] + 1];
        while (std::max(i + k, j + k) < s.length() && s[i + k] == s[j + k]) {
            ++k;
        }
        lcp[rank[i]] = k;
    }
}

//    vector<ll> p(s.size());
//    vector<ll> lcp(s.size());
//    build_suffix_array(s, p);
//    kasai(s, p, lcp);