#include "digittokenizer.h"
#include <cctype>

DigitTokenizer::DigitTokenizer(const std::string& input)
    : input_(input), pos_(0), error_() {}

void DigitTokenizer::reset()
{
    pos_ = 0;
    error_.clear();
}

void DigitTokenizer::skipWhitespace()
{
    while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
        ++pos_;
    }
}

bool DigitTokenizer::isDigitChar(char c) const
{
    return std::isdigit(static_cast<unsigned char>(c)) ||
           std::isalpha(static_cast<unsigned char>(c));
}

int DigitTokenizer::charToValue(char c) const
{
    if (std::isdigit(static_cast<unsigned char>(c))) {
        return c - '0';
    }

    return std::toupper(c) - 'A' + 10;
}

bool DigitTokenizer::parseBracketDigit(int& value)
{
    ++pos_;

    if (pos_ >= input_.size()) {
        error_ = "Незакрытая квадратная скобка";
        return false;
    }

    std::string numStr;

    while (pos_ < input_.size() && std::isdigit(static_cast<unsigned char>(input_[pos_]))) {
        numStr += input_[pos_];
        ++pos_;
    }

    if (pos_ >= input_.size() || input_[pos_] != ']') {
        error_ = "Незакрытая квадратная скобка";
        return false;
    }

    ++pos_;

    if (numStr.empty()) {
        error_ = "Пустые квадратные скобки [] не допускаются";
        return false;
    }

    try {
        value = std::stoi(numStr);

        if (value < 0) {
            error_ = "Значение цифры не может быть отрицательным";
            return false;
        }
    } catch (...) {
        error_ = "Некорректное число в квадратных скобках";
        return false;
    }

    return true;
}

bool DigitTokenizer::nextToken(Token& tok)
{
    skipWhitespace();

    if (pos_ >= input_.size()) {
        return false;
    }

    char c = input_[pos_];

    if (c == '.') {
        tok.type = '.';
        tok.value = 0;
        ++pos_;
        return true;
    }

    if (c == '(') {
        tok.type = '(';
        tok.value = 0;
        ++pos_;
        return true;
    }

    if (c == ')') {
        tok.type = ')';
        tok.value = 0;
        ++pos_;
        return true;
    }

    if (c == '[') {
        int val;

        if (!parseBracketDigit(val)) {
            return false;
        }

        tok.type = 'd';
        tok.value = val;
        return true;
    }

    if (isDigitChar(c)) {
        tok.type = 'd';
        tok.value = charToValue(c);
        ++pos_;
        return true;
    }

    error_ = std::string("Недопустимый символ: ") + c;

    return false;
}
