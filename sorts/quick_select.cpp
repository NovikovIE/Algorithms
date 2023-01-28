#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::swap;
using std::vector;

long long partition(vector<long long>& numbers, long long left, long long right, long long pivot) {
    long long j = left;
    long long temp_pivot = numbers[pivot];
    swap(numbers[pivot], numbers[right]);
    for (long long i = left; i < right; ++i) {
        if (numbers[i] <= temp_pivot) {
            swap(numbers[i], numbers[j]);
            ++j;
        }
    }
    swap(numbers[j], numbers[right]);
    return j;
}

long long quickSelect(vector<long long>& numbers, long long left, long long right, long long k) {
    if (left == right)
        return numbers[right];
    long long pivot = (left + right) / 2;
    pivot = partition(numbers, left, right, pivot);
    if (k == pivot) {
        return numbers[pivot];
    } else if (k > pivot) {
        return quickSelect(numbers, pivot + 1, right, k);
    } else {
        return quickSelect(numbers, left, pivot - 1, k);
    }
}