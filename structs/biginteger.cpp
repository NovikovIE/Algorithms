#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;


////////////////////////////////////////////////////////
///////// WARNING //////////////////////////////////////
////////////////////////////////////////////////////////
///////// MAYBE IT DOES NOT WORK ON BIG NUMBERS/////////
////////////////////////////////////////////////////////


class BigInteger {
private:
    static const int BASE = 10000;
    bool isPositive = true;
    vector<int> data = vector<int>(0);

    void shift();

    const int& operator[](size_t index) const { return data[index]; }

    int& operator[](size_t index) { return data[index]; }

public:
    BigInteger() = default;

    BigInteger(const BigInteger& bigInt) = default;

    BigInteger(int number);

    ~BigInteger() = default;

    int length() const { return data.size(); }

    void clear() {
        data.clear();
        isPositive = true;
    }

    void changeSign() { isPositive = !isPositive; }

    BigInteger& operator=(const BigInteger& bigInt) = default;

    string toString() const;

    BigInteger& operator++();

    BigInteger operator++(int);

    BigInteger& operator--();

    BigInteger operator--(int);

    BigInteger& operator%=(const BigInteger& bigInt);

    BigInteger& operator/=(const BigInteger& bigInt);

    BigInteger& operator*=(const BigInteger& bigInt);

    BigInteger& operator+=(const BigInteger& bigInt);

    BigInteger& operator-=(const BigInteger& bigInt);

    friend std::ostream& operator<<(std::ostream& stream, const BigInteger& bigInt);

    friend std::istream& operator>>(std::istream& stream, BigInteger& bigInt);

    friend bool operator<(const BigInteger& bigInt1, const BigInteger& bigInt2);

    friend bool operator==(const BigInteger& bigInt1, const BigInteger& bigInt2);

    friend class Rational;

    explicit operator bool() const;

    void divideBy2();
};

BigInteger operator-(const BigInteger& bigInt) {
    BigInteger temp = bigInt;
    temp.changeSign();
    return temp;
}

bool operator==(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return (bigInt1.isPositive == bigInt2.isPositive) && (bigInt1.data == bigInt2.data);
}

bool operator!=(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return !(bigInt1 == bigInt2);
}

bool operator<(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    if (bigInt1.isPositive && !bigInt2.isPositive)
        return false;
    if (!bigInt1.isPositive && bigInt2.isPositive)
        return true;
    if (bigInt1.length() != bigInt2.length())
        return (bigInt1.length() < bigInt2.length());
    if (!bigInt1.isPositive && !bigInt2.isPositive)
        return !(-bigInt1 < -bigInt2);
    for (int i = bigInt1.length() - 1; i >= 0; --i) {
        if (bigInt1[i] != bigInt2[i])
            return bigInt1[i] < bigInt2[i];
    }
    return false;
}

bool operator>(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return bigInt2 < bigInt1;
}

bool operator>=(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return !(bigInt1 < bigInt2);
}

bool operator<=(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    return !(bigInt1 > bigInt2);
}

BigInteger& BigInteger::operator+=(const BigInteger& bigInt) {
    if (isPositive != bigInt.isPositive) {
        BigInteger temp;
        if (isPositive)
            temp = bigInt;
        else {
            temp = *this;
            *this = bigInt;
        }
        temp.changeSign();
        *this -= temp;
        return *this;
    }
    int bigIntLength = length();
    int border = (bigIntLength > bigInt.length()) ? (std::min(bigInt.length(), bigIntLength))
                                                  : (std::min(bigInt.length(), bigIntLength) - 1);
    for (int i = 0; i < border; ++i) {
        data[i] += bigInt[i];
        data[i + 1] += data[i] / BASE;
        data[i] %= BASE;
    }
    if (bigInt.length() >= bigIntLength) {
        data[bigIntLength - 1] += bigInt[bigIntLength - 1];
        int temp = data[bigIntLength - 1] / BASE;
        for (int i = bigIntLength; i < bigInt.length(); ++i) {
            data.push_back(bigInt[i] + temp);
            temp = data[i] / BASE;
            data[i] %= BASE;
        }
        data[bigInt.length() - 1] %= BASE;
        if (temp)
            data.push_back(temp);
    }
    return *this;
}

BigInteger operator+(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger temp = bigInt1;
    temp += bigInt2;
    return temp;
}

