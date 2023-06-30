#include <algorithm>
#include <cmath>
#include <complex>
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

const ll P = 7'340'033;
const ll root = 5;
const ll root_1 = 4'404'020;
const ll root_pw = 1ll << 20;

ll pow_by_module(ll number, ll power) {
    if (!power)
        return 1ll;
    else if (power & 1) {
        number *= pow_by_module(number, power - 1);
        number %= P;
        return number;
    } else {
        number = pow_by_module(number, power / 2);
        number *= number;
        number %= P;
        return number;
    }
}

void fft(vector<ll>& a, bool invert) {
    ll n = static_cast<ll>(a.size());
    for (ll i = 1, j = 0; i < n; ++i) {
        ll bit = n >> 1;
        for (; j >= bit; bit >>= 1) {
            j -= bit;
        }
        j += bit;
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }
    for (ll len = 2; len <= n; len <<= 1) {
        ll w_len = (invert) ? (root_1) : (root);
        for (ll i = len; i < root_pw; i <<= 1) {
            w_len = w_len * 1ll * w_len % P;
        }
        for (ll i = 0; i < n; i += len) {
            ll w = 1;
            for (ll j = 0; j < len / 2; ++j) {
                ll u = a[i + j];
                ll v = a[i + j + len / 2] * 1ll * w % P;
                a[i + j + len / 2] = (u - v >= 0) ? (u - v) : (u - v + P);
                a[i + j] = (u + v < P) ? (u + v) : (u + v - P);
                w = w * 1ll * w_len % P;
            }
        }
    }
    if (invert) {
        ll n_reverse = pow_by_module(n, P - 2);
        for (ll i = 0; i < n; ++i) {
            a[i] = a[i] * 1ll * n_reverse % P;
        }
    }
}

vector<ll> multiply(vector<ll> a, vector<ll> b) {
    ll n = 1;
    while (n < std::max(a.size(), b.size())) {
        n <<= 1;
    }
    n <<= 1;
    a.resize(n);
    b.resize(n);
    fft(a, false);
    fft(b, false);
    for (size_t i = 0; i < n; ++i) {
        a[i] *= b[i];
        a[i] %= P;
    }
    fft(a, true);
    return a;
}

vector<ll> divide(vector<ll>& a, ll m) {
    // Q0 = 1/a0
    if (!a[0]) {
        // can not divide
        exit(0);
    }
    ll n = 1;
    while (n < m) {
        n <<= 1;
    }
    a.resize(n);
    vector<ll> q(1, 0);
    vector<ll> p(1, 0);
    p.reserve(n);
    q.reserve(n);
    p[0] = a[0];
    q[0] = pow_by_module(a[0], P - 2);
    ll m_new = 1;
    while (m_new < n) {
        // calculate (- r)
        vector<ll> r = multiply(p, q);
        for (size_t i = m_new; i < r.size(); ++i) {
            r[i - m_new] = r[i];
        }
        r.resize(m_new);
        for (ll& i : r) {
            i = (-i + P) % P;
        }
        // calculate (- p1 * q)
        vector<ll> p1(p);
        for (auto i = 0; i < p1.size(); ++i) {
            p1[i] = a[m_new + i];
        }
        auto temp_p1 = multiply(p1, q);
        for (ll& i : temp_p1) {
            i = (-i + P) % P;
        }
        // calculate (- r - p1 * q)
        for (auto i = 0; i < r.size(); ++i) {
            r[i] += temp_p1[i];
            r[i] %= P;
        }
        // t = q * (- r - p1 * q)
        auto t = multiply(q, r);
        p.resize(m_new << 1);
        q.resize(m_new << 1);
        for (auto i = 0; i < m_new; ++i) {
            p[i + m_new] = p1[i];
            q[i + m_new] = t[i];
        }
        m_new <<= 1;
    }
    return q;
}
