#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using std::cin;
using std::cout;
using std::numeric_limits;
using std::pair;
using std::set;
using std::string;
using std::unordered_map;
using std::vector;

// it's a number that we get interpreting permutation 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0
// as binary numbers from 0 to 1111 and adding them to right bytes by shifting by 4 * i
static const uint64_t finish = 1147797409030816545;

int get_number(uint64_t number, uint64_t pos) {
    return static_cast<int>((number >> (pos * 4)) & 15);
}

uint64_t manhattan_distance(uint64_t permutation) {
    uint64_t result = 0;
    for (int i = 0; i < 16; ++i) {
        int byte = get_number(permutation, i);
        if (byte != 0) {
            result += abs(i / 4 - (byte - 1) / 4) + abs(i % 4 - (byte - 1) % 4);
        }
    }
    return result;
}

uint64_t linear_conflict(uint64_t permutation) {
    vector<size_t> linear_conflicts(16, 0);
    for (size_t i = 0; i < 4; ++i) {
        vector<int> temp(4);
        vector<bool> is_on_their_linear_place(4);
        for (size_t j = 0; j < 4; ++j) {
            temp[j] = get_number(permutation, i * 4 + j);
            if (temp[j] == 0) {
                is_on_their_linear_place[j] = false;
            } else {
                is_on_their_linear_place[j] = (i == (temp[j] - 1) / 4);
            }
        }
        for (size_t j = 0; j < 4; ++j) {
            if (is_on_their_linear_place[j]) {
                for (size_t k = j + 1; k < 4; ++k) {
                    if (is_on_their_linear_place[k] && temp[k] < temp[j]) {
                        linear_conflicts[i * 4 + j] += 1;
                        linear_conflicts[i * 4 + k] += 1;
                    }
                }
            }
        }
    }
    for (int i = 0; i < 4; ++i) {
        vector<int> temp(4);
        vector<bool> is_on_their_column_place(4);
        for (size_t j = 0; j < 4; ++j) {
            temp[j] = get_number(permutation, i + j * 4);
            if (temp[j] == 0) {
                is_on_their_column_place[j] = false;
            } else {
                is_on_their_column_place[j] = (i == (temp[j] - 1) % 4);
            }
        }
        for (size_t j = 0; j < 4; ++j) {
            if (is_on_their_column_place[j]) {
                for (size_t k = j + 1; k < 4; ++k) {
                    if (is_on_their_column_place[k] && temp[k] < temp[j]) {
                        linear_conflicts[i + j * 4] += 1;
                        linear_conflicts[i + k * 4] += 1;
                    }
                }
            }
        }
    }
    if (get_number(permutation, 15) == 0 && (get_number(permutation, 11) == 15 || get_number(permutation, 14) == 12)) {
        linear_conflicts[15] += 2;
    }
    uint64_t result = 0;
    for (size_t i = 0; i < 16; ++i) {
        result += linear_conflicts[i];
    }
    return result;
}

uint64_t heuristics(uint64_t permutation) {
    return 112 * (manhattan_distance(permutation) + linear_conflict(permutation)) / 100;
}

bool is_unsolvable(uint64_t permutation) {
    vector<uint64_t> numbers(16);
    for (uint64_t i = 0; i < 16; ++i) {
        numbers[i] = get_number(permutation, i);
    }
    uint64_t permutations = 0;
    for (uint64_t i = 0; i < 16; ++i) {
        if (numbers[i] == 0) {
            permutations += (i / 4) + 1;
            continue;
        }
        for (uint64_t j = i + 1; j < 16; ++j) {
            if (numbers[j] != 0 && numbers[j] < numbers[i]) {
                ++permutations;
            }
        }
    }
    return (permutations) % 2 == 1;
}

void get_neighbour(uint64_t permutation, uint64_t pos_of_zero, vector<uint64_t>& neighbors, int pos_change) {
    uint64_t temp = permutation;
    uint64_t byte = get_number(permutation, pos_of_zero + pos_change);
    temp -= (byte << (4 * (pos_of_zero + pos_change)));
    temp += (byte << (4 * pos_of_zero));
    neighbors.push_back(temp);
}

vector<uint64_t> find_neighbours(uint64_t permutation) {
    uint64_t pos_of_zero = 0;
    for (uint64_t i = 0; i < 16; ++i) {
        uint64_t byte = get_number(permutation, i);
        if (byte == 0) {
            pos_of_zero = i;
            break;
        }
    }
    vector<uint64_t> neighbors;
    if (pos_of_zero / 4 > 0) {
        get_neighbour(permutation, pos_of_zero, neighbors, -4);
    } else {
        neighbors.push_back(numeric_limits<uint64_t>::max());
    }
    if (pos_of_zero % 4 > 0) {
        get_neighbour(permutation, pos_of_zero, neighbors, -1);
    } else {
        neighbors.push_back(numeric_limits<uint64_t>::max());
    }

    if (pos_of_zero / 4 < 3) {
        get_neighbour(permutation, pos_of_zero, neighbors, 4);
    } else {
        neighbors.push_back(numeric_limits<uint64_t>::max());
    }

    if (pos_of_zero % 4 < 3) {
        get_neighbour(permutation, pos_of_zero, neighbors, 1);
    } else {
        neighbors.push_back(numeric_limits<uint64_t>::max());
    }

    return neighbors;
}

void find_path(uint64_t start, unordered_map<uint64_t, pair<uint64_t, char>>& parent) {
    unordered_map<uint64_t, uint64_t> cost, final_cost;
    set<pair<uint64_t, uint64_t>> current_paths;
    cost[start] = 0;
    final_cost[start] = cost[start] + heuristics(start);
    current_paths.insert({final_cost[start], start});
    while (!current_paths.empty()) {
        uint64_t current = current_paths.begin()->second;
        current_paths.erase({final_cost[current], current});
        if (current == finish) {
            return;
        }
        vector<uint64_t> neighbours = find_neighbours(current);
        int counter = -1;
        for (auto v : neighbours) {
            ++counter;
            if (v == numeric_limits<uint64_t>::max()) {
                continue;
            }
            auto it = cost.find(v);
            if (it == cost.end() || it->second > cost[current] + 1) {
                current_paths.erase({final_cost[v], v});
                cost[v] = cost[current] + 1;
                final_cost[v] = cost[v] + heuristics(v);
                if (counter == 0) {
                    parent[v] = {current, 'D'};
                } else if (counter == 1) {
                    parent[v] = {current, 'R'};
                } else if (counter == 2) {
                    parent[v] = {current, 'U'};
                } else if (counter == 3) {
                    parent[v] = {current, 'L'};
                }
                current_paths.insert({final_cost[v], v});
            }
        }
    }
}

pair<int, vector<char>> get_path(const vector<uint64_t>& vec) {
    uint64_t start = 0;
    for (uint64_t i = 0; i < vec.size(); ++i) {
        start += (vec[i] << (4 * i));
    }
    vector<char> path;
    if (is_unsolvable(start)) {
        return {-1, path};
    }
    unordered_map<uint64_t, pair<uint64_t, char>> parent;
    find_path(start, parent);
    uint64_t fin = finish;
    while (fin != start) {
        path.push_back(parent[fin].second);
        fin = parent[fin].first;
    }
    std::reverse(path.begin(), path.end());
    return {path.size(), path};
}