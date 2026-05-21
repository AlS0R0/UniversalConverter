#ifndef BASEFORMATTER_H
#define BASEFORMATTER_H

#include "bigfraction.h"
#include <string>

const size_t MAX_OUTPUT_LENGTH = 100000;

class BaseFormatter
{
public:
    static std::string format(const BigFraction& fraction, int q,
                              bool& truncated, std::string& truncationMsg);
};

#endif // BASEFORMATTER_H
