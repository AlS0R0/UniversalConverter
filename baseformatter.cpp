#include "baseformatter.h"
#include "biginteger.h"
#include <map>
#include <algorithm>
#include <sstream>

static std::string digitToString(int d)
{
    if (d >= 0 && d <= 9) {
        return std::string(1, '0' + d);
    } else if (d >= 10 && d <= 35) {
        return std::string(1, 'A' + (d - 10));
    } else {
        return "[" + std::to_string(d) + "]";
    }
}

static std::string formatIntegerPart(const BigInteger& intPart, int base)
{
    if (intPart.TestIfZero()) {
        return "0";
    }

    std::vector<int> digits;
    BigInteger n = intPart;

    while (!n.TestIfZero()) {

        int digit = (n % base).ConvertToString()[0] - '0';
        BigInteger remBig = n % base;

        int digitVal = std::stoi(remBig.ConvertToString());
        digits.push_back(digitVal);

        n = n / base;
    }

    std::reverse(digits.begin(), digits.end());
    std::string result;

    for (int d : digits) {
        result += digitToString(d);
    }

    return result;
}

std::string BaseFormatter::format(const BigFraction& fraction, int q,
                                  bool& truncated, std::string& truncationMsg)
{
    truncated = false;
    truncationMsg.clear();

    BigInteger intPart = fraction.num() / fraction.den();
    BigInteger remainder = fraction.num() % fraction.den();

    std::string intStr = formatIntegerPart(intPart, q);

    if (intStr.length() > MAX_OUTPUT_LENGTH) {
        truncated = true;
        truncationMsg = "Целая часть результата слишком длинная.";
        return intStr.substr(0, MAX_OUTPUT_LENGTH) + "...";
    }

    if (remainder.TestIfZero()) {
        return intStr;
    }

    size_t remaining = MAX_OUTPUT_LENGTH - intStr.length() - 1;

    if (remaining == 0) {

        truncated = true;
        truncationMsg = "Полная запись результата слишком длинная. Показаны первые " +
                        std::to_string(MAX_OUTPUT_LENGTH) + " символов.";

        return intStr + ".";
    }

    std::vector<int> fracDigits;
    std::map<BigInteger, size_t> remainderPos;
    size_t pos = 0;
    BigInteger rem = remainder;

    while (!rem.TestIfZero() && remaining > 0) {

        auto it = remainderPos.find(rem);

        if (it != remainderPos.end()) {

            size_t periodStart = it->second;
            std::string result = intStr + ".";

            for (size_t i = 0; i < periodStart; ++i) {
                result += digitToString(fracDigits[i]);
            }

            result += "(";

            for (size_t i = periodStart; i < fracDigits.size(); ++i) {
                result += digitToString(fracDigits[i]);
            }

            result += ")";

            if (result.length() > MAX_OUTPUT_LENGTH) {
                truncated = true;
                truncationMsg = "Полная запись результата слишком длинная. Показаны первые " +
                                std::to_string(MAX_OUTPUT_LENGTH) + " символов.";
                result = result.substr(0, MAX_OUTPUT_LENGTH) + "...";
            }

            return result;
        }

        remainderPos[rem] = pos;
        rem = rem * q;
        BigInteger digitBig = rem / fraction.den();
        int digit = std::stoi(digitBig.ConvertToString());
        rem = rem % fraction.den();

        std::string digitStr = digitToString(digit);

        if (digitStr.length() > remaining) {
            truncated = true;

            truncationMsg = "Полная запись результата слишком длинная. Показаны первые " +
                            std::to_string(MAX_OUTPUT_LENGTH) + " символов.";

            std::string result = intStr + ".";

            for (size_t i = 0; i < fracDigits.size(); ++i) {
                result += digitToString(fracDigits[i]);
            }

            result += "...";
            return result;
        }

        fracDigits.push_back(digit);
        remaining -= digitStr.length();
        ++pos;
    }

    if (rem.TestIfZero()) {

        std::string result = intStr + ".";

        for (int d : fracDigits) {
            result += digitToString(d);
        }

        if (result.length() > MAX_OUTPUT_LENGTH) {
            truncated = true;
            truncationMsg = "Полная запись результата слишком длинная. Показаны первые " +
                            std::to_string(MAX_OUTPUT_LENGTH) + " символов.";
            result = result.substr(0, MAX_OUTPUT_LENGTH) + "...";
        }

        return result;

    } else {

        truncated = true;
        truncationMsg = "Полная запись результата слишком длинная. Показаны первые " +
                        std::to_string(MAX_OUTPUT_LENGTH) + " символов.";
        std::string result = intStr + ".";

        for (int d : fracDigits) {
            result += digitToString(d);
        }

        result += "...";
        return result;
    }
}
