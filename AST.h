//
// Created by pe200012 on 2021/09/20.
//

#ifndef JSONLIB_AST_H
#define JSONLIB_AST_H

#include <vector>
#include <string>
#include <map>
#include <optional>
#include <variant>
#include <memory>
#include <type_traits>

template<class> inline constexpr bool always_false_v = false;

enum class JSONType {
    jsnull,
    jsboolean,
    jsarray,
    jsobject,
    jsstring,
    jsnumber
};

class JSONNull;

class JSONBoolean;

class JSONArray;

class JSONObject;

class JSONString;

class JSONNumber;

class JSONValue {
public:
    shared_ptr<variant < JSONNull, JSONBoolean, JSONArray, JSONObject, JSONString, JSONNumber>> v;

    template<typename T>
    void put(T x) {
        v = make_shared<variant < JSONNull, JSONBoolean, JSONArray, JSONObject, JSONString, JSONNumber>>
        (x);
    }
};

class JSONNull : public JSONValue {

};

class JSONBoolean : public JSONValue {
public:
    bool b;
    explicit JSONBoolean(bool _b) : b(_b){}
};

class JSONArray : public JSONValue {
public:
    std::vector<JSONValue> arr;
    void push_back(const JSONValue &v) {
        arr.push_back(v);
    }
};

class JSONObject : public JSONValue {
public:
    std::map<string, JSONValue> m;
    void insert(const string &key, const JSONValue &value) {
        m[key] = value;
    }

    optional <JSONValue> lookup(const string &key) {
        if (m.find(key) == m.end()) {
            return {};
        } else {
            return {m[key]};
        }
    }
};

class JSONNumber : public JSONValue {
public:
    double x;
};

class JSONString : public JSONValue {
public:
    std::string s;
    void append(const std::string &t) {
        s += t;
    }

    [[nodiscard]] std::string get() const {
        return s;
    }
};

optional <JSONType> type(const JSONValue &val) {
    if (!(val.v.operator bool())) return {};
    optional <JSONType> x;
    std::visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::is_same_v<T, JSONNull>) {
            x = {JSONType::jsnull};
        } else if constexpr(std::is_same_v<T, JSONString>) {
            x = {JSONType::jsstring};
        } else if constexpr(std::is_same_v<T, JSONObject>) {
            x = {JSONType::jsobject};
        } else if constexpr(std::is_same_v<T, JSONArray>) {
            x = {JSONType::jsarray};
        } else if constexpr(std::is_same_v<T, JSONBoolean>) {
            x = {JSONType::jsboolean};
        } else if constexpr(std::is_same_v<T, JSONNumber>) {
            x = {JSONType::jsnumber};
        } else {
            static_assert(always_false_v<T>, "non-exhaustive visitor!");
        }
    }, *val.v);
    return x;
}

#endif //JSONLIB_AST_H
