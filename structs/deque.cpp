#include<iostream>

template<typename T = int>
class Deque {
private:
    static const int cellSize = 8;

    int size_ = 0;
    int endIndex = -1;
    int beginIndex = 0;
    int startCell = 0;
    int endCell = 0;
    int allocated = 0;
    T** external;

    void reallocate(bool isExpandingForward);

public:
    Deque();

    ~Deque();

    explicit Deque(int length);

    Deque(int newSize, const T& object);

    T& operator[](int pos);

    const T& operator[](int pos) const;

    T& at(int pos);

    const T& at(int pos) const;

    size_t size() const {
        return static_cast<size_t>(size_);
    }

    Deque& operator=(Deque deque);

    Deque(const Deque& deque);

    void push_back(const T& object);

    void pop_back();

    void push_front(const T& object);

    void pop_front();

    template<typename U>
    class Iterator {
    private:
        U* position;
        U* const* outerPosition;
        int counter = 1;
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = U*;
        using reference = U&;

        Iterator() = default;

        Iterator(U* pointer, U* const* outerPointer, int beginIndex) : position(pointer),
                                                                       outerPosition(outerPointer),
                                                                       counter(beginIndex + 1) {}

        ~Iterator() = default;

        Iterator& operator=(const Iterator& iter);;

        Iterator(const Iterator& iter) = default;

        reference operator*() {
            return *position;
        }

        value_type operator*() const {
            return *position;
        }

        pointer operator->() {
            return position;
        }

        Iterator& operator++();

        Iterator operator++(int) const;

        Iterator& operator--();

        Iterator operator--(int) const;

        Iterator& operator+=(int n);

        Iterator& operator-=(int n);

        Iterator operator+(int n) const;

        Iterator operator-(int n) const;

        bool operator==(const Iterator& iter) const {
            return position == iter.position;
        }

        bool operator!=(const Iterator& iter) const {
            return !(*this == iter);
        }

