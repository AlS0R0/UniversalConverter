#include "biginteger.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

// Ограничение на алгоритм Карацубы

static const int KARATSUBA_THRESHOLD = 32;

// Конструкторы

BigInteger::BigInteger() : digits_{0} {};

BigInteger::BigInteger(int x)
{
    if (x == 0) {
        digits_.push_back(0);
        return;
    }

    long long val = (long long)x;

    while (val > 0) {
        digits_.push_back(val % BASE);
        val /= BASE;
    }
}

BigInteger::BigInteger(int64_t x)
{
    if (x == 0) {
        digits_.push_back(0);
        return;
    }

    while (x > 0) {
        digits_.push_back(x % BASE);
        x /= BASE;
    }
}

BigInteger::BigInteger(unsigned long long x)
{
    if (x == 0) {
        digits_.push_back(0);
        return;
    }

    while (x) {
        digits_.push_back(x % BASE);
        x /= BASE;
    }
}

BigInteger::BigInteger(const std::string& s)
{
    ConvertFromString(s);
}

// Перевод из строки

std::string BigInteger::ConvertToString() const
{
    if (TestIfZero()) {
        return "0";
    }

    std::ostringstream s;
    s << digits_.back();

    for (int i = (int)digits_.size() - 2; i >= 0; --i) {
        s << std::setfill('0') << std::setw(9) << digits_[i];
    }
    return s.str();
}

// Перевод в строку

void BigInteger::ConvertFromString(const std::string& s)
{
    digits_.clear();

    if (s.empty()) {
        digits_.push_back(0);
        return;
    }

    int len = (int)s.length();

    for (int i = len; i > 0; i -= 9) {
        int start = std::max(0, i - 9);
        int blockLen = i - start;
        std::string block = s.substr(start, blockLen);
        digits_.push_back(std::stoi(block));
    }

    normalize();
}

// Доп операции

bool BigInteger::TestIfZero() const
{
    for (auto& iter : digits_) {
        if (iter != 0) return false;
    }

    return true;
}

BigInteger gcd(const BigInteger& lhs, const BigInteger& rhs)
{
    BigInteger l = lhs, r = rhs;

    while (!r.TestIfZero()) {
        BigInteger n = l % r;
        l = std::move(r);
        r = std::move(n);
    }

    return l;
}

void BigInteger::normalize()
{
    long long carry = 0;

    for (int i = 0; i < (int)digits_.size() || carry; ++i) {
        if (i >= (int)digits_.size()) {
            digits_.push_back(0);
        }

        long long curr = (long long)digits_[i] + carry;

        carry = curr / BASE;
        digits_[i] = (int)(curr % BASE);
    }

    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }

    if (digits_.empty()) {
        digits_.push_back(0);
    }
}

// Функции для работы как с вектором

size_t BigInteger::size() const
{
    return digits_.size();
}

void BigInteger::push_back(int x)
{
    digits_.push_back(x);
}

void BigInteger::pop_back()
{
    digits_.pop_back();
}

int BigInteger::back() const
{
    return digits_.back();
}

void BigInteger::reserve(size_t x)
{
    digits_.reserve(x);
}

void BigInteger::reverse()
{
    std::reverse(digits_.begin(), digits_.end());
}

void BigInteger::resize(size_t size, int x)
{
    digits_.resize(size, x);
}

const int& BigInteger::operator[](size_t x) const
{
    return digits_[x];
}

int& BigInteger::operator[](size_t x)
{
    return digits_[x];
}

// Сложение

BigInteger operator+(BigInteger lhs, const BigInteger& rhs)
{
    int carry = 0;
    for (size_t i = 0; i < std::max(lhs.size(), rhs.size()) || carry; ++i) {
        if (i >= lhs.size()) lhs.push_back(0);

        long long sum = (long long)lhs[i] + carry + (i < rhs.size() ? rhs[i] : 0);

        lhs[i] = (int)(sum % BASE);
        carry = (int)(sum / BASE);
    }
    lhs.normalize();
    return lhs;
}

BigInteger operator+(BigInteger lhs, const int& rhs)
{
    return lhs + BigInteger(rhs);
}

// Вычитание

BigInteger operator-(BigInteger lhs, const BigInteger& rhs)
{
    if (lhs < rhs) {
        throw std::runtime_error("BigInteger subtraction: lhs < rhs");
    }

    int carry = 0;
    for (size_t i = 0; i < rhs.size() || carry; ++i) {
        long long sub = (long long)lhs[i] - (i < rhs.size() ? rhs[i] : 0) - carry;
        carry = (sub < 0) ? 1 : 0;
        if (carry) sub += BASE;
        lhs[i] = (int)sub;
    }

    lhs.normalize();
    return lhs;
}

