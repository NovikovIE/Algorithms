#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::swap;
using std::vector;

class Heap {
public:
    explicit Heap(size_t q) { pointer.assign(q + 1, 0); }

    long long getMin() { return values[0]; }
    void decreaseKey(size_t index, long long delta);
    void extractMin();
    void insert(long long value, size_t elementId);

private:
    vector<long long> values;
    vector<size_t> index_to_pointer;
    vector<size_t> pointer;

    void exchange(size_t index1, size_t index2);
    void siftUp(size_t index);
    void siftDown(size_t index);
};

void Heap::decreaseKey(size_t index, long long int delta) {
    values[pointer[index]] -= delta;
    siftUp(pointer[index]);
}

void Heap::extractMin() {
    exchange(0, values.size() - 1);
    index_to_pointer.pop_back();
    values.pop_back();
    siftDown(0);
}

void Heap::insert(long long int value, size_t elementId) {
    values.push_back(value);
    index_to_pointer.push_back(elementId);
    pointer[elementId] = static_cast<size_t>(values.size() - 1);
    siftUp(values.size() - 1);
}

void Heap::exchange(size_t index1, size_t index2) {
    size_t k = index_to_pointer[index1];
    size_t m = index_to_pointer[index2];
    swap(index_to_pointer[index1], index_to_pointer[index2]);
    swap(pointer[k], pointer[m]);
    swap(values[index1], values[index2]);
}

void Heap::siftUp(size_t index) {
    while (index != 0) {
        if (values[index] < values[(index - 1) / 2]) {
            exchange(index, (index - 1) / 2);
            index = (index - 1) / 2;
        } else {
            break;
        }
    }
}

void Heap::siftDown(size_t index) {
    size_t temp_index;
    while (2 * index + 1 < values.size()) {
        temp_index = 2 * index + 1;
        if (values[index] < values[temp_index] &&
            (temp_index >= values.size() - 1 || values[index] < values[temp_index + 1])) {
            break;
        } else if (temp_index + 1 < values.size()) {
            if (values[temp_index + 1] < values[temp_index] && temp_index + 1 < values.size()) {
                temp_index = temp_index + 1;
            }
        }
        exchange(temp_index, index);
        index = temp_index;
    }
}
