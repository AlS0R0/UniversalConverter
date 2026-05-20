#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <string>
#include <vector>
#include "digittokenizer.h"

class InputValidator
{
public:
    static bool validate(int p, int q,
                         const std::vector<DigitTokenizer::Token>& tokens,
                         std::string& error);
};

#endif // INPUTVALIDATOR_H
