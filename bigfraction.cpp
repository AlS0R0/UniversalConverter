#include "bigfraction.h"
#include "biginteger.h"

BigInteger lcm(const BigInteger& first_number, const BigInteger& second_number) {
    return (first_number * second_number) / gcd(first_number, second_number);
}

void BigFraction::reduce() {
    if (den_ == BigInteger(0)) throw "Denominator is zero";

    BigInteger gcd_ = gcd(num_, den_);
    num_ /= gcd_;
    den_ /= gcd_;
}

BigFraction::BigFraction() : num_(0), den_(1) {}

// конструктор от BigInteger

BigFraction::BigFraction(const BigInteger& num) : num_(num), den_(1) {}

BigFraction::BigFraction(const BigInteger& num, const BigInteger& den)
    : num_(num), den_(den) {

    if (den_ == 0) throw "Denominator is zero";

    this->reduce();
}

// конструктор от int

BigFraction::BigFraction(const int& num, const int& den) : num_(num), den_(den) {

    if (den_ == 0) throw "Denominator is zero";

    this->reduce();
}

BigFraction::BigFraction(const int& num) : num_(num), den_(1) {}

// конструктор от BigFraction

BigFraction::BigFraction(const BigFraction& other) : num_(other.num_),
    den_(other.den_) {
    this->reduce();
}

// деструктор

BigFraction::~BigFraction() {
    num_ = 0;
    den_ = 1;
}

// оператор присваивания

BigFraction& BigFraction::operator=(const BigFraction& fract) {

    num_ = fract.num_;
    den_ = fract.den_;
    reduce();

    return *this;
}

// арифметические операторы для BigFraction и BigFraction

BigFraction operator+(BigFraction lhs, const BigFraction& rhs) {

    BigInteger lcm_ = lcm(lhs.den_, rhs.den_);
    lhs.num_ = lhs.num_ * (lcm_ / lhs.den_) + rhs.num_ * (lcm_ / rhs.den_);
    lhs.den_ = lcm_;

    lhs.reduce();
    return lhs;
}

BigFraction operator-(BigFraction lhs, const BigFraction& rhs) {

    BigInteger lcm_ = lcm(lhs.den_, rhs.den_);
    lhs.num_ = lhs.num_ * (lcm_ / lhs.den_) - rhs.num_ * (lcm_ / rhs.den_);
    lhs.den_ = lcm_;

    lhs.reduce();
    return lhs;
}

BigFraction operator*(BigFraction lhs, const BigFraction& rhs) {

    lhs.num_ *= rhs.num_;
    lhs.den_ *= rhs.den_;

    lhs.reduce();
    return lhs;
}

BigFraction operator/(BigFraction lhs, const BigFraction& rhs) {

    if (rhs.num_ == 0) {
        lhs.num_ = 0;
        lhs.den_ = 1;

        return lhs;
    }

    lhs.num_ *= rhs.den_;
    lhs.den_ *= rhs.num_;

    lhs.reduce();
    return lhs;
}

// арифметические операторы для BigFraction и int

BigFraction operator+(BigFraction lhs, const int& rhs) {

    lhs.num_ += rhs * lhs.den_;

    lhs.reduce();
    return lhs;
}

BigFraction operator+(const int& lhs, BigFraction rhs) {

    rhs.num_ += lhs * rhs.den_;

    rhs.reduce();
    return rhs;
}

BigFraction operator-(BigFraction lhs, const int& rhs) {

    lhs.num_ -= rhs * lhs.den_;

    lhs.reduce();
    return lhs;
}

BigFraction operator-(const int& lhs, BigFraction rhs) {

    rhs.num_ = lhs * rhs.den_ - rhs.num_;

    rhs.reduce();
    return rhs;
}

BigFraction operator*(BigFraction lhs, const int& rhs) {

    lhs.num_ *= rhs;

    lhs.reduce();
    return lhs;
}

BigFraction operator*(const int& lhs, BigFraction rhs) {

    rhs.num_ *= lhs;

    rhs.reduce();
    return rhs;
}

BigFraction operator/(BigFraction lhs, const int& rhs) {

    lhs.den_ *= rhs;

    lhs.reduce();
    return lhs;
}

BigFraction operator/(const int& lhs, BigFraction rhs) {
    return BigFraction(lhs) / rhs;
}

// арифметические операторы для BigFraction и BigInteger

BigFraction operator+(BigFraction lhs, const BigInteger& rhs) {

    lhs.num_ += rhs * lhs.den_;

    lhs.reduce();
    return lhs;
}

BigFraction operator+(const BigInteger& lhs, BigFraction rhs) {

    rhs.num_ += lhs * rhs.den_;

    rhs.reduce();
    return rhs;
}

BigFraction operator-(BigFraction lhs, const BigInteger& rhs) {

    lhs.num_ -= rhs * lhs.den_;

    lhs.reduce();
    return lhs;
}

BigFraction operator-(const BigInteger& lhs, BigFraction rhs) {

    rhs.num_ = lhs * rhs.den_ - rhs.num_;

    rhs.reduce();
    return rhs;
}

BigFraction operator*(BigFraction lhs, const BigInteger& rhs) {

    lhs.num_ *= rhs;

    lhs.reduce();
    return lhs;
}

BigFraction operator*(const BigInteger& lhs, BigFraction rhs) {

    rhs.num_ *= lhs;

    rhs.reduce();
    return rhs;
}

