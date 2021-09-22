//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_TOKEN_H
#define JSONLIB_TOKEN_H

#include <string>

struct Token {
    enum class Type {
        Digit,
        Whitespace,
        Letter,
        LeftBracket,
        RightBracket,
        LeftParenthesis,
        RightParenthesis,
        LeftBrace,
        RightBrace,
        Quote,
        Solidus,
        Backslash,
        Colon,
        Comma,

        Backspace,
        Formfeed,
        Linefeed,
        CarriageReturn,
        HorizontalTab,

        Unicode
    };
    Type type;
    std::string value;
};


#endif //JSONLIB_TOKEN_H
