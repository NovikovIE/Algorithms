#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cin;
using std::cout;
using std::string;
using std::vector;

template <size_t N>
class Residue {
private:
    int data;

    template <int number, int current, int sqr>
    struct PrimeRecursion {
        static const bool value =
            (number % current != 0) &&
            PrimeRecursion<number, current + 1, std::min((current + 1) * (current + 1), number)>::value;
    };

    template <int number, int current>
    struct PrimeRecursion<number, current, number> {
        static const bool value = true;
    };

    template <int number>
    struct PrimeCheck {
        static const bool result = PrimeRecursion<number, 2, 4>::value;
    };

    int pow(int a, int n);

public:
    explicit Residue(int number);
    Residue() = default;
    Residue(const Residue& residue) { data = residue.data; }
    ~Residue() = default;
    Residue& operator=(const Residue& residue) {
        data = residue.data;
        return *this;
    }
    Residue& operator+=(const Residue& residue) {
        data += residue.data;
        data %= N;
        return *this;
    }
    Residue operator+(const Residue& residue) const {
        Residue temp = *this;
        temp += residue;
        return temp;
    }
    Residue& operator-=(const Residue& residue) {
        data = N + data - residue.data;
        data %= N;
        return *this;
    }
    Residue operator-(const Residue& residue) const {
        Residue temp = *this;
        temp -= residue;
        return temp;
    }
    Residue& operator*=(const Residue& residue) {
        data *= residue.data;
        data %= N;
        return *this;
    }
    Residue operator*(const Residue& residue) const {
        Residue temp = *this;
        temp *= residue;
        return temp;
    }
    Residue& operator/=(const Residue& residue) {
        static_assert(PrimeCheck<N>::result, "");
        int j = pow(residue.data, N - 2) % N;
        data *= j;
        data %= N;
        return *this;
    }
    Residue operator/(const Residue& residue) const {
        Residue temp = *this;
        temp /= residue;
        return temp;
    }
    bool operator==(const Residue& residue) const { return data == residue.data; }
    bool operator!=(const Residue& residue) const { return data != residue.data; }
    bool operator>(const Residue& residue) const { return data > residue.data; }
    bool operator<(const Residue& residue) const { return data < residue.data; }
    bool operator<=(const Residue& residue) const { return data <= residue.data; }
    bool operator>=(const Residue& residue) const { return data >= residue.data; }
    explicit operator int() const {
        int temp = data;
        return temp;
    }
    template <size_t n>
    friend std::ostream& operator<<(std::ostream& stream, const Residue<n>& residue);
    template <size_t n>
    friend std::istream& operator>>(std::istream& stream, Residue<n> residue);
};

template <size_t n>
std::ostream& operator<<(std::ostream& stream, const Residue<n>& residue) {
    stream << residue.data;
    return stream;
}

template <size_t n>
std::istream& operator>>(std::istream& stream, Residue<n> residue) {
    stream >> residue.data;
    residue %= n;
    return stream;
}

template <size_t N>
int Residue<N>::pow(int a, int n) {
    if (n == 0)
        return 1;
    if (n % 2 == 1)
        return pow(a, n - 1) * a % N;
    else {
        int b = pow(a, n / 2) % N;
        return b * b % N;
    }
}

template <size_t N>
Residue<N>::Residue(int number) {
    if (number >= 0) {
        data = number % N;
        return;
    }
    int temp = -number / N + 1;
    data = (int(N) * temp + number) % N;
}

template <size_t rows, size_t columns, typename Field = double>
class Matrix {
private:
    vector<vector<Field>> data;

    bool isConsistsOfZeros(const vector<Field>& vec) const;

public:
    void gauss();
    Matrix<rows, columns, Field>();

    Matrix& operator=(const Matrix& matrix) = default;
    Matrix<rows, columns, Field>(const Matrix<rows, columns, Field>& matrix);

    template <typename T>
    Matrix(const std::initializer_list<std::initializer_list<T>>& list);

    explicit Matrix(vector<vector<Field>> newData);
    ~Matrix() = default;

    template <size_t rows1, size_t columns1, typename Field1>
    bool operator==(const Matrix<rows1, columns1, Field1>& matrix) const;

    template <size_t rows1, size_t columns1, typename Field1>
    bool operator!=(const Matrix<rows1, columns1, Field1>& matrix) const {
        return !(*this == matrix);
    }