BigInteger& BigInteger::operator-=(const BigInteger& bigInt) {
    if (isPositive != bigInt.isPositive) {
        if (!isPositive)
            return *this = -(-*this += bigInt);
        return *this += -bigInt;
    }
    if (!isPositive && !bigInt.isPositive) {
        BigInteger temp = bigInt;
        return *this = (-temp -= -*this);
    }
    if (*this < bigInt) {
        BigInteger temp = bigInt;
        return *this = -(temp -= *this);
    }
    int bigIntLength = length();
    int border = (bigIntLength > bigInt.length()) ? (std::min(bigInt.length(), bigIntLength))
                                                  : (std::min(bigInt.length(), bigIntLength) - 1);
    for (int i = 0; i < border; ++i) {
        data[i] -= bigInt[i];
        if (data[i] < 0) {
            data[i + 1]--;
            data[i] += BASE;
        }
    }
    if (bigIntLength > bigInt.length()) {
        for (int i = bigInt.length(); i < bigIntLength - 1; ++i) {
            if (data[i] < 0) {
                data[i + 1]--;
                data[i] += BASE;
            }
        }
    } else {
        data[bigInt.length() - 1] -= bigInt[bigInt.length() - 1];
    }
    while (length() > 1 && data[length() - 1] == 0) {
        data.pop_back();
    }
    return *this;
}

BigInteger operator-(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger temp = bigInt1;
    temp -= bigInt2;
    return temp;
}

BigInteger& BigInteger::operator*=(const BigInteger& bigInt) {
    BigInteger product;
    product.data.assign(length() + bigInt.length(), 0);
    for (int i = 0; i < length(); ++i) {
        int temp = 0;
        for (int j = 0; j < bigInt.length() || temp != 0; ++j) {
            long long currentProduct = 0;
            currentProduct = product[i + j] + data[i] * ((j < bigInt.length()) ? (bigInt[j]) : (0)) + temp;
            product[i + j] = int(currentProduct % BASE);
            temp = int(currentProduct) / BASE;
        }
    }
    product.isPositive = (isPositive == bigInt.isPositive);
    *this = product;
    while (length() > 1 && data[length() - 1] == 0) {
        data.pop_back();
    }
    return *this;
}

BigInteger operator*(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger temp = bigInt1;
    temp *= bigInt2;
    return temp;
}

BigInteger operator/(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger temp = bigInt1;
    temp /= bigInt2;
    return temp;
}

BigInteger operator%(const BigInteger& bigInt1, const BigInteger& bigInt2) {
    BigInteger temp = bigInt1;
    temp %= bigInt2;
    return temp;
}

std::ostream& operator<<(std::ostream& stream, const BigInteger& bigInt) {
    if (bigInt.data.empty())
        stream << 0;
    else {
        long long border = bigInt.length() - 1;
        if (!bigInt.isPositive)
            stream << "-";
        if (bigInt.length() != 0)
            stream << bigInt[border];
        for (long long i = border - 1; i >= 0; --i) {
            if (bigInt[i] < 10)
                stream << "000" << bigInt[i];
            else if (bigInt[i] < 100)
                stream << "00" << bigInt[i];
            else if (bigInt[i] < 1000)
                stream << "0" << bigInt[i];
            else
                stream << bigInt[i];
        }
    }
    return stream;
}

std::istream& operator>>(std::istream& stream, BigInteger& bigInt) {
    bigInt.clear();
    int digits = 4;
    int border = 0;
    string in;
    string temp;
    stream >> in;
    long long i = in.size() - 1;
    if (in[0] == '-') {
        bigInt.changeSign();
        ++border;
    }
    for (; i >= border; i -= digits) {
        temp.clear();
        if (i - digits >= border) {
            for (long long j = i - digits + 1; j <= i; ++j) {
                temp.push_back(in[j]);
            }
            int dat;
            dat = std::stoi(temp);
            bigInt.data.push_back(dat);
        } else {
            long long j = border;
            for (; j <= i; ++j) {
                temp.push_back(in[j]);
            }
            if (j != border) {
                int dat;
                dat = std::stoi(temp);
                bigInt.data.push_back(dat);
            }
        }
    }
    while (bigInt.length() > 1 && bigInt[bigInt.length() - 1] == 0) {
        bigInt.data.pop_back();
    }
    return stream;
}

BigInteger::BigInteger(int number) {
    isPositive = (number >= 0);
    int temp = number;
    if (temp == 0)
        data.push_back(0);
    while (temp != 0) {
        if (number > 0)
            data.push_back(temp % BASE);
        else
            data.push_back(-(temp % BASE));
        temp /= BASE;
    }
}

string BigInteger::toString() const {
    if (data.empty())
        return "0";
    if (data.size() == 1 && data[0] == 0)
        return "0";
    string str;
    if (!isPositive)
        str.push_back('-');
    str += std::to_string(data[data.size() - 1]);
    for (long long i = length() - 2; i >= 0; --i) {
        if (data[i] < 10)
            str += "000";
        else if (data[i] < 100)
            str += "00";
        else if (data[i] < 1000)
            str += "0";
        str += std::to_string(data[i]);
    }
    return str;
}

