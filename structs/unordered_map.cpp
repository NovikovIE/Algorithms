#include <functional>
#include <memory>
#include <vector>
#include <limits>
#include <iostream>

using std::equal_to;
using std::allocator;
using std::pair;
using std::vector;
using std::allocator_traits;

template<typename T, typename Allocator = std::allocator<T>>
class List {
private:
    struct Node {
        T* value = nullptr;
        Node* prev = nullptr;
        Node* next = nullptr;

        Node() = default;

        Node(Allocator& alloc, T* ptr);

        Node(Allocator& alloc, T* ptr, const T& obj);

        Node(Allocator& alloc, T* ptr, T&& obj);

        Node(T* ptr);

        Node(const Node&) = default;

        Node& operator=(const Node&) = default;

        void deletion(Allocator& alloc);

        ~Node() = default;
    };

    Node* create_new_node_back(T* ptr);

    Node* create_new_node_back(T* ptr, const T& obj);

    Node* create_new_node_front(T* ptr, const T& obj);

    void initialize_corner();

    void deletion();

    Allocator allocator;
    Node* corner = nullptr;
    size_t size_ = 0;
public:
    explicit List(Allocator& alloc) : allocator(alloc) {
        initialize_corner();
    }

    List() : allocator(Allocator()) {
        initialize_corner();
    }

    explicit List(size_t n);

    List(size_t n, const T& object);

    List(size_t n, Allocator& alloc);

    List(size_t n, const T& object, Allocator& alloc);

    size_t size() const {
        return size_;
    }

    void swap(List<T, Allocator>& list);

    List& operator=(const List<T, Allocator>& list);

    List& operator=(List<T, Allocator>&& list) noexcept;

    List(const List& list);

    void clear();

    List(List&& list) noexcept;

    ~List() {
        deletion();
    }

    template<typename Type>
    class Iterator {
    private:
        Node* position;
    public:
        friend List<T, Allocator>;

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Type;
        using pointer = Type*;
        using reference = Type&;

        Iterator() = default;

        explicit Iterator(Node* pointer) : position(pointer) {}

        ~Iterator() = default;

        void swap(Iterator& iter) {
            std::swap(position, iter.position);
        }

        Iterator& operator=(Iterator<Type> iter);;

        Iterator(const Iterator& iter) = default;

        template<typename Another_Type>
        explicit Iterator(const Iterator<Another_Type>& iter) : position(iter.position) {}

        reference operator*() {
            return *(position->value);
        }

        value_type operator*() const {
            return *(position->value);
        }

        pointer operator->() {
            return position->value;
        }

        Iterator& operator++();

        Iterator operator++(int);

        Iterator& operator--();

        Iterator operator--(int);

        bool operator==(const Iterator& iter) const {
            return position == iter.position;
        }

        bool operator!=(const Iterator& iter) const {
            return position != iter.position;
        }

        bool operator<(const Iterator& iter) const {
            return position < iter.position;
        }

        bool operator>(const Iterator& iter) const {
            return iter < *this;
        }

        bool operator<=(const Iterator& iter) const {
            return !(*this > iter);
        }

        bool operator>=(const Iterator& iter) const {
            return !(*this < iter);
        }

        difference_type operator-(const Iterator& iter) const {
            return position - iter.position;
        }

        Node* next() {
            return position->next;
        }

