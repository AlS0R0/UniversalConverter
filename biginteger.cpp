#include "biginteger.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

BigInteger::BigInteger() : digits_{0} {};

BigInteger::BigInteger(int x)
{
    int rest;

    while (x) {
        rest = x % BASE;
        digits_.push_back(rest);
        x /= BASE;
    }
}


BigInteger::BigInteger(int64_t x)
{
    int rest;

    while (x) {
        rest = x % BASE;
        digits_.push_back(rest);
        x /= BASE;
    }
}

BigInteger::BigInteger(unsigned long long x)
{
    int rest;

    while (x) {
        rest = x % BASE;
        digits_.push_back(rest);
        x /= BASE;
    }
}

BigInteger::BigInteger(const std::string& s)
{
    for (long long i = s.length() - 10;; i -= 9) {
        if (i < 0) {
            digits_.push_back(std::stoi(s.substr(0, 9 + i)));
            break;
        }

        digits_.push_back(std::stoi(s.substr(i, 9)));
    }
}

std::string BigInteger::ConvertToString()
{
    if (TestIfZero()) {
        return "0";
    }

    std::ostringstream s;

    s << digits_.back();

    for (int i = digits_.size() - 2; i >= 0; --i) {
        s << std::setfill('0') << std::setw(9) << digits_[i];
    }
    
    return s.str();
}

void BigInteger::ConvertFromString(const std::string& s)
{
    if (TestIfZero()) digits_.clear();

    for (long long i = s.length() - 10;; i -= 9) {
        if (i <= 0) {
            digits_.push_back(std::stoi(s.substr(0, 9 + i)));
            break;
        }
        digits_.push_back(std::stoi(s.substr(i, 9)));
    }
}

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

    for (int i = 0; i < digits_.size() || carry; ++i) {
        if (i >= digits_.size()) {
            digits_.push_back(0);
        }

        long long curr = digits_[i] + carry;
        carry = curr / BASE;
        digits_[i] = curr % BASE;
    }
    
    while (digits_.size() > 1 && digits_.back() == 0) {
        digits_.pop_back();
    }
    
    if (digits_.empty()) {
        digits_.push_back(0);
    }
}

size_t BigInteger::size() const{
    return digits_.size();
}

void BigInteger::push_back(int x) {
    digits_.push_back(x);
}

void BigInteger::pop_back() {
    digits_.pop_back();
}

int BigInteger::back() {
    return digits_.back();
}

void BigInteger::reserve(size_t x) {
    digits_.reserve(x);
}

void BigInteger::reverse() {
    std::reverse(digits_.begin(), digits_.end());
}

void BigInteger::resize(size_t size, int x) {
    digits_.resize(size, x);
}

const int& BigInteger::operator[](size_t x) const {
    return digits_[x];
}

int& BigInteger::operator[](size_t x) {
    return digits_[x];
}

BigInteger operator+(BigInteger lhs, const BigInteger& rhs)
{
    for (int i = 0, carry = 0;
            i < std::max(lhs.size(), rhs.size()) || carry; ++i) {
        if (i >= lhs.size()) lhs.push_back(0);

        lhs[i] += carry + (i >= rhs.size() ? 0 : rhs[i]);
        carry = lhs[i] >= BASE;

        if (carry) lhs[i] -= BASE;
    }

    lhs.normalize();
    return lhs;
}

BigInteger operator+(BigInteger lhs, const int& rhs)
{
    return lhs + BigInteger(static_cast<int64_t>(rhs));
}

BigInteger operator-(BigInteger lhs, const BigInteger& rhs)
{
    for (int i = 0, carry = 0; i < rhs.size() || carry; ++i) {
        lhs[i] -= (i >= rhs.size() ? 0 : rhs[i]) + carry;
        carry = lhs[i] < 0;
        if (carry) lhs[i] += BASE;
    }

    //while (lhs.size() > 1 && lhs.back() == 0) {
    //    lhs.pop_back();
    //}

    lhs.normalize();
    return lhs;
}

BigInteger operator*(BigInteger lhs, const BigInteger& rhs)
{
    BigInteger res;
    res.resize(lhs.size() + rhs.size(), 0);

    for (int i = 0; i < lhs.size(); ++i) {
        for (int j = 0, carry = 0; j < rhs.size() || carry; ++j) {
            long long curr = carry + res[i + j] + lhs[i]
                * (j >= rhs.size() ? 0 : rhs[j]);
            res[i + j] = curr % BASE;
            carry = curr / BASE;
        }
    }

    //while (res.size() > 1 && res.back() == 0) {
    //    res.pop_back();
    //}

    res.normalize();
    return res;
}

BigInteger operator*(BigInteger lhs, const int& rhs)
{
    for (int i = 0, carry = 0; i < lhs.size() || carry; ++i) {
        if (i >= lhs.size()) lhs.push_back(0);

        long long curr = lhs[i] * rhs + carry;
        lhs[i] = curr % BASE;
        carry = curr / BASE;
    }

    //while (lhs.size() > 1 && lhs.back() == 0) {
    //    lhs.pop_back();
    //}

    lhs.normalize();
    return lhs;
}

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
    
    for (int i = lhs.digits_.size(); i-- > 0; ) {
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
        carry = carry - (rhs * q);
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

    for (int i = lhs.size() - 1, carry = 0; i >= 0; --i) {
        long long curr = lhs[i] + carry * BASE;
        carry = curr % rhs;
        res.push_back(curr / rhs);
    }

    res.reverse();

    //while (res.size() > 1 && res.back() == 0) {
    //    res.pop_back();
    //}

    res.normalize();
    return res;
}

BigInteger operator%(BigInteger lhs, const BigInteger& rhs)
{
    return lhs - (lhs / rhs * rhs);
}

BigInteger operator%(BigInteger lhs, const int& rhs) {
    return lhs - (lhs / rhs * rhs);
}

BigInteger karatsuba(const BigInteger& lhs, const BigInteger& rhs)
{
    // потом
    return BigInteger();
}

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

bool operator<(const BigInteger& lhs, const BigInteger& rhs)
{
    if (lhs.size() < rhs.size()) return true;
    else if (lhs.size() > rhs.size()) return false;
    else {
        for (int i = lhs.size() - 1; i >= 0; --i) {
            if (lhs[i] < rhs[i]) return true;
        }
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


std::ostream& operator<<(std::ostream& out, const BigInteger& num)
{
    const std::vector<int>& digits = num.digits_;

    if (digits.empty()) {
        out << 0;
        return out;
    }

    out << digits.back();

    for (int i = digits.size() - 2; i >= 0; --i) {
        out << std::setfill('0') << std::setw(9) << digits[i];
    }

    return out;
}

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
