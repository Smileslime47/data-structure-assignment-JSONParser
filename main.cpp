
#include <iostream>
#include <functional>
#include "Stream.h"
#include "Lexer.h"
using namespace std;

int main() {
    shared_ptr<ifstream> file = make_shared<ifstream>("./input.in");
    FileStream ss(file);
    Lexer x("");
    int t = 10;
    while (t--) {
        cout << ss.get() << endl;
        ss = ss.next();
    }
    return 0;
}
