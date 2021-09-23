//
// Created by pe200012 on 2021/09/20.
//

#ifndef JSONLIB_PARSER_H
#define JSONLIB_PARSER_H

#include <utility>
#include <algorithm>

#include "Lexer.h"
#include "AST.h"

class ParseException : public std::exception {

};

struct UnexpectedTokenException : public ParseException {
    explicit UnexpectedTokenException(Token n) : t(std::move(n)) {}

    [[nodiscard]] char const *what() const noexcept override {
        return t.value.c_str();
    }

    Token t;
};

namespace ParserUtil {
    static const std::vector<std::string> table = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

    template<typename T>
    T qpow(T a, int b) {
        if (!b) return 1;
        if (b == 1) return a;
        auto ans = qpow(a, b / 2);
        if (b & 1) return ans * ans * a;
        else return ans * ans;
    }
}

class Parser {
    Lexer ll;
public:
    explicit Parser(Lexer &_ll) : ll(_ll) {}

    JSONValue parse() {
        using namespace ParserUtil;
        parseWhitespace();
        Token cur = ll.ts.get();
        JSONValue ans;
        if (cur.value == "\"") {
            auto x = parseString();
            ans.put(x);
            return ans;
        } else if (cur.value == "{") {
            auto x = parseObject();
            ans.put(x);
            return ans;
        } else if (cur.value == "[") {
            auto x = parseArray();
            ans.put(x);
            return ans;
        } else if (cur.value == "f" || cur.value == "t") {
            auto x = parseBoolean();
            ans.put(x);
            return ans;
        } else if (cur.value == "n") {
            auto x = parseNULL();
            ans.put(x);
            return ans;
        } else if (cur.value == "-" || find(table.begin(), table.end(), cur.value) != table.end()) {
            auto x = parseNumber();
            ans.put(x);
            return ans;
        } else {
            throw UnexpectedTokenException(cur);
        }
    }

    JSONString parseString() {
        Token cur = ll.ts.get();
        if (cur.value != "\"") {
            throw UnexpectedTokenException(cur);
        }
        JSONString ans;
        ll.ts = ll.ts.next();
        cur = ll.ts.get();
        while (cur.value != "\"") {
            ans.append(cur.value);
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
        }
        if (!ll.ts.exhausted())
            ll.ts = ll.ts.next();
        return ans;
    }

    string parseWhitespace() {
        string ans;
        Token cur = ll.ts.get();
        while (cur.value == " " || cur.value == "\n" || cur.value == "\r" || cur.value == "\t") {
            ans += cur.value;
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
        }
        return ans;
    }

    JSONNull parseNULL() {
        Token cur;
        char null[] = "null";
        for (int i = 0; i != 4; ++i) {
            cur = ll.ts.get();
            if (cur.value[0] != null[i]) throw UnexpectedTokenException(cur);
            ll.ts = ll.ts.next();
        }
        return {};
    };

    JSONBoolean parseBoolean() {
        Token cur = ll.ts.get();
        if (cur.value == "t") {
            char tr[] = "true";
            for (int i = 0; i != 4; ++i) {
                cur = ll.ts.get();
                if (cur.value[0] != tr[i]) throw UnexpectedTokenException(cur);
                ll.ts = ll.ts.next();
            }
            return JSONTrue();
        } else if (cur.value == "f") {
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

    JSONArray parseArray() {
        Token cur = ll.ts.get();
        if (cur.value != "[") throw UnexpectedTokenException(cur);
        ll.ts = ll.ts.next();
        JSONArray arr;
        parseWhitespace();
        if (cur.value == "]") return arr;
        arr.push_back(parse());
        cur = ll.ts.get();
        while (cur.value == ",") {
            ll.ts = ll.ts.next();
            arr.push_back(parse());
            cur = ll.ts.get();
        }
        cur = ll.ts.get();
        if (cur.value != "]") {
            throw UnexpectedTokenException(cur);
        }
        if (!ll.ts.exhausted())
            ll.ts = ll.ts.next();
        return arr;
    }

    JSONNumber parseNumber() {
        using namespace ParserUtil;
        int s = 1;
        JSONNumber ans{};
        Token cur = ll.ts.get();
        if (cur.value == "-") {
            s = -1;
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
        }
        if (cur.value == "0") {
            ans.x = 0;
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
        } else {
            while (std::find(table.begin(), table.end(), cur.value) != table.end()) {
                ans.x *= 10;
                ans.x += cur.value[0] - '0';
                if (ll.ts.exhausted()) break;
                ll.ts = ll.ts.next();
                cur = ll.ts.get();
            }
        }
        if (cur.value == ".") {
            double x = 0;
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
            while (std::find(table.begin(), table.end(), cur.value) != table.end()) {
                x *= 10;
                x += cur.value[0] - '0';
                ll.ts = ll.ts.next();
                cur = ll.ts.get();
            }
            while (x >= 1) x /= 10;
            ans.x += x;
        }
        if (cur.value == "e" || cur.value == "E") {
            int s2 = 1, expo = 0;
            ll.ts = ll.ts.next();
            cur = ll.ts.get();
            if (cur.value == "-") {
                s2 = -1;
                ll.ts = ll.ts.next();
                cur = ll.ts.get();
            } else if (cur.value == "+") {
                ll.ts = ll.ts.next();
                cur = ll.ts.get();
            }
            while ('0' <= cur.value[0] && cur.value[0] <= '9') {
                expo *= 10;
                expo += cur.value[0] - '0';
                if (ll.ts.exhausted()) break;
                ll.ts = ll.ts.next();
                cur = ll.ts.get();
            }
            if (s2 > 0) {
                ans.x *= qpow(10, expo);
            } else {
                ans.x /= qpow(10, expo);
            }
        }
        ans.x *= s;
        return ans;
    }

    JSONObject parseObject() {
        Token cur = ll.ts.get();
        if (cur.value != "{") throw UnexpectedTokenException(cur);
        ll.ts = ll.ts.next();
        parseWhitespace();
        JSONObject ans;
        cur = ll.ts.get();
        if (cur.value == "}") return ans;
        else {
            string key;
            JSONValue value;
            key = parseString().get();
            parseWhitespace();
            cur = ll.ts.get();
            if (cur.value != ":") throw UnexpectedTokenException(cur);
            ll.ts = ll.ts.next();
            value = parse();
            ans.insert(key, value);
            cur = ll.ts.get();
            while (cur.value == ",") {
                ll.ts = ll.ts.next();
                parseWhitespace();
                key = parseString().get();
                parseWhitespace();
                cur = ll.ts.get();
                if (cur.value != ":") throw UnexpectedTokenException(cur);
                ll.ts = ll.ts.next();
                value = parse();
                ans.insert(key, value);
                cur = ll.ts.get();
            }
            if (cur.value != "}") throw UnexpectedTokenException(cur);
            if (!ll.ts.exhausted())
                ll.ts = ll.ts.next();
            return ans;
        }
    };
};

#endif //JSONLIB_PARSER_H
