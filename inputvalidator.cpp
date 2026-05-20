#include "inputvalidator.h"

bool InputValidator::validate(int p, int q,
                              const std::vector<DigitTokenizer::Token>& tokens,
                              std::string& error)
{
    if (p < 2 || p > 500) {
        error = "Основание исходной системы должно быть целым числом от 2 до 500.";
        return false;
    }
    
    if (q < 2 || q > 500) {
        error = "Основание целевой системы должно быть целым числом от 2 до 500.";
        return false;
    }

    if (tokens.empty()) {
        error = "Входная строка пуста.";
        return false;
    }

    const int S_INT = 0;
    const int S_AFTER_DOT = 1;
    const int S_FRAC = 2;
    const int S_AFTER_LPAREN = 3;
    const int S_PERIOD = 4;

    int state = S_INT;

    bool dotSeen = false;
    bool lparenSeen = false;
    bool rparenSeen = false;
    int digitCountAfterDot = 0;
    int digitCountInPeriod = 0;
    bool intPartExists = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const DigitTokenizer::Token& tok = tokens[i];

        if (tok.type == 'd') {
                if (tok.value >= p) {
                error = "Цифра со значением " + std::to_string(tok.value) +
                        " недопустима в системе с основанием " + std::to_string(p) + ".";
                return false;
            }

            if (state == S_INT) {

                intPartExists = true;
            } else if (state == S_AFTER_DOT) {

                state = S_FRAC;
                digitCountAfterDot++;
            } else if (state == S_FRAC) {

                digitCountAfterDot++;
            } else if (state == S_AFTER_LPAREN) {

                state = S_PERIOD;
                digitCountInPeriod++;
            }   else if (state == S_PERIOD) {

                digitCountInPeriod++;
            } else {

                error = "Некорректное расположение цифры.";
                return false;
            }

        } else if (tok.type == '.') {

            if (dotSeen) {
                error = "Более одной точки в числе.";
                return false;
            }

            if (state != S_INT) {
                error = "Точка может находиться только после целой части.";
                return false;
            }

            dotSeen = true;
            state = S_AFTER_DOT;

        } else if (tok.type == '(') {

            if (lparenSeen) {
                error = "Более одной открывающей скобки.";
                return false;
            }

            if (!dotSeen) {
                error = "Период может быть только в дробной части.";
                return false;
            }

            if (state != S_FRAC && state != S_AFTER_DOT) {
                error = "Некорректное расположение '('.";
                return false;
            }

            lparenSeen = true;
            state = S_AFTER_LPAREN;

        } else if (tok.type == ')') {

            if (!lparenSeen) {
                error = "Закрывающая скобка без открывающей.";
                return false;
            }

            if (state != S_PERIOD) {
                error = "Некорректное расположение ')'.";
                return false;
            }

            rparenSeen = true;

            if (i + 1 < tokens.size()) {
                error = "После закрывающей скобки периода не должно быть других символов.";
                return false;
            }

            break;

        } else {
            error = "Неизвестный тип токена.";
            return false;
        }
    }

    if (lparenSeen && !rparenSeen) {
        error = "Незакрытая скобка периода.";
        return false;
    }

    if (lparenSeen && digitCountInPeriod == 0) {
        error = "Период не может быть пустым.";
        return false;
    }

    if (dotSeen && state == S_AFTER_DOT) {
        error = "После точки должна быть хотя бы одна цифра или период.";
        return false;
    }

    if (!dotSeen && (lparenSeen || rparenSeen)) {
        error = "Скобки периода могут быть только в дробной части.";
        return false;
    }

    if (!intPartExists) {
        error = "Отсутствует целая часть числа.";
        return false;
    }

    return true;
}
