
#include <iostream>
#include <functional>
#include "Stream.h"
#include "Lexer.h"
#include "Map.h"
#include "Parser.h"
using namespace std;

int main() {
    Lexer x("./input.in");
    Parser p(x);
    p.parseBoolean();
    return 0;
}
