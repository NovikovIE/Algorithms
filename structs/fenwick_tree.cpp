// find the alternating sum on the interval from l to r inclusive (a[l]−a[l+1]+a[l+2]−…±a[r])

#include <iostream>
#include <vector>
 
using std::cin;
using std::cout;
using std::vector;
 
class FenwickTree {
public:
    FenwickTree(const vector <int>& values);
    long long get_sum(int left, int right) const {
        return sum(right) - sum(left - 1);
    }
    void change_value(int index, int value);
private:
 
    int low_border(int index) const {
        return index & (index + 1);
    }
    int up_border(int index) const {
        return index | (index + 1);
    }
    long long sum(int index) const;
    vector <int> data;
};
 
FenwickTree::FenwickTree(const vector<int>& values) : data(values.size(), 0) {
    for (size_t i = 0; i < data.size(); ++i) {
        change_value(i, (i % 2 == 0) ? (values[i]) : (-values[i]));
    }
}
 
void FenwickTree::change_value(int index, int value) {
    for (int i = index; i < data.size(); i = up_border(i)) {
        data[i] += value;
    }
}
 
long long FenwickTree::sum(int index) const {
    long long result = 0;
    for (int i = index; i >= 0; i = low_border(i) - 1) {
        result += data[i];
    }
    return result;
}