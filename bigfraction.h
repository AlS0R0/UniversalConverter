#ifndef BIGFRACTION_H
#define BIGFRACTION_H

#include "biginteger.h"

class BigFraction {
 public:
  BigFraction();
  BigFraction(const BigInteger& num, const BigInteger& den);
  BigFraction(const int& num, const int& den);
  explicit BigFraction(const BigInteger& num);
  explicit BigFraction(const int& num);
  BigFraction(const BigFraction& other);
  ~BigFraction();

  BigInteger num() const {
      return num_;
  }

  BigInteger den() const {
    return den_;
  }

  friend BigFraction operator+(BigFraction lhs, const BigFraction& rhs);
  friend BigFraction operator-(BigFraction lhs, const BigFraction& rhs);
  friend BigFraction operator*(BigFraction lhs, const BigFraction& rhs);
  friend BigFraction operator/(BigFraction lhs, const BigFraction& rhs);

  friend BigFraction operator+(BigFraction lhs, const int& rhs);
  friend BigFraction operator+(const int& lhs, BigFraction rhs);
  friend BigFraction operator-(BigFraction lhs, const int& rhs);
  friend BigFraction operator-(const int& lhs, BigFraction rhs);
  friend BigFraction operator*(BigFraction lhs, const int& rhs);
  friend BigFraction operator*(const int& lhs, BigFraction rhs);
  friend BigFraction operator/(BigFraction lhs, const int& rhs);
  friend BigFraction operator/(const int& lhs, BigFraction rhs);

  friend BigFraction operator+(BigFraction lhs, const BigInteger& rhs);
  friend BigFraction operator+(const BigInteger& lhs, BigFraction rhs);
  friend BigFraction operator-(BigFraction lhs, const BigInteger& rhs);
  friend BigFraction operator-(const BigInteger& lhs, BigFraction rhs);
  friend BigFraction operator*(BigFraction lhs, const BigInteger& rhs);
  friend BigFraction operator*(const BigInteger& lhs, BigFraction rhs);
  friend BigFraction operator/(BigFraction lhs, const BigInteger& rhs);
  friend BigFraction operator/(const BigInteger& lhs, BigFraction rhs);

  friend BigFraction& operator+=(BigFraction& lhs, const BigFraction& rhs);
  friend BigFraction& operator+=(BigFraction& lhs, int rhs);
  friend BigFraction& operator-=(BigFraction& lhs, const BigFraction& rhs);
  friend BigFraction& operator-=(BigFraction& lhs, int rhs);
  friend BigFraction& operator*=(BigFraction& lhs, const BigFraction& rhs);
  friend BigFraction& operator*=(BigFraction& lhs, int rhs);
  friend BigFraction& operator/=(BigFraction& lhs, const BigFraction& rhs);
  friend BigFraction& operator/=(BigFraction& lhs, int rhs);

  friend BigFraction& operator+=(BigFraction& lhs, const BigInteger& rhs);
  friend BigFraction& operator-=(BigFraction& lhs, const BigInteger& rhs);
  friend BigFraction& operator*=(BigFraction& lhs, const BigInteger& rhs);
  friend BigFraction& operator/=(BigFraction& lhs, const BigInteger& rhs);

  friend bool operator==(const BigFraction& lhs, const BigFraction& rhs);
  friend bool operator!=(const BigFraction& lhs, const BigFraction& rhs);
  friend bool operator<(const BigFraction& lhs, const BigFraction& rhs);
  friend bool operator>(const BigFraction& lhs, const BigFraction& rhs);
  friend bool operator<=(const BigFraction& lhs, const BigFraction& rhs);
  friend bool operator>=(const BigFraction& lhs, const BigFraction& rhs);

  friend bool operator==(const int& lhs, const BigFraction& rhs);
  friend bool operator!=(const int& lhs, const BigFraction& rhs);
  friend bool operator<(const int& lhs, const BigFraction& rhs);
  friend bool operator>(const int& lhs, const BigFraction& rhs);
  friend bool operator<=(const int& lhs, const BigFraction& rhs);
  friend bool operator>=(const int& lhs, const BigFraction& rhs);

  friend bool operator==(const BigFraction& lhs, const int& rhs);
  friend bool operator!=(const BigFraction& lhs, const int& rhs);
  friend bool operator<(const BigFraction& lhs, const int& rhs);
  friend bool operator>(const BigFraction& lhs, const int& rhs);
  friend bool operator<=(const BigFraction& lhs, const int& rhs);
  friend bool operator>=(const BigFraction& lhs, const int& rhs);

  friend bool operator==(const BigInteger& lhs, const BigFraction& rhs);
  friend bool operator!=(const BigInteger& lhs, const BigFraction& rhs);
  friend bool operator<(const BigInteger& lhs, const BigFraction& rhs);
  friend bool operator>(const BigInteger& lhs, const BigFraction& rhs);
  friend bool operator<=(const BigInteger& lhs, const BigFraction& rhs);
  friend bool operator>=(const BigInteger& lhs, const BigFraction& rhs);

  friend bool operator==(const BigFraction& lhs, const BigInteger& rhs);
  friend bool operator!=(const BigFraction& lhs, const BigInteger& rhs);
  friend bool operator<(const BigFraction& lhs, const BigInteger& rhs);
  friend bool operator>(const BigFraction& lhs, const BigInteger& rhs);
  friend bool operator<=(const BigFraction& lhs, const BigInteger& rhs);
  friend bool operator>=(const BigFraction& lhs, const BigInteger& rhs);

  friend std::ostream& operator<<(std::ostream& out, const BigFraction& fract);

  BigFraction& operator=(const BigFraction& fract);

 private:
  BigInteger num_, den_;

  void reduce();
};

#endif // BIGFRACTION_H
