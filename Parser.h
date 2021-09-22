//
// Created by pe200012 on 2021/09/20.
//

#ifndef JSONLIB_PARSER_H
#define JSONLIB_PARSER_H

#include <utility>

#include "Lexer.h"
#include "AST.h"

class ParseException : public std::exception {

};

struct UnexpectedTokenException : public ParseException {
    explicit UnexpectedTokenException(Token n) : t(std::move(n)) {}
    [[nodiscard]] char const* what() const noexcept override {
        return t.value.c_str();
    }
    Token t;
};

class Parser {
    Lexer ll;
public:
    explicit Parser(Lexer &_ll) : ll(_ll) {}
    JSONValue parse() {

    }
    string parseWhitespace() {
        string ans;
        Token cur = ll.ts.get();
        while(cur.value == " " || cur.value == "\n" || cur.value == "\r" || cur.value == "\t") {
            ans += cur.value;
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
        }
        return ans;
    }
    void parseNULL() {
        Token cur;
        char null[]="null";
        for (int i = 0; i != 4; ++i) {
            cur=ll.ts.get();
            if(cur.value[0]!=null[i]) throw UnexpectedTokenException(cur);
            ll.ts=ll.ts.next();
        }
    };
    JSONBoolean parseBoolean() {
        Token cur=ll.ts.get();
        if(cur.value == "t") {
            char tr[] = "true";
            for (int i = 0; i != 4; ++i) {
                cur = ll.ts.get();
                if (cur.value[0] != tr[i]) throw UnexpectedTokenException(cur);
                ll.ts = ll.ts.next();
            }
            return JSONTrue();
        }
        else if(cur.value == "f") {
            char fl[] = "false";
            for (int i = 0; i != 5; ++i) {
                cur = ll.ts.get();
                if (cur.value[0] != fl[i]) throw UnexpectedTokenException(cur);
                ll.ts = ll.ts.next();
            }
            return JSONFalse();
        } else {
            throw UnexpectedTokenException(cur);
        }
    }
};

#endif //JSONLIB_PARSER_H
