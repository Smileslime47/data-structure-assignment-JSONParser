//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_TOKEN_H
#define JSONLIB_TOKEN_H

#include <string>

struct Token {
    enum class Type {
        Digit,            // 数字
        Whitespace,       // 空格
        Letter,           // 字母
        LeftBracket,      // 左方括号
        RightBracket,     // 右方括号
        LeftParenthesis,  // 左小括号
        RightParenthesis, // 右小括号
        LeftBrace,        // 左大括号
        RightBrace,       // 右大括号
        Quote,            // 双引号
        Solidus,          // 斜杠
        Backslash,        // 反斜杠
        Colon,            // 冒号
        Comma,            // 逗号

        Backspace,        // 退格
        Formfeed,         // 换页
        Linefeed,         // 换行
        CarriageReturn,   // 回车
        HorizontalTab,    // 水平制表符

        Unicode           // Unicode
    };
    Type type;
    std::string value;    // 值
};


#endif //JSONLIB_TOKEN_H