// Умножение за квадрат

static BigInteger multiplyNaive(const BigInteger& lhs, const BigInteger& rhs)
{
    BigInteger res;
    res.resize(lhs.size() + rhs.size(), 0);

    for (size_t i = 0; i < lhs.size(); ++i) {

        long long carry = 0;

        for (size_t j = 0; j < rhs.size() || carry; ++j) {

            if (i + j >= res.size()) res.push_back(0);

            long long curr = (long long)res[i + j]
                             + (long long)lhs[i] * (j < rhs.size() ? rhs[j] : 0)
                             + carry;

            res[i + j] = (int)(curr % BASE);
            carry = curr / BASE;
        }
    }

    res.normalize();
    return res;
}

// Карацуба

// Сдвиг влево на k цифр
static BigInteger shiftLeft(const BigInteger& a, size_t k)
{
    if (a.TestIfZero()) return BigInteger(0);

    BigInteger res;
    res.resize(a.size() + k, 0);

    for (size_t i = 0; i < a.size(); ++i)
        res[i + k] = a[i];

    res.normalize();
    return res;
}

// Взять младшие n цифр
static BigInteger low(const BigInteger& a, size_t n)
{
    BigInteger res;
    size_t sz = std::min(a.size(), n);

    for (size_t i = 0; i < sz; ++i)
        res.push_back(a[i]);

    res.normalize();
    return res;
}

// Взять старшие цифры (начиная с позиции n)
static BigInteger high(const BigInteger& a, size_t n)
{
    if (a.size() <= n) return BigInteger(0);

    BigInteger res;

    for (size_t i = n; i < a.size(); ++i)
        res.push_back(a[i]);

    res.normalize();
    return res;
}

BigInteger karatsuba(const BigInteger& lhs, const BigInteger& rhs)
{
    size_t n = std::max(lhs.size(), rhs.size());

    // Для маленьких чисел
    if (n <= (size_t)KARATSUBA_THRESHOLD)
        return multiplyNaive(lhs, rhs);

    size_t half = n / 2;

    BigInteger a0 = low(lhs, half);
    BigInteger a1 = high(lhs, half);
    BigInteger b0 = low(rhs, half);
    BigInteger b1 = high(rhs, half);

    BigInteger z0 = karatsuba(a0, b0);
    BigInteger z2 = karatsuba(a1, b1);
    BigInteger z1 = karatsuba(a0 + a1, b0 + b1) - z0 - z2;

    return shiftLeft(z2, 2 * half) + shiftLeft(z1, half) + z0;
}

// Умножение

BigInteger operator*(BigInteger lhs, const BigInteger& rhs)
{
    size_t n = std::max(lhs.size(), rhs.size());
    if (n <= (size_t)KARATSUBA_THRESHOLD)
        return multiplyNaive(lhs, rhs);
    return karatsuba(lhs, rhs);
}

BigInteger operator*(BigInteger lhs, const int& rhs)
{
    for (size_t i = 0; i < lhs.size() || false; ++i) {
        if (i >= lhs.size()) lhs.push_back(0);
    }

    long long carry = 0;

    for (size_t i = 0; i < lhs.size() || carry; ++i) {
        if (i >= lhs.size()) lhs.push_back(0);
        long long curr = (long long)lhs[i] * rhs + carry;
        lhs[i] = (int)(curr % BASE);
        carry = curr / BASE;
    }

    lhs.normalize();
    return lhs;
}

// Целочисленное деление

