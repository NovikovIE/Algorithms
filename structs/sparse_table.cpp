#include <cmath>
#include <vector>

using std::max;
using std::min;
using std::pair;
using std::vector;

const long long MAX_VALUE = 1'000'000'000;

class SparseTable {
public:
    explicit SparseTable(const vector<long long>& values);
    pair<long long, size_t> get_min(size_t left, size_t right) const;

private:
    void preprocessing(const vector<long long>& values, size_t height);
    vector<vector<pair<long long, size_t>>> sparse;
};

SparseTable::SparseTable(const vector<long long>& values) {
    auto height = static_cast<size_t>(std::log2(values.size()));
    sparse = vector<vector<pair<long long, size_t>>>(height + 1, vector<pair<long long, size_t>>(values.size()));
    preprocessing(values, height);
}

pair<long long, size_t> SparseTable::get_min(size_t left, size_t right) const {
    if (left > right)
        return std::make_pair(MAX_VALUE, right + 1);
    auto k = static_cast<size_t>(std::log2(right + 1 - left));
    if (sparse[k][left] < sparse[k][right - (1 << k) + 1])
        return sparse[k][left];
    else
        return sparse[k][right - (1 << k) + 1];
}

void SparseTable::preprocessing(const vector<long long>& values, size_t height) {
    for (size_t i = 0; i < values.size(); ++i) {
        sparse[0][i] = std::make_pair(values[i], i);
    }
    for (size_t k = 0; k < height; ++k) {
        size_t j = (1 << k);
        for (size_t i = 0; i + j - 1 < values.size(); ++i) {
            if (sparse[k][i] < sparse[k][j + i]) {
                sparse[k + 1][i] = sparse[k][i];
            } else {
                sparse[k + 1][i] = sparse[k][j + i];
            }
        }
    }
}