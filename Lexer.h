//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_LEXER_H
#define JSONLIB_LEXER_H

#include <unordered_map>
#include "Stream.h"

class LexerException : public std::exception {
};

class UnexpectedTokenException : public LexerException {
};

class InvalidHexadecimal : public std::exception {
};

namespace LexerUtil {
    static const unordered_map<char, Token> table = {
            {'(',  Token{Token::Type::LeftParenthesis, "("}},
            {')',  Token{Token::Type::RightParenthesis, ")"}},
            {'[',  Token{Token::Type::LeftBracket, "["}},
            {']',  Token{Token::Type::RightBracket, "]"}},
            {'{',  Token{Token::Type::LeftBrace, "{"}},
            {'}',  Token{Token::Type::RightBrace, "}"}},
            {'"',  Token{Token::Type::Quote, "\'"}},
            {'/',  Token{Token::Type::Solidus, "/"}},
            {'\\', Token{Token::Type::Backslash, "\\"}},
            {':',  Token{Token::Type::Colon, ":"}},
            {'\b', Token{Token::Type::Backspace, "\b"}},
            {'\f', Token{Token::Type::Formfeed, "\f"}},
            {'\n', Token{Token::Type::Linefeed, "\n"}},
            {'\r', Token{Token::Type::CarriageReturn, "\r"}},
            {'\t', Token{Token::Type::HorizontalTab, "\t"}},
    };
    static const unordered_map<char, Token> space = {
            {' ',  Token{Token::Type::Whitespace, " "}},
            {'\r', Token{Token::Type::Whitespace, "\r"}},
            {'\n', Token{Token::Type::Whitespace, "\n"}},
            {'\t', Token{Token::Type::Whitespace, "\t"}},

    };
    static const char hexadecimal[] = "0123456789ABCDEF";

    template<typename T>
    T fromHex(char c) {
        T r = lower_bound(hexadecimal, hexadecimal + 16, c) - hexadecimal;
        if (r == 16) throw InvalidHexadecimal();
        return r;
    }
}


struct Lexer {

    struct TokenStream : public Stream<Token, TokenStream> {

        TokenStream(shared_ptr<ifstream> input, Lexer *_parent) : ss(move(input)), parent(_parent) {
            x = parent->nextToken(ss);
        }

        TokenStream(FileStream &input, Lexer *_parent) : ss(input), parent(_parent) {
            x = parent->nextToken(ss);
        }

        Token get() const override {
            return x;
        }

        bool exhausted() const noexcept override {
            return ss.exhausted();
        }

        TokenStream next() override {
            return {ss, parent};
        }

    private:
        FileStream ss;
        Token x;
        Lexer *parent;
    };

    Token nextToken(FileStream &ss) {
        using namespace LexerUtil;
        char cur = ss.get();
        string cur2;
        while (!withinString && space.find(cur) != space.end()) {
            ss = ss.next();
            cur = ss.get();
        }
        cur2.push_back(cur);
        ss = ss.next();
        if ('0' <= cur && cur <= '9') return Token{Token::Type::Digit, cur2};
        if (('a' <= cur && cur <= 'z') || ('A' <= cur && cur <= 'Z')) return Token{Token::Type::Letter, cur2};
        if (cur == '"') withinString = !withinString;
        // TODO: judge if the character is a unicode codepoint?
        if (table.find(cur) != table.end() ) return table.at(cur);
        if (space.find(cur) != space.end()) return space.at(cur);
        return Token{Token::Type::Unicode, cur2};
    }

    explicit Lexer(const string &path) : ts(make_shared<ifstream>(path), this) {}

    TokenStream ts;
private:
    bool withinString = false;
};


#endif //JSONLIB_LEXER_H
