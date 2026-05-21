#include "baseparser.h"
#include "biginteger.h"
#include <vector>

static BigInteger powBase(int base, size_t exp)
{
    BigInteger result(1);

    for (size_t i = 0; i < exp; ++i) {
        result = result * base;
    }

    return result;
}

static BigInteger digitsToBigInteger(const std::vector<int>& digits, int base)
{
    BigInteger result(0);

    for (int d : digits) {
        result = result * base + d;
    }

    return result;
}

BigFraction BaseParser::parse(const std::vector<DigitTokenizer::Token>& tokens,
                              int p)
{
    std::vector<int> intDigits;
    std::vector<int> fracDigits;
    std::vector<int> periodDigits;

    const int ST_INT = 0;
    const int ST_FRAC = 1;
    const int ST_PERIOD = 2;

    int state = ST_INT;
    bool dotSeen = false;
    bool parenOpen = false;

    for (const auto& tok : tokens) {

        if (tok.type == 'd') {
            if (state == ST_INT) {
                intDigits.push_back(tok.value);
            } else if (state == ST_FRAC) {
                fracDigits.push_back(tok.value);
            } else {
                periodDigits.push_back(tok.value);
            }
        } else if (tok.type == '.') {
            dotSeen = true;
            state = ST_FRAC;
        } else if (tok.type == '(') {
            parenOpen = true;
            state = ST_PERIOD;
        }
    }

    BigInteger I = digitsToBigInteger(intDigits, p);
    BigInteger F = digitsToBigInteger(fracDigits, p);
    BigInteger P = digitsToBigInteger(periodDigits, p);

    size_t k = fracDigits.size();
    size_t m = periodDigits.size();

    if (m == 0) {

        if (k == 0) {
            return BigFraction(I, BigInteger(1));
        } else {
            BigInteger denom = powBase(p, k);
            BigInteger num = I * denom + F;
            return BigFraction(num, denom);
        }

    } else {

        BigInteger pow_p_k = powBase(p, k);
        BigInteger pow_p_m = powBase(p, m);
        BigInteger factor = pow_p_m - 1;

        BigInteger num = I * pow_p_k * factor + F * factor + P;
        BigInteger denom = pow_p_k * factor;
        return BigFraction(num, denom);
    }
}
