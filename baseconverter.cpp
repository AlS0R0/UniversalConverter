#include "baseconverter.h"
#include "digittokenizer.h"
#include "inputvalidator.h"
#include "baseparser.h"
#include "baseformatter.h"
#include "bigfraction.h"
#include <vector>

bool BaseConverter::convert(const std::string& input, int p, int q,
                            std::string& output, std::string& warning)
{
    warning.clear();
    output.clear();

    DigitTokenizer tokenizer(input);
    std::vector<DigitTokenizer::Token> tokens;
    DigitTokenizer::Token tok;
    while (tokenizer.nextToken(tok)) {
        tokens.push_back(tok);
    }
    if (!tokenizer.getLastError().empty()) {
        warning = "Ошибка токенизации: " + tokenizer.getLastError();
        return false;
    }

    std::string validationError;
    if (!InputValidator::validate(p, q, tokens, validationError)) {
        warning = validationError;
        return false;
    }

    BigFraction fraction;
    try {
        fraction = BaseParser::parse(tokens, p);
    } catch (const std::exception& e) {
        warning = "Ошибка парсинга: " + std::string(e.what());
        return false;
    } catch (...) {
        warning = "Неизвестная ошибка при парсинге.";
        return false;
    }

    bool truncated;
    std::string truncMsg;
    output = BaseFormatter::format(fraction, q, truncated, truncMsg);

    if (truncated) {
        warning = truncMsg;
    }
    return true;
}
