#include <algorithm>
#include <complex>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

using ll = long long;
using vll = vector<ll>;
using complex = std::complex<double>;

const double PI = 3.14159265358979323846;
const ll MOD = 119;

void fft(vector<complex>& a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (bit <= j) {
            j -= bit;
            bit >>= 1;
        }
        j += bit;
        if (i < j) {
            swap(a[i], a[j]);
        }
    }
    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2. * PI / len * ((invert) ? (-1) : (1));
        complex w_len(cos(angle), sin(angle));
        for (int i = 0; i < n; i += len) {
            complex w = 1;
            for (int j = 0; j < len / 2; ++j) {
                complex u = a[i + j];
                complex v = a[i + j + len / 2] * w;
                a[i + j + len / 2] = u - v;
                a[i + j] = u + v;
                w *= w_len;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

vll multiply(const vll& a, const vll& b) {
    vector<complex> complex_a(a.begin(), a.end());
    vector<complex> complex_b(b.begin(), b.end());
    int n = 1;
    while (n < std::max(a.size(), b.size())) {
        n <<= 1;
    }
    n <<= 1;
    complex_a.resize(n);
    complex_b.resize(n);
    fft(complex_a, false);
    fft(complex_b, false);
    for (size_t i = 0; i < n; ++i) {
        complex_a[i] *= complex_b[i];
    }
    fft(complex_a, true);
    vll res;
    res.resize(n);
    for (size_t i = 0; i < n; ++i) {
        res[i] = (llround(complex_a[i].real()) + MOD) % MOD;
    }
    return res;
}

ll pow_by_modulo(ll number, ll power) {
    if (!power)
        return 1ll;
    else if (power & 1) {
        number *= pow_by_modulo(number, power - 1);
        number %= MOD;
        return number;
    } else {
        number = pow_by_modulo(number, power >> 1);
        number *= number;
        number %= MOD;
        return number;
    }
}

vll find_inverse(vll a, ll m) {
    ll n = 1;
    while (n < m) {
        n <<= 1;
    }
    a.resize(n);
    vll q(1, 0);
    vll p(1, 0);
    p.reserve(n);
    q.reserve(n);
    p[0] = a[0] % MOD;
    // Q0 = 1/a0
    q[0] = pow_by_modulo(a[0], MOD - 2);
    ll m_new = 1;
    while (m_new < n) {
        // calculate (- r)
        vll r = multiply(p, q);
        for (size_t i = m_new; i < r.size(); ++i) {
            r[i - m_new] = r[i];
        }
        r.resize(m_new);
        for (ll& i : r) {
            i = (-i + MOD) % MOD;
        }
        // calculate (- p1 * q)
        vll p1(p);
        for (auto i = 0; i < p1.size(); ++i) {
            p1[i] = a[m_new + i];
        }
        auto temp_p1 = multiply(p1, q);
        for (ll& i : temp_p1) {
            i = (-i + MOD) % MOD;
        }
        // calculate (- r - p1 * q)
        for (auto i = 0; i < r.size(); ++i) {
            r[i] += temp_p1[i];
            r[i] %= MOD;
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
    q.resize(m);
    return q;
}

vll divide(vll f, vll g) {
    if (f.size() < g.size()) {
        return f;
    }
    ll n = f.size() - g.size() + 1;
    reverse(f.begin(), f.end());
    reverse(g.begin(), g.end());
    auto inv_rev_g = find_inverse(g, n);
    auto q = multiply(f, inv_rev_g);
    q.resize(n);
    reverse(q.begin(), q.end());
    reverse(f.begin(), f.end());
    reverse(g.begin(), g.end());
    auto gq = multiply(g, q);
    f.resize(std::max(f.size(), gq.size()));
    gq.resize(std::max(f.size(), gq.size()));
    vll r(f.size(), 0);
    for (size_t i = 0; i < f.size(); ++i) {
        r[i] = (f[i] - gq[i] + MOD) % MOD;
    }
    r.resize(g.size() - 1);
    return r;
}

ll kitamasa(vll& coeff, vll& a, ll n) {
    vll d = {1};
    vll x_n = {0, 1};
    vll f(coeff.size() + 1);
    f.back() = 1;
    for (int i = 0; i < coeff.size(); ++i) {
        f[i] = (-coeff[i] + MOD) % MOD;
    }
    while (n) {
        if (n & 1) {
            d = divide(multiply(d, x_n), f);
        }
        n >>= 1;
        x_n = divide(multiply(x_n, x_n), f);
    }
    ll result = 0;
    for (int i = 0; i < a.size(); ++i) {
        result = (result + a[i] * d[i] + MOD) % MOD;
    }
    return result;
}

ll find_recursive_function(ll n, ll a, ll b, ll p, ll q) {
    // k≤0, f(k) = 1
    // k≥1, f(k) = a*f(k−p) + b*f(k−q)
    // f(n) % 119 - ?
    vll coeff(std::max(p, q), 0);
    coeff[coeff.size() - p] += a % MOD;
    coeff[coeff.size() - q] += b % MOD;
    vll a_vec(std::max(p, q));
    a_vec[0] = 1;
    for (ll i = 1; i < a_vec.size(); ++i) {
        ll temp1 = (i - p > 0) ? (a_vec[i - p]) : (1);
        ll temp2 = (i - q > 0) ? (a_vec[i - q]) : (1);
        a_vec[i] = (a * temp1 + b * temp2) % MOD;
    }
    if (n < a_vec.size()) {
        return a_vec[n];
    }
    return (kitamasa(coeff, a_vec, n) + MOD) % MOD;
}
