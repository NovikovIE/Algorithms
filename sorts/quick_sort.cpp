#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

void partition(vector<long long>& numbers, int& left, int& right) {
    if (left + 1 >= right) {
        if (numbers[left] > numbers[right])
            std::swap(numbers[left], numbers[right]);
        return;
    }
    int pivot = (left + right) / 2;
    int pivot_value = numbers[pivot];
    int j = left;
    vector<long long> less;
    vector<long long> more;
    for (int i = left; i <= right; ++i) {
        if (numbers[i] < numbers[pivot]) {
            less.push_back(numbers[i]);
        } else if (numbers[i] > numbers[pivot]) {
            more.push_back(numbers[i]);
        }
    }
    for (int i = left; i < left + less.size(); ++i) {
        numbers[i] = less[i - left];
        ++j;
    }
    for (int i = j; i < right - more.size() + 1; ++i) {
        numbers[i] = pivot_value;
        ++j;
    }
    for (int i = 0; i < more.size(); ++i) {
        numbers[j + i] = more[i];
    }
    left += less.size();
    right -= more.size();
}

void quick_sort(vector<long long>& numbers, int left, int right) {
    if (left < right) {
        int tempLeft = left;
        int tempRight = right;
        partition(numbers, tempLeft, tempRight);
        quick_sort(numbers, left, tempLeft);
        quick_sort(numbers, tempRight, right);
    }
}

void quick_sort(vector<long long>& numbers) {
    quick_sort(numbers, 0, numbers.size() - 1);
}