
#include <iostream>
#include <functional>
#include "Stream.h"
#include "Lexer.h"
#include "Map.h"
#include "Parser.h"
#include <variant>
using namespace std;

template<typename E>
constexpr auto toUnderlyingType(E e)
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

int main() {
    Lexer x("./input.in");
    JSONBoolean z{true};
    cout << z.b ;
    return 0;
}
