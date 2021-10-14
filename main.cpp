
#include <iostream>
#include <functional>
#include <variant>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

template <typename Target>
void visitValue(JSONValue &v, function<void(Target)> f) {
    std::visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Target>) {
            f(arg);
        }
    }, *(v.v));
}

int main() {
    Lexer x("./test.json");
    Parser p(x);
    auto j = p.parse();
    visitValue<JSONObject>(j,[](JSONObject y) -> void {
        visitValue<JSONString>(y.m["name"], [](const JSONString &n) {
            cout << "name: " << n.get() << '\n';
        });
        visitValue<JSONNumber>(y.m["id"], [](const JSONNumber &id) {
            cout << "id: " << (int)id.x << '\n';
        });
        visitValue<JSONBoolean>(y.m["protected"], [](const JSONBoolean &pro) {
            cout << "protected: " << std::boolalpha << pro.b << '\n';
        });
    });
    return 0;
}
