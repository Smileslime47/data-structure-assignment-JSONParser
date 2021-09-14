//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_LEXER_H
#define JSONLIB_LEXER_H

#include <unordered_map>
#include "Stream.h"

static const unordered_map<char, Token> table = {
        {'(',  Token{Token::Type::LeftParenthesis, "("}},
        {')',  Token{Token::Type::RightParenthesis, ")"}},
        {'[',  Token{Token::Type::LeftBracket, "["}},
        {']',  Token{Token::Type::RightBracket, "]"}},
        {'{',  Token{Token::Type::LeftBrace, "{"}},
        {'}',  Token{Token::Type::RightBrace, "}"}},
        {' ',  Token{Token::Type::Whitespace, " "}},
        {'"',  Token{Token::Type::Quote, "\""}},
        {'/',  Token{Token::Type::Solidus, "/"}},
        {'\\', Token{Token::Type::Backslash, "\\"}},
        {':',  Token{Token::Type::Colon, ":"}}
};

struct Lexer {

    struct TokenStream : public Stream<Token, TokenStream> {

        explicit TokenStream(shared_ptr<ifstream> input, Lexer &_parent) : ss(move(input)), parent(_parent) {
            x = parent.nextToken(ss);
        }

        Token get() const override {
            return x;
        }

        bool exhausted() const override {
            return ss.exhausted();
        }

        TokenStream next() override {
            x = parent.nextToken(ss);
            //
        }

    private:
        FileStream ss;
        Token x;
        Lexer &parent;
    };

    Token nextToken(FileStream &ss) {
        char cur = ss.get();
        if ('0' <= cur && cur <= '9') return Token{Token::Type::Digit, string(cur)};
    }

    explicit Lexer(const string &path) : ts(make_shared<ifstream>(path), *this) {}

private:
    TokenStream ts;
};



#endif //JSONLIB_LEXER_H
