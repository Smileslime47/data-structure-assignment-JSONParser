
#include <iostream>
#include <functional>
#include "Stream.h"
#include "Lexer.h"
using namespace std;

int main() {
    Lexer x("./input.in");
    while (!x.ts.exhausted()) {
        auto y = x.ts.get();
        x.ts = x.ts.next();
    }
    return 0;
}