        Node* prev() {
            return position->prev;
        }
    };

    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(corner->next);
    }

    iterator end() {
        return iterator(corner);
    }

    const_iterator cbegin() const {
        return const_iterator(corner->next);
    }

    const_iterator cend() const {
        return const_iterator(corner);
    }

    iterator begin() const {
        return iterator(corner->next);
    }

    iterator end() const {
        return iterator(corner);
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    reverse_iterator rbegin() const {
        return reverse_iterator(end());
    }

    reverse_iterator rend() const {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    void insert(iterator iter, const T& object);

    void insert(const_iterator iter, const T& object);

    template<typename... Args>
    void emplace_back(Args&& ...args);

    void erase(const_iterator iter);

    void erase(iterator iter);
};

template<typename T, typename Allocator>
template<typename Type>
typename List<T, Allocator>::template Iterator<Type>& List<T, Allocator>::Iterator<Type>::operator=(List::Iterator<Type> iter) {
    swap(iter);
    return *this;
}

template<typename T, typename Allocator>
template<typename Type>
typename List<T, Allocator>::template Iterator<Type>& List<T, Allocator>::Iterator<Type>::operator++() {
    position = position->next;
    return *this;
}

template<typename T, typename Allocator>
template<typename Type>
typename List<T, Allocator>::template Iterator<Type> List<T, Allocator>::Iterator<Type>::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T, typename Allocator>
template<typename Type>
typename List<T, Allocator>::template Iterator<Type>& List<T, Allocator>::Iterator<Type>::operator--() {
    position = position->prev;
    return *this;
}

template<typename T, typename Allocator>
template<typename Type>
typename List<T, Allocator>::template Iterator<Type> List<T, Allocator>::Iterator<Type>::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::create_new_node_back(T* ptr) {
    try {
        Node* node = new Node(allocator, ptr);
        node->next = corner;
        corner->prev = node;
        return node;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::create_new_node_back(T* ptr, const T& obj) {
    try {
        Node* node = new Node(allocator, ptr, obj);
        node->next = corner;
        corner->prev = node;
        return node;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
typename List<T, Allocator>::Node* List<T, Allocator>::create_new_node_front(T* ptr, const T& obj) {
    try {
        Node* node = new Node(allocator, ptr, obj);
        node->prev = corner;
        corner->next = node;
        return node;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::initialize_corner() {
    try {
        corner = new Node();
        corner->next = corner;
        corner->prev = corner;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::deletion() {
    if (corner) {
        Node* temp = corner;
        while (temp->next != corner) {
            if (temp != corner) temp->deletion(allocator);
            Node* temp2 = temp->next;
            delete temp;
            temp = temp2;
        }
        temp->deletion(allocator);
        delete temp;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t n) : allocator(Allocator()), size_(n) {
    try {
        initialize_corner();
        Node* prev = nullptr;
        for (size_t i = 0; i < n; ++i) {
            T* allocated = allocator_traits<Allocator>::allocate(allocator, 1);
            Node* node = create_new_node_back(allocated);
            if (prev) {
                node->prev = prev;
                prev->next = node;
            } else {
                node->prev = corner;
                corner->next = node;
            }
            prev = node;
        }
    } catch (...) {
        deletion();
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t n, const T& object) : allocator(Allocator()), size_(n) {
    try {
        initialize_corner();
        Node* prev = corner;
        for (size_t i = 0; i < n; ++i) {
            T* allocated = allocator_traits<Allocator>::allocate(allocator, 1);
            Node* node = create_new_node_back(allocated, object);
            if (prev) {
                node->prev = prev;
                prev->next = node;
            } else {
                node->prev = corner;
                corner->next = node;
            }
            prev = node;
        }
    } catch (...) {
        deletion();
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t n, Allocator& alloc) : allocator(alloc), size_(n) {
    try {
        initialize_corner();
        Node* prev = corner;
        for (size_t i = 0; i < n; ++i) {
            T* allocated = allocator_traits<Allocator>::allocate(allocator, 1);
            Node* node = create_new_node_back(allocated);
            if (prev) {
                node->prev = prev;
                prev->next = node;
            }
            prev = node;
        }
    } catch (...) {
        deletion();
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t n, const T& object, Allocator& alloc) : allocator(alloc), size_(n) {
    try {
        initialize_corner();
        Node* prev = corner;
        for (size_t i = 0; i < n; ++i) {
            T* allocated = allocator_traits<Allocator>::allocate(allocator, 1);
            Node* node = create_new_node_back(allocated, object);
            if (prev) {
                node->prev = prev;
                prev->next = node;
            } else {
                node->prev = corner;
                corner->next = node;
            }
            prev = node;
        }
    } catch (...) {
        deletion();
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::swap(List<T, Allocator>& list) {
    std::swap(allocator, list.allocator);
    std::swap(corner, list.corner);
    std::swap(size_, list.size_);
}

template<typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List<T, Allocator>& list) {
    List<T, Allocator> temp_list(list);
    if (allocator_traits<Allocator>::propagate_on_container_copy_assignment::value)
        temp_list.allocator = list.allocator;
    try {
        swap(temp_list);
        return *this;
    } catch (...) {
        swap(temp_list);
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(List<T, Allocator>&& list) noexcept {
    deletion();
    if (allocator_traits<Allocator>::propagate_on_container_move_assignment::value)
        allocator = std::move(list.allocator);
    corner = std::move(list.corner);
    size_ = std::move(list.size_);
    list.initialize_corner();
    return *this;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List& list) : allocator(
        allocator_traits<Allocator>::select_on_container_copy_construction(list.allocator)) {
    try {
        initialize_corner();
        size_ = list.size_;
        if (size_) {
            Node* temp1 = list.corner;
            Node* prev = nullptr;
            while (temp1->next != list.corner && temp1->next != nullptr) {
                T* allocated = allocator_traits<Allocator>::allocate(allocator, 1);
                Node* node = create_new_node_back(allocated, *temp1->next->value);
                if (prev) {
                    node->prev = prev;
                    prev->next = node;
                } else {
                    node->prev = corner;
                    corner->next = node;
                }
                temp1 = temp1->next;
                prev = node;
            }
        }
    } catch (...) {
        deletion();
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::clear() {
    initialize_corner();
    size_ = 0;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(List&& list) noexcept: allocator(std::move(list.allocator)) {
    corner = std::move(list.corner);
    size_ = std::move(list.size_);
}

template<typename T, typename Allocator>
void List<T, Allocator>::insert(List::const_iterator iter, const T& object) {
    try {
        insert(iterator(iter), object);
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
template<typename... Args>
void List<T, Allocator>::emplace_back(Args&& ... args) {
    try {
        ++size_;
        T* ptr = allocator_traits<Allocator>::allocate(allocator, 1);
        allocator_traits<Allocator>::construct(allocator, ptr, std::forward<Args>(args)...);
        Node* node = new Node(ptr);
        Node* temp = nullptr;
        if (corner->prev) temp = corner->prev;
        node->next = corner;
        corner->prev = node;
        if (temp) {
            node->prev = temp;
            temp->next = node;
        }
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::erase(List::iterator iter) {
    try {
        --size_;
        if (iter.position == corner) return;
        auto prev = iter.prev();
        auto next = iter.next();
        prev->next = next;
        next->prev = prev;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::erase(List::const_iterator iter) {
    try {
        erase(iterator(iter));
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::insert(List::iterator iter, const T& object) {
    try {
        ++size_;
        T* mem = allocator_traits<Allocator>::allocate(allocator, 1);
        Node* node = new Node(allocator, mem, object);
        Node* prev = iter.prev();
        Node* next = iter.position;
        prev->next = node;
        next->prev = node;
        node->prev = prev;
        node->next = next;
        iter.position = node;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::Node::Node(Allocator& alloc, T* ptr) {
    try {
        value = ptr;
        allocator_traits<Allocator>::construct(alloc, value);
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::Node::Node(Allocator& alloc, T* ptr, const T& obj) {
    try {
        value = ptr;
        allocator_traits<Allocator>::construct(alloc, value, obj);
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::Node::Node(Allocator& alloc, T* ptr, T&& obj) {
    try {
        value = ptr;
        allocator_traits<Allocator>::construct(alloc, value, std::move(obj));
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
List<T, Allocator>::Node::Node(T* ptr) {
    try {
        value = ptr;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::Node::deletion(Allocator& alloc) {
    if (value != nullptr) allocator_traits<Allocator>::destroy(alloc, value);
    if (value) allocator_traits<Allocator>::deallocate(alloc, value, 1);
    value = nullptr;
}

template<typename Key, typename Value, typename Hash = std::hash<Key>, typename Equal = equal_to<Key>,
        typename Alloc = allocator<pair<const Key, Value>>>
class UnorderedMap {
public:
    using NodeType = pair<const Key, Value>;

    using inner_iterator = typename List<NodeType, Alloc>::iterator;
private:
    const double load_factor_max = 0.95;
    vector<inner_iterator> ptr;
    List<NodeType, Alloc> data;

    Alloc alloc = Alloc();
    Hash hash = Hash();
    Equal equal = Equal();

    void rehash(size_t new_size);

public:
    UnorderedMap();

    UnorderedMap(const UnorderedMap& map);

    UnorderedMap(UnorderedMap&& map) noexcept;

    UnorderedMap& operator=(const UnorderedMap& map);

    UnorderedMap& operator=(UnorderedMap&& map) noexcept;

    ~UnorderedMap() = default;

    class iterator : public List<NodeType, Alloc>::iterator {
    public:
        iterator(const typename List<NodeType, Alloc>::iterator& iter) :
                List<NodeType, Alloc>::iterator(iter) {}
    };

    class const_iterator : public List<NodeType, Alloc>::const_iterator {
    public:
        const_iterator(const typename List<NodeType, Alloc>::const_iterator& iter) :
                List<NodeType, Alloc>::const_iterator(iter) {}

        const_iterator(const iterator& iter) :
                List<NodeType, Alloc>::const_iterator(iter) {}
    };

    iterator begin() {
        return iterator(data.begin());
    }

    iterator end() {
        return iterator(data.end());
    }

    const_iterator cbegin() const {
        return const_iterator(data.cbegin());
    }

    const_iterator cend() const {
        return const_iterator(data.cend());
    }

    iterator begin() const {
        return iterator(data.begin());
    }

    iterator end() const {
        return iterator(data.end());
    }

    iterator find(const Key& key);

    template<typename... Args>
    pair<iterator, bool> emplace(Args&& ... args);

    pair<iterator, bool> insert(const NodeType& node_type);

    pair<iterator, bool> insert(NodeType&& node_type);

    template<typename Iter>
    void insert(Iter iter1, Iter iter2);

    Value& operator[](const Key& key);

    Value& at(const Key& key);

    void erase(iterator it);

    void erase(iterator it1, iterator it2);

    size_t size() const noexcept {
        return data.size();
    }

    size_t max_size() const noexcept {
        return std::numeric_limits<size_t>::max();
    }

    double load_factor() const noexcept {
        return static_cast<double>(data.size() / ptr.size());
    }

    double max_load_factor() const noexcept {
        return load_factor_max;
    }

    void reserve(size_t new_size);
};

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::rehash(size_t new_size) {
    try {
        List<NodeType, Alloc> new_list = std::move(data);
        data.clear();
        ptr.clear();
        ptr.assign(new_size, data.end());
        for (auto it = new_list.begin(); it != new_list.end(); ++it) {
            insert(std::move(*it));
        }
    } catch(...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap() : ptr(vector<inner_iterator>(1)) {
    ptr[0] = data.end();
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(const UnorderedMap& map) : ptr(map.ptr), data(map.data) {
    try {
        for (auto it = ptr.begin(); it != ptr.end(); ++it) {
            if (*it == map.data.end()) *it = data.end();
        }
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (ptr[hash(it->first) % ptr.size()] != data.end())
                ptr[hash(it->first) % ptr.size()] = it;
        }
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(UnorderedMap&& map) noexcept {
    ptr = std::move(map.ptr);
    data = std::move(map.data);
    for (auto it = ptr.begin(); it != ptr.end(); ++it) {
        if (*it == map.data.end()) *it = data.end();
    }
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (ptr[hash(it->first) % ptr.size()] != data.end())
            ptr[hash(it->first) % ptr.size()] = it;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>& UnorderedMap<Key, Value, Hash, Equal, Alloc>
        ::operator=(const UnorderedMap& map) {
    try {
        if (this == &map) return *this;
        alloc = map.alloc;
        equal = map.equal;
        hash = map.hash;
        data = List<NodeType, Alloc>();
        ptr.assign(map.size(), data.end());
        insert(map.begin(), map.end());
        return *this;
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>& UnorderedMap<Key, Value, Hash, Equal, Alloc>
        ::operator=(UnorderedMap&& map) noexcept {
    if (this == &map) return *this;
    ptr = std::move(map.ptr);
    data = std::move(map.data);
    alloc = std::move(map.alloc);
    equal = std::move(map.equal);
    hash = std::move(map.hash);
    for (auto it = ptr.begin(); it != ptr.end(); ++it) {
        if (*it == map.data.end()) *it = data.end();
    }
    return *this;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator UnorderedMap<Key, Value, Hash, Equal, Alloc>
        ::find(const Key& key) {
    try {
        size_t hsh = hash(key) % ptr.size();
        auto it = ptr[hsh];
        while (it != data.end() && hsh == hash(it->first) % ptr.size()) {
            if (equal(it->first, key)) return it;
            ++it;
        }
        return end();
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template<typename... Args>
pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::emplace(Args&& ... args) {
    try {
        if (load_factor() > max_load_factor()) rehash(ptr.size() * 2);

        NodeType* node = allocator_traits<Alloc>::allocate(alloc, 1);
        allocator_traits<Alloc>::construct(alloc, node, std::forward<Args>(args)...);

        size_t hsh = hash(node->first) % ptr.size();
        auto it = find(node->first);
        if (it == end()) {
            data.emplace_back(std::move(const_cast<Key&>(node->first)), std::move(node->second));
            --ptr[hsh];
            return {ptr[hsh], true};
        }
        return {it, false};
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(const UnorderedMap::NodeType& node_type) {
    try {
        return emplace(node_type);
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(UnorderedMap::NodeType&& node_type) {
    try {
        size_t hsh = hash(node_type.first) % ptr.size();
        auto it = find(node_type.first);
        if (it == end()) {
            data.emplace_back(std::move(const_cast<Key&>(node_type.first)), std::move(node_type.second));
            --ptr[hsh];
            return {ptr[hsh], true};
        }
        return {it, false};
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template<typename Iter>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(Iter iter1, Iter iter2) {
    try {
        while (iter1 != iter2) {
            emplace(*iter1);
            ++iter1;
        }
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value& UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator[](const Key& key) {
    try {
        if (load_factor() > max_load_factor()) rehash(ptr.size() * 2);
        auto it = find(key);
        size_t hsh = hash(key) % ptr.size();

        if (it == end()) {
            data.insert(ptr[hsh], {key, Value()});
            --ptr[hsh];
            return ptr[hsh]->second;
        }
        return it->second;
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value& UnorderedMap<Key, Value, Hash, Equal, Alloc>::at(const Key& key) {
    try {
        auto it = find(key);
        if (it == end()) throw std::out_of_range("");
        return it->second;
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::erase(UnorderedMap::iterator it) {
    try {
        size_t hsh = hash(it->first) % ptr.size();
        if (ptr[hsh] != data.end() && equal(ptr[hsh]->first, it->first)) ++ptr[hsh];
        if (ptr[hsh] != data.end() && !equal(ptr[hsh]->first, it->first)) ptr[hsh] = data.end();
        data.erase(it);
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::erase(UnorderedMap::iterator it1, UnorderedMap::iterator it2) {
    try {
        while (it1 != it2) {
            auto temp = it1;
            ++it1;
            erase(temp);
        }
    } catch (...) {
        throw;
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::reserve(size_t new_size) {
    try {
        if (new_size > ptr.size()) rehash(new_size);
    } catch (...) {
        throw;
    }
}
