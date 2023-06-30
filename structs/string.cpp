#include <iostream>
#include <cstring>

class String {
private:
    size_t size = 0;
    size_t allocatedMemory = 0;
    char* data = nullptr;

    void reallocateMemory(size_t newAllocatedValue);
    void reallocateMemory(bool isAllocatingMoreMemory);
public:
    String() = default;
    String(const String& str) : size(str.length()), allocatedMemory(size), data(new char[size]) {
        memcpy(data, str.data, size);
    }
    String(const char* cstr) : size(strlen(cstr)), allocatedMemory(size), data(new char[size]) {
        memcpy(data, cstr, size);
    }
    String(size_t n, char c) : size(n), allocatedMemory(size), data(new char[n]) {
        memset(data, c, n);
    }
    String(char c) {
        *this = String(1, c);
    }
    ~String() {
        delete[] data;
    }
    size_t length() const {
        return size;
    }
    bool empty() const {
        return size == 0;
    }
    void clear();
    const char& operator[](size_t pos) const {
        return data[pos];
    }
    char& operator[](size_t pos) {
        return data[pos];
    }
    void push_back(char c);
    void pop_back();
    String& operator+=(const String& str);
    String& operator+=(char c);
    void swap(String& str);
    String& operator=(const String& str);
    const char& front() const {
        return data[0];
    }
    char& front() {
        return data[0];
    }
    const char& back() const {
        return data[size - 1];
    }
    char& back() {
        return data[size - 1];
    }
    size_t find(const String& substring) const;
    size_t rfind(const String& substring) const;
    String substr(size_t start, size_t count) const;
    friend std::ostream& operator<<(std::ostream& stream, const String& str);
    friend std::istream& operator>>(std::istream& stream, String& str);
};

String operator+(const String& str1, const String& str2) {
    String temp = str1;
    temp += str2;
    return temp;
}

std::ostream& operator<<(std::ostream& stream, const String& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        stream << str[i];
    }
    return stream;
}

std::istream& operator>>(std::istream& stream, String& str) {
    str.clear();
    char c = ' ';
    while (!stream.eof() && std::isspace(c)) {
        stream.get(c);
    }
    if (!std::isspace(c)) str.push_back(c);
    while (stream.get(c)) {
        if (std::isspace(c)) break;
        str.push_back(c);
    }
    return stream;
}

String String::substr(size_t start, size_t count) const {
    String temp(count, 'c');
    memcpy(temp.data, data + start, count);
    return temp;
}

size_t String::rfind(const String& substring) const {
    if (substring.length() == 0) return size;
    for (long long i = size + 1 - substring.length(); i >= 0; --i) {
        if (!memcmp(data + i, substring.data, substring.size)) return i;
    }
    return size;
}

size_t String::find(const String& substring) const {
    if (substring.length() == 0) return 0;
    for (size_t i = 0; i <= size - substring.length(); ++i) {
        if (!memcmp(data + i, substring.data, substring.size)) return i;
    }
    return size;
}

void String::clear() {
    delete[] data;
    data = nullptr;
    allocatedMemory = 0;
    size = 0;
}

void String::push_back(char c) {
    if (size >= allocatedMemory) reallocateMemory(true);
    data[size++] = c;
}

void String::pop_back() {
    size--;
    if (size * 4 <= allocatedMemory) reallocateMemory(false);
}

String& String::operator+=(const String& str) {
    size_t newSize = size + str.size;
    reallocateMemory(newSize);
    memcpy((data + size), str.data, str.size);
    size = newSize;
    return *this;
}

String& String::operator+=(char c) {
    push_back(c);
    return *this;
}

void String::swap(String& str) {
    std::swap(data, str.data);
    std::swap(size, str.size);
    std::swap(allocatedMemory, str.allocatedMemory);
}

String& String::operator=(const String& str) {
    String copy = str;
    swap(copy);
    return *this;
}

void String::reallocateMemory(size_t newAllocatedValue) {
    allocatedMemory = newAllocatedValue;
    char* temp = new char[allocatedMemory];
    memcpy(temp, data, size);
    delete[] data;
    data = temp;
}

void String::reallocateMemory(bool isAllocatingMoreMemory) {
    size_t newAllocatedValue = (isAllocatingMoreMemory) ? ((size + 1) * 2) : ((allocatedMemory + 1) / 2);
    return reallocateMemory(newAllocatedValue);
}

bool operator==(const String& str1, const String& str2) {
    size_t length = str1.length();
    if (length != str2.length()) return false;
    for (size_t i = 0; i < length; ++i) {
        if (str1[i] != str2[i]) return false;
    }
    return true;
}