BigInteger& BigInteger::operator++() {
    return *this += 1;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp = *this;
    ++*this;
    return temp;
}

BigInteger& BigInteger::operator--() {
    if (!isPositive) {
        changeSign();
        ++*this;
        changeSign();
        return *this;
    }
    size_t i = 0;
    --data[0];
    for (; i < data.size() - 1; ++i) {
        if (data[i] >= 0)
            break;
        else {
            data[i] += BASE;
            if (i < data.size() - 1) {
                --data[i + 1];
            }
        }
    }
    while (data.size() > 1 && data[data.size() - 1] == 0) {
        data.pop_back();
    }
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp = *this;
    --*this;
    return temp;
}

void BigInteger::shift() {
    if (data.empty()) {
        data.push_back(0);
        return;
    }
    data.push_back(data[data.size() - 1]);
    for (long long i = length() - 2; i >= 1; --i) {
        data[i] = data[i - 1];
    }
}

BigInteger::operator bool() const {
    if (data.empty())
        return false;
    if (data.size() == 1 && data[0] == 0)
        return false;
    return true;
}

BigInteger& BigInteger::operator%=(const BigInteger& bigInt) {
    bool flag = false;
    if (!isPositive)
        flag = true;
    *this -= (*this / bigInt) * bigInt;
    if (flag)
        isPositive = false;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& bigInt) {
    BigInteger tempBigInt = bigInt;
    tempBigInt.isPositive = true;
    BigInteger result;
    result.data.assign(length(), 0);
    BigInteger current;
    for (long long i = length() - 1; i >= 0; --i) {
        current.shift();
        current[0] = data[i];
        while (current.length() > 1 && current[current.length() - 1] == 0) {
            current.data.pop_back();
        }
        int x = 0;
        int left = 0;
        int right = BASE;
        while (left <= right) {
            int middle = (left + right) >> 1;
            BigInteger temp = tempBigInt * middle;
            if (temp > current)
                right = middle - 1;
            else {
                left = middle + 1;
                x = middle;
            }
        }
        result[i] = x;
        current -= tempBigInt * x;
    }
    result.isPositive = (isPositive == bigInt.isPositive);
    *this = result;
    while (length() > 1 && data[length() - 1] == 0) {
        data.pop_back();
    }
    return *this;
}

void BigInteger::divideBy2() {
    int i = length() - 1;
    int temp = data[i] % 2;
    data[i--] /= 2;
    while (i >= 0) {
        if (temp)
            data[i] += 10000;
        temp = data[i] % 2;
        data[i] /= 2;
        --i;
    }
    while (length() > 1 && data[length() - 1] == 0) {
        data.pop_back();
    }
}

class Rational {
private:
    BigInteger numerator = 0;
    BigInteger denominator = 1;

    void reduce();

public:
    Rational() = default;

    Rational(const Rational& rational) = default;

    Rational(const BigInteger& bigInt);

    Rational(int num);

    ~Rational() = default;

    Rational& operator=(const Rational& rational) = default;

    const BigInteger& getNumerator() const { return numerator; }

    const BigInteger& getDenominator() const { return denominator; }

    void checkSign();

    string toString() const;

    string asDecimal() { return asDecimal(0); }

    string asDecimal(size_t precision);

    explicit operator double();

    Rational& operator/=(const Rational& rational);

    Rational& operator*=(const Rational& rational);

    Rational& operator-=(const Rational& rational);

    Rational& operator+=(const Rational& rational);

    Rational operator-() const;

    friend std::istream& operator>>(std::istream& stream, Rational& rational);

    friend std::ostream& operator<<(std::ostream& stream, const Rational& rational);
};

Rational operator+(const Rational& rational1, const Rational& rational2) {
    Rational temp = rational1;
    temp += rational2;
    return temp;
}

Rational operator-(const Rational& rational1, const Rational& rational2) {
    Rational temp = rational1;
    temp -= rational2;
    return temp;
}

Rational operator*(const Rational& rational1, const Rational& rational2) {
    Rational temp = rational1;
    temp *= rational2;
    return temp;
}

Rational operator/(const Rational& rational1, const Rational& rational2) {
    Rational temp = rational1;
    temp /= rational2;
    return temp;
}

bool operator==(const Rational& rational1, const Rational& rational2) {
    return (rational1.getNumerator() == rational2.getNumerator()) &&
           (rational1.getDenominator() == rational2.getDenominator());
}

bool operator!=(const Rational& rational1, const Rational& rational2) {
    return !(rational1 == rational2);
}

bool operator<(const Rational& rational1, const Rational& rational2) {
    return (rational1.getNumerator() * rational2.getDenominator() <
            rational2.getNumerator() * rational1.getDenominator());
}

bool operator>(const Rational& rational1, const Rational& rational2) {
    return (rational2 < rational1);
}