    vector<Field>& operator[](const size_t position) { return data[position]; }
    const vector<Field>& operator[](const size_t position) const { return data[position]; }

    Matrix& operator+=(const Matrix<rows, columns, Field>& matrix);
    Matrix& operator-=(const Matrix<rows, columns, Field>& matrix);
    Matrix& operator*=(Field number);
    Matrix& operator*=(const Matrix<rows, columns, Field>& matrix);

    vector<Field> getRow(size_t pos) const { return data[pos]; }
    vector<Field> getColumn(size_t pos) const;

    Matrix<columns, rows, Field> transposed() const;

    Field trace() const;
    Field det() const;
    size_t rank() const;

    void invert();
    Matrix inverted() const;
};

template <size_t rows, size_t columns, typename Field>
bool Matrix<rows, columns, Field>::isConsistsOfZeros(const vector<Field>& vec) const {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] != Field(0))
            return false;
    }
    return true;
}

template <size_t rows, size_t columns, typename Field>
void Matrix<rows, columns, Field>::gauss() {
    for (size_t i = 0; i < std::min(rows, columns); ++i) {
        if (data[i][i] == Field(0)) {
            bool isColumnOfZeros = true;
            for (size_t j = 0; j < rows; ++j) {
                if (data[j][i] != Field(0))
                    isColumnOfZeros = false;
            }
            if (isColumnOfZeros)
                continue;
            size_t j = i + 1;
            while (j < rows) {
                if (data[j][i] != Field(0)) {
                    for (size_t k = 0; k < columns; ++k) {
                        data[i][k] += data[j][k];
                    }
                    break;
                }
                ++j;
            }
        }
        for (size_t j = i + 1; j < rows; ++j) {
            if (data[j][i] == Field(0))
                continue;
            Field temp = data[j][i] / data[i][i];
            for (size_t k = i; k < columns; ++k) {
                data[j][k] -= temp * data[i][k];
            }
        }
    }
}

template <size_t rows, size_t columns, typename Field>
template <size_t rows1, size_t columns1, typename Field1>
bool Matrix<rows, columns, Field>::operator==(const Matrix<rows1, columns1, Field1>& matrix) const {
    if (rows != rows1 || columns != columns1)
        return false;
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[i].size(); ++j) {
            if (data[i][j] != matrix.data[i][j])
                return false;
        }
    }
    return true;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>& Matrix<rows, columns, Field>::operator+=(const Matrix<rows, columns, Field>& matrix) {
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            data[i][j] += matrix[i][j];
        }
    }
    return *this;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>& Matrix<rows, columns, Field>::operator-=(const Matrix<rows, columns, Field>& matrix) {
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            data[i][j] -= matrix[i][j];
        }
    }
    return *this;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>& Matrix<rows, columns, Field>::operator*=(const Field number) {
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            data[i][j] *= number;
        }
    }
    return *this;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>& Matrix<rows, columns, Field>::operator*=(const Matrix<rows, columns, Field>& matrix) {
    static_assert(rows == columns, "");
    *this = (*this * matrix);
    return *this;
}

template <size_t rows, size_t columns, typename Field>
vector<Field> Matrix<rows, columns, Field>::getColumn(size_t pos) const {
    vector<Field> resultColumn(rows);
    for (size_t i = 0; i < rows; ++i) {
        resultColumn[i] = data[i][pos];
    }
    return resultColumn;
}

template <size_t rows, size_t columns, typename Field>
Field Matrix<rows, columns, Field>::trace() const {
    static_assert(rows == columns, "");
    Field trace(0);
    for (size_t i = 0; i < data.size(); ++i) {
        trace += data[i][i];
    }
    return trace;
}

template <size_t rows, size_t columns, typename Field>
Matrix<columns, rows, Field> Matrix<rows, columns, Field>::transposed() const {
    Matrix<columns, rows, Field> result;
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            result[j][i] = data[i][j];
        }
    }
    return result;
}

template <size_t rows, size_t columns, typename Field>
Field Matrix<rows, columns, Field>::det() const {
    static_assert(rows == columns, "");
    Matrix copy = *this;
    copy.gauss();
    Field result(1);
    for (size_t i = 0; i < rows; ++i) {
        result *= copy[i][i];
    }
    return result;
}

