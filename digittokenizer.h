#ifndef DIGITTOKENIZER_H
#define DIGITTOKENIZER_H

#include <string>

class DigitTokenizer
{
public:
    struct Token {
        char type;
        int value;
    };

    explicit DigitTokenizer(const std::string& input);
    
    bool nextToken(Token& tok);
    std::string getLastError() const { return error_; }
    bool isEnd() const { return pos_ >= input_.size(); }
    void reset();

private:
    std::string input_;
    size_t pos_;
    std::string error_;

    void skipWhitespace();
    bool parseBracketDigit(int& value);
    bool isDigitChar(char c) const;
    int charToValue(char c) const;
};

#endif // DIGITTOKENIZER_H