bool operator<=(const Rational& rational1, const Rational& rational2) {
    return !(rational1 > rational2);
}

bool operator>=(const Rational& rational1, const Rational& rational2) {
    return !(rational1 < rational2);
}

Rational::Rational(const BigInteger& bigInt) {
    numerator = bigInt;
    denominator = 1;
}

Rational::Rational(int num) {
    numerator = num;
    denominator = 1;
}

void Rational::checkSign() {
    if (numerator == 0) {
        if (!numerator.isPositive)
            numerator.changeSign();
        if (!denominator.isPositive)
            denominator.changeSign();
    } else if (!numerator.isPositive && !denominator.isPositive) {
        numerator.changeSign();
        denominator.changeSign();
    } else if (!denominator.isPositive) {
        numerator.changeSign();
        denominator.changeSign();
    }
}

string Rational::toString() const {
    string str;
    str += numerator.toString();
    if (denominator != 1) {
        str += "/";
        str += denominator.toString();
    }
    return str;
}

string Rational::asDecimal(size_t precision) {
    string result;
    BigInteger zeros = 1;
    for (size_t i = 0; i < precision; ++i) {
        zeros *= 10;
    }
    BigInteger temp = numerator;
    temp *= zeros;
    temp /= denominator;
    result = temp.toString();
    if (precision == 0)
        return result;
    size_t length;
    if (result[0] == '-')
        length = result.size() - 1;
    else
        length = result.size();
    if (length < precision) {
        string strZeros(precision - length, '0');
        if (result[0] == '-') {
            for (size_t i = 1; i < result.size(); ++i) {
                result[i - 1] = result[i];
            }
            result.pop_back();
            result = "-0." + strZeros + result;
            return result;
        }
        result = "0." + strZeros + result;
        return result;
    }
    if (length == precision) {
        if (result[0] == '-') {
            result[0] = '.';
            result = "-0" + result;
            return result;
        }
        result = "0." + result;
        return result;
    }
    result = result.substr(0, result.size() - precision) + "." + result.substr(result.size() - precision, precision);
    return result;
}

Rational::operator double() {
    double result;
    string str = asDecimal(25);
    result = std::stod(str);
    return result;
}

Rational& Rational::operator/=(const Rational& rational) {
    numerator *= rational.denominator;
    denominator *= rational.numerator;
    checkSign();
    reduce();
    return *this;
}

Rational& Rational::operator*=(const Rational& rational) {
    numerator *= rational.numerator;
    denominator *= rational.denominator;
    checkSign();
    reduce();
    return *this;
}

Rational& Rational::operator-=(const Rational& rational) {
    if (denominator == rational.denominator) {
        numerator -= rational.numerator;
    } else {
        numerator *= rational.denominator;
        numerator -= rational.numerator * denominator;
        denominator *= rational.denominator;
    }
    checkSign();
    reduce();
    return *this;
}

Rational& Rational::operator+=(const Rational& rational) {
    if (denominator == rational.denominator) {
        numerator += rational.numerator;
    } else {
        numerator *= rational.denominator;
        numerator += rational.numerator * denominator;
        denominator *= rational.denominator;
    }
    checkSign();
    reduce();
    return *this;
}

Rational Rational::operator-() const {
    Rational temp = *this;
    temp.checkSign();
    temp.numerator.changeSign();
    return temp;
}

std::istream& operator>>(std::istream& stream, Rational& rational) {
    stream >> rational.numerator;
    rational.denominator = 1;
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Rational& rational) {
    stream << rational.toString();
    return stream;
}

void Rational::reduce() {
    if (numerator == 0) {
        denominator = 1;
        return;
    }
    BigInteger temp1 = numerator;
    if (!temp1.isPositive)
        temp1.changeSign();
    BigInteger temp2 = denominator;
    if (!temp2.isPositive)
        temp2.changeSign();
    BigInteger result = 1;
    while (temp1 != temp2) {
        if (temp1.length() == 1 && (temp1[0] == 0 || temp1[0] == 1))
            break;
        if (temp2.length() == 1 && (temp2[0] == 0 || temp2[0] == 1))
            break;
        if (temp1[0] % 2 == 0 && temp2[0] % 2 == 0) {
            result *= 2;
            temp1.divideBy2();
            temp2.divideBy2();
        } else if (temp1[0] % 2 == 0) {
            temp1.divideBy2();
        } else if (temp2[0] % 2 == 0) {
            temp2.divideBy2();
        } else {
            if (temp1 < temp2)
                temp2 -= temp1;
            else if (temp2 < temp1)
                temp1 -= temp2;
            else
                break;
        }
    }
    if (temp1 < temp2)
        result *= temp1;
    else
        result *= temp2;
    numerator /= result;
    denominator /= result;
}