BigFraction operator/(BigFraction lhs, const BigInteger& rhs) {

    lhs.den_ *= rhs;

    lhs.reduce();
    return lhs;
}

BigFraction operator/(const BigInteger& lhs, BigFraction rhs) {
    return BigFraction(lhs) / rhs;
}


// арифметические операторы с присваиванием для BigFraction
// и int / BigFraction и BigFraction

BigFraction& operator+=(BigFraction& lhs, const BigFraction& rhs) {

    lhs = lhs + rhs;
    return lhs;
}


BigFraction& operator+=(BigFraction& lhs, int rhs) {

    lhs = lhs + rhs;
    return lhs;
}

BigFraction& operator-=(BigFraction& lhs, const BigFraction& rhs) {

    lhs = lhs - rhs;
    return lhs;
}

BigFraction& operator-=(BigFraction& lhs, int rhs) {

    lhs = lhs - rhs;
    return lhs;
}

BigFraction& operator*=(BigFraction& lhs, const BigFraction& rhs) {

    lhs = lhs * rhs;
    return lhs;
}

BigFraction& operator*=(BigFraction& lhs, int rhs) {

    lhs = lhs * rhs;
    return lhs;
}

BigFraction& operator/=(BigFraction& lhs, const BigFraction& rhs) {

    lhs = lhs / rhs;
    return lhs;
}

BigFraction& operator/=(BigFraction& lhs, int rhs) {

    lhs = lhs / rhs;
    return lhs;
}

// арифметические операторы с присваиванием для BigFraction и BigInteger

BigFraction& operator+=(BigFraction& lhs, const BigInteger& rhs) {

    lhs = lhs + rhs;
    return lhs;
}

BigFraction& operator-=(BigFraction& lhs, const BigInteger& rhs) {

    lhs = lhs - rhs;
    return lhs;
}

BigFraction& operator*=(BigFraction& lhs, const BigInteger& rhs) {

    lhs = lhs * rhs;
    return lhs;
}

BigFraction& operator/=(BigFraction& lhs, const BigInteger& rhs) {

    lhs = lhs / rhs;
    return lhs;
}


//  сравнение для BigFraction и BigFraction

bool operator==(const BigFraction& lhs, const BigFraction& rhs) {
    return lhs.num_ == rhs.num_ && lhs.den_ == rhs.den_;
}

bool operator!=(const BigFraction& lhs, const BigFraction& rhs) {
    return !(lhs == rhs);
}

bool operator<(const BigFraction& lhs, const BigFraction& rhs) {
    return lhs.num_ * rhs.den_ < rhs.num_ * lhs.den_;
}

bool operator>(const BigFraction& lhs, const BigFraction& rhs) {
    return rhs < lhs;
}

bool operator<=(const BigFraction& lhs, const BigFraction& rhs) {
    return !(rhs < lhs);
}

bool operator>=(const BigFraction& lhs, const BigFraction& rhs) {
    return !(lhs < rhs);
}


// сравнение для BigFraction и int

bool operator==(const int& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) == rhs;
}

bool operator!=(const int& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) != rhs;
}

bool operator<(const int& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) < rhs;
}

bool operator>(const int& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) > rhs;
}

bool operator<=(const int& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) <= rhs;
}

bool operator>=(const int& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) >= rhs;
}



bool operator==(const BigFraction& lhs, const int& rhs) {
    return lhs == BigFraction(rhs);
}

bool operator!=(const BigFraction& lhs, const int& rhs) {
    return lhs != BigFraction(rhs);
}

bool operator<(const BigFraction& lhs, const int& rhs) {
    return lhs < BigFraction(rhs);
}

bool operator>(const BigFraction& lhs, const int& rhs) {
    return lhs > BigFraction(rhs);
}

bool operator<=(const BigFraction& lhs, const int& rhs) {
    return lhs <= BigFraction(rhs);
}

bool operator>=(const BigFraction& lhs, const int& rhs) {
    return lhs >= BigFraction(rhs);
}

// сравнение для BigInteger и BigFraction

bool operator==(const BigInteger& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) == rhs;
}

bool operator!=(const BigInteger& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) != rhs;
}

bool operator<(const BigInteger& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) < rhs;
}

bool operator>(const BigInteger& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) > rhs;
}

bool operator<=(const BigInteger& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) <= rhs;
}

bool operator>=(const BigInteger& lhs, const BigFraction& rhs) {
    return BigFraction(lhs) >= rhs;
}



bool operator==(const BigFraction& lhs, const BigInteger& rhs) {
    return lhs == BigFraction(rhs);
}

bool operator!=(const BigFraction& lhs, const BigInteger& rhs) {
    return lhs != BigFraction(rhs);
}

bool operator<(const BigFraction& lhs, const BigInteger& rhs) {
    return lhs < BigFraction(rhs);
}

bool operator>(const BigFraction& lhs, const BigInteger& rhs) {
    return lhs > BigFraction(rhs);
}

bool operator<=(const BigFraction& lhs, const BigInteger& rhs) {
    return lhs <= BigFraction(rhs);
}

bool operator>=(const BigFraction& lhs, const BigInteger& rhs) {
    return lhs >= BigFraction(rhs);
}


// вывод BigFraction

std::ostream& operator<<(std::ostream& out, const BigFraction& fract) {

    out << fract.num_;
    out << fract.den_;
    return out;
}