BigInteger operator/(BigInteger lhs, const BigInteger& rhs)
{
    if (rhs.TestIfZero()) {
        throw std::runtime_error("Division by zero");
    }
    if (lhs < rhs) {
        return BigInteger(static_cast<int64_t>(0));
    }

    BigInteger res;
    BigInteger carry;

    for (int i = (int)lhs.size() - 1; i >= 0; --i) {
        carry = carry * BASE + lhs[i];

        int lo = 0, hi = BASE - 1, q = 0;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (rhs * mid <= carry) {
                q = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        res.digits_.push_back(q);
        carry = carry - rhs * q;
    }

    res.reverse();
    res.normalize();
    return res;
}

BigInteger operator/(BigInteger lhs, const int& rhs)
{
    if (rhs == 0) {
        throw std::runtime_error("Division by zero");
    }
    if (lhs.TestIfZero()) {
        return lhs;
    }

    BigInteger res;
    long long carry = 0;

    for (int i = (int)lhs.size() - 1; i >= 0; --i) {
        long long curr = (long long)lhs[i] + carry * BASE;
        res.push_back((int)(curr / rhs));
        carry = curr % rhs;
    }

    res.reverse();
    res.normalize();
    return res;
}

// Деление с остатком

BigInteger operator%(BigInteger lhs, const BigInteger& rhs)
{
    if (rhs.TestIfZero()) throw std::runtime_error("Modulo by zero");
    if (lhs < rhs) return lhs;

    // Вычисляем остаток через деление, но с гарантией корректности
    BigInteger quotient = lhs / rhs;
    BigInteger product = quotient * rhs;

    // Убеждаемся, что product <= lhs
    while (product > lhs) {
        quotient = quotient - 1;
        product = product - rhs;
    }

    // Теперь lhs - product точно неотрицательно и меньше rhs
    return lhs - product;
}

BigInteger operator%(BigInteger lhs, const int& rhs) {
    return lhs - (lhs / rhs * rhs);
}

// Операторы присваивания

BigInteger& operator+=(BigInteger& lhs, const BigInteger& rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

BigInteger& operator+=(BigInteger& lhs, const int& rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

BigInteger& operator-=(BigInteger& lhs, const BigInteger& rhs)
{
    lhs = lhs - rhs;
    return lhs;
}

BigInteger& operator-=(BigInteger& lhs, const int& rhs)
{
    lhs = lhs - rhs;
    return lhs;
}

BigInteger& operator*=(BigInteger& lhs, const BigInteger& rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

BigInteger& operator*=(BigInteger& lhs, const int& rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

BigInteger& operator/=(BigInteger& lhs, const BigInteger& rhs)
{
    lhs = lhs / rhs;
    return lhs;
}

BigInteger& operator/=(BigInteger& lhs, const int& rhs)
{
    lhs = lhs / rhs;
    return lhs;
}

// Операторы сравнения

bool operator<(const BigInteger& lhs, const BigInteger& rhs)
{
    if (lhs.size() != rhs.size()) return lhs.size() < rhs.size();

    for (int i = (int)lhs.size() - 1; i >= 0; --i) {
        if (lhs[i] < rhs[i]) return true;
        else if (lhs[i] > rhs[i]) return false;
    }

    return false;
}

bool operator<=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs > rhs);
}

bool operator>(const BigInteger& lhs, const BigInteger& rhs)
{
    return rhs < lhs;
}

bool operator>=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs < rhs);
}

bool operator==(const BigInteger& lhs, const BigInteger& rhs)
{
    return (lhs >= rhs) && (lhs <= rhs);
}

bool operator!=(const BigInteger& lhs, const BigInteger& rhs)
{
    return !(lhs == rhs);
}

bool operator<(const BigInteger& lhs, const int& rhs)
{
    return lhs < BigInteger(rhs);
}

bool operator<=(const BigInteger& lhs, const int& rhs)
{
    return lhs <= BigInteger(rhs);
}

bool operator>(const BigInteger& lhs, const int& rhs)
{
    return lhs > BigInteger(rhs);
}

bool operator>=(const BigInteger& lhs, const int& rhs)
{
    return lhs >= BigInteger(rhs);
}

bool operator==(const BigInteger& lhs, const int& rhs)
{
    return lhs == BigInteger(rhs);
}

bool operator!=(const BigInteger& lhs, const int& rhs)
{
    return lhs != BigInteger(rhs);
}

bool operator<(const int& lhs, const BigInteger& rhs)
{
    return rhs > lhs;
}

bool operator<=(const int& lhs, const BigInteger& rhs)
{
    return rhs >= lhs;
}

bool operator>(const int& lhs, const BigInteger& rhs)
{
    return rhs < lhs;
}

bool operator>=(const int& lhs, const BigInteger& rhs)
{
    return rhs <= lhs;
}

bool operator==(const int& lhs, const BigInteger& rhs)
{
    return rhs == lhs;
}

bool operator!=(const int& lhs, const BigInteger& rhs)
{
    return rhs != lhs;
}

// Вывод в поток

std::ostream& operator<<(std::ostream& out, const BigInteger& num)
{
    const std::vector<int>& digits = num.digits_;

    if (digits.empty()) { out << 0; return out; }

    out << digits.back();
    for (int i = (int)digits.size() - 2; i >= 0; --i) {
        out << std::setfill('0') << std::setw(9) << digits[i];
    }
    return out;
}

// Обычное присваивание

BigInteger& BigInteger::operator=(const BigInteger& integer)
{
    digits_ = integer.digits_;
    normalize();
    return *this;
}

BigInteger& BigInteger::operator=(const int& integer)
{
    *this = BigInteger(integer);
    return *this;
}