        bool operator<(const Iterator& iter) const {
            return (outerPosition == iter.outerPosition) ?
                   (position < iter.position) : (outerPosition < iter.outerPosition);
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
            return (outerPosition - iter.outerPosition) * cellSize + (counter - iter.counter);
        }
    };

    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return iterator(&external[startCell][beginIndex], &external[startCell], beginIndex);
    }

    const_iterator cbegin() const {
        return const_iterator(&external[startCell][beginIndex], &external[startCell], beginIndex);
    }

    iterator end();

    const_iterator cend() const;

    const_iterator begin() const {
        return const_iterator(&external[startCell][beginIndex], &external[startCell], beginIndex);
    }

    const_iterator end() const;

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    void swap(Iterator<T>& iter1, Iterator<T>& iter2) {
        std::swap(*iter1.position, *iter2.position);
    }

    void insert(iterator iter, const T& object);

    void erase(iterator iter);

};

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U>& Deque<T>::Iterator<U>::operator=(const Deque<T>::Iterator<U>& iter) {
    position = iter.position;
    outerPosition = iter.outerPosition;
    counter = iter.counter;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U>& Deque<T>::Iterator<U>::operator++() {
    if (counter != cellSize) {
        ++counter;
        ++position;
        return *this;
    }
    counter = 1;
    ++outerPosition;
    position = outerPosition[0];
    return *this;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U> Deque<T>::Iterator<U>::operator++(int) const {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U>& Deque<T>::Iterator<U>::operator--() {
    if (counter != 1) {
        --counter;
        --position;
        return *this;
    }
    counter = cellSize;
    --outerPosition;
    position = outerPosition[0];
    position += (cellSize - 1);
    return *this;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U> Deque<T>::Iterator<U>::operator--(int) const {
    Iterator temp = *this;
    --(*this);
    return temp;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U>& Deque<T>::Iterator<U>::operator+=(int n) {
    outerPosition += (counter + n - 1) / cellSize;
    position = outerPosition[0];
    position += (counter + n - 1) % cellSize;
    counter = (counter + n) % cellSize;
    return *this;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U>& Deque<T>::Iterator<U>::operator-=(int n) {
    int temp = (counter - n - 1) % cellSize;
    if (temp < 0) {
        outerPosition += (counter - n - 1) / cellSize - 1;
        position = outerPosition[0];
        position += temp + cellSize;
        counter = temp + cellSize + 1;
        return *this;
    }
    outerPosition += (counter - n - 1) / cellSize;
    position = outerPosition[0];
    counter = temp + 1;
    position += counter - 1;
    return *this;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U> Deque<T>::Iterator<U>::operator+(int n) const {
    Iterator temp = *this;
    temp += n;
    return temp;
}

template<typename T>
template<typename U>
typename Deque<T>::template Iterator<U> Deque<T>::Iterator<U>::operator-(int n) const {
    Iterator temp = *this;
    temp -= n;
    return temp;
}

template<typename T>
void Deque<T>::reallocate(bool isExpandingForward) {
    try {
        if (isExpandingForward) {
            T** newExternal = new T* [allocated * 2];
            for (int i = 0; i < allocated; ++i) {
                newExternal[i] = external[i];
            }
            for (int i = allocated; i < allocated * 2; ++i) {
                newExternal[i] = reinterpret_cast<T*>(new uint8_t[cellSize * sizeof(T)]);
            }
            allocated *= 2;
            external = newExternal;
            return;
        }
        T** newExternal = new T* [allocated * 2];
        for (int i = allocated; i < allocated * 2; ++i) {
            newExternal[i] = external[i - allocated];
        }
        for (int i = 0; i < allocated; ++i) {
            newExternal[i] = reinterpret_cast<T*>(new uint8_t[cellSize * sizeof(T)]);
        }
        allocated *= 2;
        endCell += (allocated / 2 + allocated % 2);
        external = newExternal;
        startCell = allocated / 2;
    } catch (...) {
        throw;
    }
}

template<typename T>
Deque<T>::Deque() {
    try {
        allocated = 1;
        external = new T* [allocated];
        external[0] = reinterpret_cast<T*>(new uint8_t[cellSize * sizeof(T)]);
    } catch (...) {
        throw;
    }
}

template<typename T>
Deque<T>::~Deque() {
    for (int i = 0; i < allocated; ++i) {
        delete[] external[i];
    }
}

template<typename T>
Deque<T>::Deque(int length) {
    try {
        if (length != 0) endIndex = length % cellSize - 1;
        size_ = length;
        allocated = length / cellSize + 1;
        external = new T* [allocated];
        for (int i = 0; i < allocated; ++i) {
            external[i] = reinterpret_cast<T*>(new uint8_t[cellSize * sizeof(T)]);
        }
    } catch (...) {
        throw;
    }
}

template<typename T>
Deque<T>::Deque(int newSize, const T& object) : size_(newSize),
                                                endIndex(size_ % cellSize - 1) {
    try {
        allocated = size_ / cellSize + 1;
        external = new T* [allocated];
        for (int i = 0; i < allocated; ++i) {
            external[i] = reinterpret_cast<T*>(new uint8_t[cellSize * sizeof(T)]);
        }
        for (int i = 0; i < size_; ++i) {
            new(external[i / cellSize] + (i % cellSize)) T(object);
        }
        endCell = allocated - 1;
    } catch (...) {
        throw;
    }
}

template<typename T>
T& Deque<T>::at(int pos) {
    if (size_ <= pos) throw std::out_of_range("");
    return (*this)[pos];
}

template<typename T>
const T& Deque<T>::at(int pos) const {
    if (size_ <= pos) throw std::out_of_range("");
    return (*this)[pos];
}

template<typename T>
T& Deque<T>::operator[](int pos) {
    return external[startCell + (pos + beginIndex) / cellSize][(beginIndex + pos) % cellSize];
}

template<typename T>
const T& Deque<T>::operator[](int pos) const {
    return external[startCell + (pos + beginIndex) / cellSize][(beginIndex + pos) % cellSize];
}

template<typename T>
Deque<T>& Deque<T>::operator=(Deque deque) {
    try {
        std::swap(external, deque.external);
        std::swap(beginIndex, deque.beginIndex);
        std::swap(endIndex, deque.endIndex);
        std::swap(startCell, deque.startCell);
        std::swap(endCell, deque.endCell);
        std::swap(size_, deque.size_);
        std::swap(allocated, deque.allocated);
        return *this;
    } catch (...) {
        throw;
    }
}

template<typename T>
Deque<T>::Deque(const Deque& deque) {
    external = new T* [deque.allocated];
    beginIndex = deque.beginIndex;
    endIndex = deque.endIndex;
    endCell = deque.endCell;
    startCell = deque.startCell;
    size_ = deque.size_;
    allocated = deque.allocated;
    for (int i = 0; i < deque.allocated; ++i) {
        external[i] = reinterpret_cast<T*>(new uint8_t[cellSize * sizeof(T)]);
    }
    for (int i = 0; i < deque.size_; ++i) {
        (*this)[i] = deque[i];
    }
}

template<typename T>
void Deque<T>::push_back(const T& object) {
    try {
        if (endIndex != cellSize - 1) {
            new(external[endCell] + endIndex + 1) T(object);
            ++endIndex;
            ++size_;
            return;
        }
        if (endCell == allocated - 1) reallocate(true);
        ++endCell;
        new(external[endCell]) T(object);
        endIndex = 0;
        ++size_;
    } catch (...) {
        throw;
    }
}

template<typename T>
void Deque<T>::pop_back() {
    try {
        (external[endCell] + endIndex)->~T();
        --size_;
        if (endIndex != 0) --endIndex;
        else {
            --endCell;
            endIndex = cellSize - 1;
        }
    } catch (...) {
        throw;
    }
}

template<typename T>
void Deque<T>::push_front(const T& object) {
    try {
        if (beginIndex != 0) {
            new(external[startCell] + beginIndex - 1) T(object);
            --beginIndex;
            ++size_;
            return;
        }
        if (startCell == 0) reallocate(false);
        if (endIndex == -1) {
            endIndex = cellSize - 1;
            endCell = 0;
        }
        --startCell;
        new(external[startCell] + cellSize - 1) T(object);
        ++size_;
        beginIndex = cellSize - 1;
    } catch (...) {
        throw;
    }
}

template<typename T>
void Deque<T>::pop_front() {
    try {
        if (beginIndex == cellSize) {
            ++startCell;
            beginIndex = 1;
            (external[startCell] + beginIndex - 1)->~T();
        } else {
            ++beginIndex;
            (external[startCell] + beginIndex - 1)->~T();
        }
        --size_;
        if (beginIndex == cellSize) {
            beginIndex = 0;
            ++startCell;
        }
    } catch (...) {
        throw;
    }
}

template<typename T>
typename Deque<T>::iterator Deque<T>::end() {
    iterator temp = iterator(&external[endCell][endIndex], &external[endCell], endIndex);
    ++temp;
    return temp;
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const {
    iterator temp = const_iterator(&external[endCell][endIndex], &external[endCell], endIndex);
    ++temp;
    return temp;
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::end() const {
    const_iterator temp = const_iterator(&external[endCell][endIndex], &external[endCell], endIndex);
    ++temp;
    return temp;
}

template<typename T>
void Deque<T>::insert(Deque::iterator iter, const T& object) {
    push_back(object);
    for (auto it = end() - 2; it >= iter; --it) {
        std::iter_swap(it, it + 1);
    }
}

template<typename T>
void Deque<T>::erase(Deque::iterator iter) {
    for (auto it = iter; it < end() - 2; ++it) {
        std::iter_swap(it, it + 1);
    }
    pop_back();
}
