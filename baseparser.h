#ifndef BASEPARSER_H
#define BASEPARSER_H

#include "bigfraction.h"
#include "digittokenizer.h"
#include <string>
#include <vector>

class BaseParser
{
public:
    static BigFraction parse(const std::vector<DigitTokenizer::Token>& tokens,
                             int p);
};

#endif // BASEPARSER_H
