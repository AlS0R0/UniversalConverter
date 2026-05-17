#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <vector>
#include <string>

static const int BASE = 1000000000;

class BigInteger
{
public:
    BigInteger();
    BigInteger(int x);
    BigInteger(unsigned long long x);
    BigInteger(int64_t x);
    BigInteger(const std::string& s);

    std::string ConvertToString();
    void ConvertFromString(const std::string& s);
    bool TestIfZero() const;
    friend BigInteger gcd(const BigInteger& lhs, const BigInteger& rhs);
    void normalize();

    size_t size() const;

    void push_back(int x);
    void pop_back();
    int back();
    void reserve(size_t x);
    void reverse();
    void resize(size_t size, int x);

    int& operator[](size_t x);
    const int& operator[](size_t x) const;

    friend BigInteger operator+(BigInteger lhs, const BigInteger& rhs);
    friend BigInteger operator+(BigInteger lhs, const int& rhs);
    friend BigInteger operator-(BigInteger lhs, const BigInteger& rhs);
    friend BigInteger operator*(BigInteger lhs, const BigInteger& rhs);
    friend BigInteger operator*(BigInteger lhs, const int& rhs);
    friend BigInteger operator/(BigInteger lhs, const BigInteger& rhs);
    friend BigInteger operator/(BigInteger lhs, const int& rhs);
    friend BigInteger operator%(BigInteger lhs, const BigInteger& rhs);
    friend BigInteger operator%(BigInteger lhs, const int& rhs);
    friend BigInteger karatsuba(const BigInteger& lhs, const BigInteger& rhs);

    friend BigInteger& operator+=(BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger& operator+=(BigInteger& lhs, const int& rhs);
    friend BigInteger& operator-=(BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger& operator-=(BigInteger& lhs, const int& rhs);
    friend BigInteger& operator*=(BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger& operator*=(BigInteger& lhs, const int& rhs);
    friend BigInteger& operator/=(BigInteger& lhs, const BigInteger& rhs);
    friend BigInteger& operator/=(BigInteger& lhs, const int& rhs);

    friend bool operator<(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator<=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator>=(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator==(const BigInteger& lhs, const BigInteger& rhs);
    friend bool operator!=(const BigInteger& lhs, const BigInteger& rhs);
    
    friend bool operator<(const BigInteger& lhs, const int& rhs);
    friend bool operator<=(const BigInteger& lhs, const int& rhs);
    friend bool operator>(const BigInteger& lhs, const int& rhs);
    friend bool operator>=(const BigInteger& lhs, const int& rhs);
    friend bool operator==(const BigInteger& lhs, const int& rhs);
    friend bool operator!=(const BigInteger& lhs, const int& rhs);

    friend bool operator<(const int& lhs, const BigInteger& rhs);
    friend bool operator<=(const int& lhs, const BigInteger& rhs);
    friend bool operator>(const int& lhs, const BigInteger& rhs);
    friend bool operator>=(const int& lhs, const BigInteger& rhs);
    friend bool operator==(const int& lhs, const BigInteger& rhs);
    friend bool operator!=(const int& lhs, const BigInteger& rhs);

    friend std::ostream& operator<<(std::ostream& out, const BigInteger& num);

    BigInteger& operator=(const BigInteger& integer);

private:

    std::vector<int> digits_;
};

#endif // BIGINTEGER_H
