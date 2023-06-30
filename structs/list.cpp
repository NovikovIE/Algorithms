#include <memory>

using std::allocator_traits;

template<size_t N>
class StackStorage {
private:
    uint8_t data[N];
    uint8_t* current = data;
public:
    StackStorage() = default;

    ~StackStorage() = default;

    uint8_t* allocate(size_t n, size_t alignment);

    void deallocate(size_t n, uint8_t* ptr);

    StackStorage(const StackStorage&) = delete;

    StackStorage& operator=(const StackStorage&) = delete;
};

template<size_t N>
uint8_t* StackStorage<N>::allocate(size_t n, size_t alignment) {
    if (uintptr_t(current) % alignment != 0) {
        current += alignment - uintptr_t(current) % alignment;
    }
    uint8_t* temp = current;
    current += n * alignment;
    return temp;
}

template<size_t N>
void StackStorage<N>::deallocate(size_t n, uint8_t* ptr) {
    if (ptr + n == current) {
        current = ptr;
    }
}

template<typename T, size_t N>
class StackAllocator {
public:
    StackStorage<N>* storage = nullptr;

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template<size_t Length>
    explicit StackAllocator(StackStorage<Length>& storage_) {
        storage = &storage_;
    }

    StackAllocator(const StackAllocator& stack_allocator) = default;


    template<typename U>
    StackAllocator(const StackAllocator<U, N>& stack_allocator) {
        storage = stack_allocator.storage;
    }

    StackAllocator& operator=(const StackAllocator& stackAllocator);

    template<typename U, size_t Other_N>
    bool operator==(const StackAllocator<U, Other_N>& stackAllocator) const {
        return (N == Other_N && storage == stackAllocator.storage);
    }

    template<typename U, size_t Other_N>
    bool operator!=(const StackAllocator<U, Other_N>& stackAllocator) const {
        return !(*this == stackAllocator);
    }

    pointer allocate(size_t n) {
        return reinterpret_cast<T*>(storage->allocate(n, sizeof(T)));
    }

    void deallocate(pointer ptr, size_t n) {
        storage->deallocate(n * sizeof(T), reinterpret_cast<uint8_t*>(ptr));
    }

    ~StackAllocator() = default;

    template<typename Other>
    struct rebind {
        using other = StackAllocator<Other, N>;
    };
};

template<typename T, size_t N>
StackAllocator<T, N>& StackAllocator<T, N>::operator=(const StackAllocator& stackAllocator) {
    storage = stackAllocator.storage;
    return *this;
}

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

        Node(Allocator& alloc, const T& obj);

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
    std::allocator<Node> node_allocator = std::allocator<Node>();
    Node* corner = nullptr;
    size_t size_ = 0;
public:
    explicit List(Allocator& alloc) : allocator(alloc){
        initialize_corner();
    }

    List() : allocator(Allocator()) {
        initialize_corner();
    }

    explicit List(size_t n);

    List(size_t n, const T& object);

    List(size_t n, Allocator& alloc);

    List(size_t n, const T& object, Allocator& alloc);

    Allocator get_allocator() const {
        return allocator;
    }

    size_t size() const {
        return size_;
    }

    void swap(List<T, Allocator>& list);

    List& operator=(const List<T, Allocator>& list);

    List(const List& list);

    ~List() {
        deletion();
    }

    void push_back(const T& object);

    void pop_back();

    void push_front(const T& object);

    void pop_front();

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
        explicit Iterator(const Iterator<Another_Type>& iter) {
            position = iter.position;
        }

        reference operator*() {
            return *(position->value);
        }

        value_type operator*() const {
            return *(position->value);
        }

        pointer operator->() {
            return position;
        }

        Iterator& operator++();

        Iterator operator++(int);

        Iterator& operator--();

        Iterator operator--(int);

        bool operator==(const Iterator& iter) const {
            return position == iter.position;
        }

        bool operator!=(const Iterator& iter) const {
            return !(*this == iter);
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

        void deletion(Allocator alloc) {
            position->deletion(alloc);
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

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    void insert(iterator iter, const T& object);

    void insert(const_iterator iter, const T& object);

    void erase(const_iterator iter);

    void erase(iterator iter);
};

template<typename T, typename Allocator>
template<typename Type>
typename List<T, Allocator>::template Iterator<Type>&
List<T, Allocator>::Iterator<Type>::operator=(List::Iterator<Type> iter) {
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
        Node* node = node_allocator.allocate(1);
        std::allocator_traits<std::allocator<Node>>::construct(node_allocator, node, allocator, ptr);
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
        Node* node = node_allocator.allocate(1);
        std::allocator_traits<std::allocator<Node>>::construct(node_allocator, node, allocator, ptr, obj);
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
        Node* node = node_allocator.allocate(1);
        std::allocator_traits<std::allocator<Node>>::construct(node_allocator, node, allocator, ptr, obj);
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
        corner = node_allocator.allocate(1);
        std::allocator_traits<std::allocator<Node>>::construct(node_allocator, corner);
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
    std::swap(node_allocator, list.node_allocator);
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
void List<T, Allocator>::push_back(const T& object) {
    try {
        ++size_;
        T* ptr = allocator_traits<Allocator>::allocate(allocator, 1);
        Node* temp = nullptr;
        if (corner->prev) temp = corner->prev;
        Node* node = create_new_node_back(ptr, object);
        if (temp) {
            node->prev = temp;
            temp->next = node;
        }
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
    try {
        --size_;
        Node* temp = corner->prev->prev;
        if (temp != corner) {
            corner->prev->deletion(allocator);
            delete corner->prev;
        }
        corner->prev = temp;
        corner->prev->next = corner;
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_front(const T& object) {
    try {
        ++size_;
        T* ptr = allocator_traits<Allocator>::allocate(allocator, 1);
        Node* temp = corner->next;
        Node* node = create_new_node_front(ptr, object);
        if (temp) {
            node->next = temp;
            temp->prev = node;
        }
    } catch (...) {
        throw;
    }
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
    try {
        --size_;
        Node* temp = corner->next->next;
        if (temp != corner) {
            corner->next->deletion(allocator);
            delete corner->next;
        }
        corner->next = temp;
        corner->next->prev = corner;
    } catch (...) {
        throw;
    }
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
void List<T, Allocator>::erase(List::iterator iter) {
    try {
        --size_;
        if (iter.position == corner) return;
        auto prev = iter.prev();
        auto next = iter.next();
        iter.deletion(allocator);
        delete iter.position;
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
        Node* node = node_allocator.allocate(1);
        std::allocator_traits<std::allocator<Node>>::construct(node_allocator, node, allocator, object);
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
void List<T, Allocator>::Node::deletion(Allocator& alloc) {
    allocator_traits<Allocator>::destroy(alloc, value);
    if (value) allocator_traits<Allocator>::deallocate(alloc, value, 1);
    value = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>::Node::Node(Allocator& alloc, const T& obj) {
    value = alloc.allocate(1);
    std::allocator_traits<Allocator>::construct(alloc, value, obj);
}
