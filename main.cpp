
#include <iostream>
#include <functional>
using namespace std;

template <typename T>
struct Stream {
    T x;
    Stream<T> next() const;
};

template <>
struct Stream<int> {
    int x;
    explicit Stream<int>(int y) : x(y) {}
    Stream<int> next() const {
        return Stream<int>{x+1};
    }
};

template <>
struct Stream<char> {
    char x;
    explicit Stream<char>(char y) : x(y) {}
    static Stream<char> next() {
        return Stream<char>{static_cast<char>(getchar())};
    }
};

int main() {
    Stream<char> x(' ');
    int t = 10;
    while (t--) {
        cout << x.x;
        x = x.next();
    }
    return 0;
}
