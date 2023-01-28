#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

uint64_t getByte(uint64_t element, int i) {
    uint64_t temp;
    temp = (element >> (8 * i)) % 256;
    return temp;
}

void radixSort(vector<uint64_t>& numbers) {
    vector<uint64_t> bytes(256);
    vector<uint64_t> temp_numbers(numbers.size());
    int count;
    int byte;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 256; ++j) {
            bytes[j] = 0;
        }
        for (int j = 0; j < numbers.size(); ++j) {
            byte = getByte(numbers[j], i);
            ++bytes[byte];
            count = 0;
        }
        for (int j = 0; j < 256; ++j) {
            uint64_t temp = bytes[j];
            bytes[j] = count;
            count += temp;
        }
        for (int j = 0; j < numbers.size(); ++j) {
            byte = getByte(numbers[j], i);
            temp_numbers[bytes[byte]++] = numbers[j];
        }
        for (int j = 0; j < numbers.size(); ++j) {
            numbers[j] = temp_numbers[j];
        }
    }
}