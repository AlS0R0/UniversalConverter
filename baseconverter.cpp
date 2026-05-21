#include "baseconverter.h"
#include "inputvalidator.h"
#include "baseparser.h"
#include "baseformatter.h"

bool BaseConverter::convert(const std::string& input, int p, int q,
                            std::string& output, std::string& error)
{
    bool tokenOk;
    std::string tokenErr;
    auto tokens = DigitTokenizer::tokenize(input, tokenOk, tokenErr);

    if (!tokenOk) {
        error = "Ошибка лексического анализа: " + tokenErr;
        return false;
    }

    if (!InputValidator::validate(p, q, tokens, error)) {
        return false;
    }

    BigFraction fraction;

    try {
        fraction = BaseParser::parse(tokens, p);
    } catch (const std::exception& e) {
        error = "Ошибка преобразования в дробь: " + std::string(e.what());
        return false;
    } catch () {
        error = "Неизвестная ошибка при парсинге числа.";
        return false;
    }

    bool truncated;
    std::string truncMsg;
    output = BaseFormatter::format(fraction, q, truncated, truncMsg);
    
    if (truncated) {
        error = truncMsg;
        return true;
    }

    error.clear();
    return true;
}