template <size_t rows, size_t columns, typename Field>
size_t Matrix<rows, columns, Field>::rank() const {
    Matrix copy = *this;
    copy.gauss();
    size_t result1 = 0;
    size_t result2 = 0;
    for (size_t i = 0; i < rows; ++i) {
        if (!isConsistsOfZeros(copy.getRow(i)))
            ++result1;
    }
    for (size_t i = 0; i < columns; ++i) {
        if (!isConsistsOfZeros(copy.getColumn(i)))
            ++result2;
    }
    return std::min(result1, result2);
}

template <size_t rows, size_t columns, typename Field>
void Matrix<rows, columns, Field>::invert() {
    static_assert(rows == columns, "");
    Matrix<rows, rows, Field> result;
    for (size_t i = 0; i < rows; ++i) {
        if (data[i][i] == Field(0)) {
            size_t j = i + 1;
            while (j < rows) {
                if (data[j][i] != Field(0)) {
                    swap(data[i], data[j]);
                    swap(result.data[i], result.data[j]);
                    break;
                }
                ++j;
            }
        }
        if (data[i][i] != Field(1)) {
            Field temp = data[i][i];
            for (size_t j = 0; j < columns; ++j) {
                data[j][i] /= temp;
                result[j][i] /= temp;
            }
        }
        for (size_t j = 0; j < rows; ++j) {
            if (data[j][i] == Field(0))
                continue;
            if (i == j)
                continue;
            Field temp = data[j][i] / data[i][i];
            for (size_t k = 0; k < columns; ++k) {
                data[j][k] -= temp * data[i][k];
                result.data[j][k] -= temp * result.data[i][k];
            }
        }
    }
    data = result.data;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field> Matrix<rows, columns, Field>::inverted() const {
    Matrix<rows, columns, Field> copy = *this;
    copy.invert();
    return copy;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>::Matrix(vector<vector<Field>> newData) {
    data = vector<vector<Field>>(rows, vector<Field>(columns));
    for (size_t i = 0; i < data.size(); ++i) {
        for (size_t j = 0; j < data[0].size(); ++j) {
            data[i][j] = newData[i][j];
        }
    }
}

template <size_t rows, size_t columns, typename Field>
template <typename T>
Matrix<rows, columns, Field>::Matrix(const std::initializer_list<std::initializer_list<T>>& list) {
    data = vector<vector<Field>>(rows, vector<Field>(columns));
    size_t i = 0;
    for (auto element : list) {
        size_t j = 0;
        for (T number : element) {
            data[i][j++] = Field(number);
        }
        ++i;
    }
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>::Matrix(const Matrix<rows, columns, Field>& matrix) {
    data = vector<vector<Field>>(rows, vector<Field>(columns));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            data[i][j] = matrix[i][j];
        }
    }
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field>::Matrix() {
    data = vector<vector<Field>>(rows, vector<Field>(columns));
    if (rows == columns) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                data[i][j] = (i == j) ? (Field(1)) : (Field(0));
            }
        }
    }
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field> operator*(const Matrix<rows, columns, Field> matrix, const Field number) {
    Matrix<rows, columns, Field> temp = matrix;
    temp *= number;
    return temp;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field> operator*(const Field number, const Matrix<rows, columns, Field> matrix) {
    Matrix<rows, columns, Field> temp = matrix;
    temp *= number;
    return temp;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field> operator+(const Matrix<rows, columns, Field>& matrix1,
                                       const Matrix<rows, columns, Field>& matrix2) {
    Matrix<rows, columns, Field> temp = matrix1;
    temp += matrix2;
    return temp;
}

template <size_t rows, size_t columns, typename Field>
Matrix<rows, columns, Field> operator-(const Matrix<rows, columns, Field>& matrix1,
                                       const Matrix<rows, columns, Field>& matrix2) {
    Matrix<rows, columns, Field> temp = matrix1;
    temp -= matrix2;
    return temp;
}

template <size_t rows1, size_t columns1, size_t rows2, size_t columns2, typename Field>
Matrix<rows1, columns2, Field> operator*(const Matrix<rows1, columns1, Field>& matrix1,
                                         const Matrix<rows2, columns2, Field>& matrix2) {
    static_assert(columns1 == rows2, "");
    Matrix<rows1, columns2, Field> result;
    for (size_t i = 0; i < rows1; ++i) {
        for (size_t j = 0; j < columns2; ++j) {
            result[i][j] = Field(0);
            for (size_t k = 0; k < columns1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

template <size_t N, typename Field = double>
using SquareMatrix = Matrix<N, N, Field>;
