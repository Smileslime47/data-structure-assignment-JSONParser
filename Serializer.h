//
// Created by pe200012 on 2021/09/23.
//

#ifndef JSONLIB_SERIALIZER_H
#define JSONLIB_SERIALIZER_H

#include <sstream>
#include <variant>
#include <type_traits>
#include "AST.h"

void serialize(std::stringstream &ss, JSONBoolean b) {
    ss << (b.b ? "true" : "false");
}

void serialize(std::stringstream &ss, JSONNull n) {
    ss << "null";
}

void serialize(std::stringstream &ss, JSONNumber n) {
    ss << n.x;
}

void serialize(std::stringstream &ss, const JSONString& s) {
    ss << "\"";
    ss << s.get();
    ss << "\"";
}

void serialize(std::stringstream &ss, JSONArray a) {
    ss << "[";
    for (int i = 0; i != a.arr.size(); ++i) {
        if (i) ss << ",";
        serialize(ss, a.arr[i]);
    }
    ss << "]";
}

void serialize(std::strinstream &ss, JSONObject obj) {
    ss << "{";
    int cnt = 0;
    for (auto &p: obj.m) {
        if (cnt) ss << ",";
        ss << p.first << ":";
        serialize(ss, p.second);
        ++cnt;
    }
    ss << "}";
}

void serialize(std::stringstream &ss, JSONValue v) {
    std::visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::is_same_v<T, JSONNull>) {
            serialize(ss, arg);
        } else if constexpr(std::is_same_v<T, JSONString>) {
            serialize(ss, arg);
        } else if constexpr(std::is_same_v<T, JSONObject>) {
            serialize(ss, arg);
        } else if constexpr(std::is_same_v<T, JSONArray>) {
            serialize(ss, arg);
        } else if constexpr(std::is_same_v<T, JSONBoolean>) {
            serialize(ss, arg);
        } else if constexpr(std::is_same_v<T, JSONNumber>) {
            serialize(ss, arg);
        } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    }, *v.v);
}

#endif //JSONLIB_SERIALIZER_H
