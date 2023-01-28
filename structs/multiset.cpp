#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::map;
using std::set;
using std::string;
using std::vector;

class MultiSet {
public:
    MultiSet() = default;
    ~MultiSet() = default;
    void add(uint64_t number, int set_number);
    void erase(uint64_t number, int set_number);
    void clear(int set_number);
    // Show contents of set s (0≤s≤m) in ascending order, or −1 if the set is empty
    void list_set(int set_number);
    // Prints the numbers of sets containing the number e (0≤e≤n) in ascending order,
    // or −1 if this number is not in any set.
    void list_set_of(uint64_t number);

private:
    map<uint64_t, set<int>> set_entering;
    map<int, set<uint64_t>> sets;
};

void MultiSet::add(uint64_t number, int set_number) {
    sets[set_number].insert(number);
    set_entering[number].insert(set_number);
}

void MultiSet::erase(uint64_t number, int set_number) {
    sets[set_number].erase(number);
    set_entering[number].erase(set_number);
}

void MultiSet::clear(int set_number) {
    for (auto it : sets[set_number]) {
        set_entering[it].erase(set_number);
    }
    sets[set_number].clear();
}

void MultiSet::list_set(int set_number) {
    if (sets[set_number].empty()) {
        cout << -1 << "\n";
        return;
    }
    for (auto it : sets[set_number]) {
        cout << it << " ";
    }
    cout << "\n";
}

void MultiSet::list_set_of(uint64_t number) {
    if (set_entering[number].empty()) {
        cout << -1 << "\n";
        return;
    }
    for (int it : set_entering[number]) {
        cout << it + 1 << " ";
    }
    cout << "\n";
}
