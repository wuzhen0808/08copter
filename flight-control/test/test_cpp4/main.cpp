#include "a8/util/BitUtil.h"
#include <iostream>
using namespace a8::util;
using namespace std;
template <typename T>
void print(String title, T value) {
    String bin;
    BitUtil::toBinary<T>(value, bin);
    title.setLength(' ', 38);
    cout << title.text() << bin.text() << endl;
}

template <typename F, typename T>
void printFloat(F f) {

    T i = *reinterpret_cast<T *>(&f);
    String str;
    BitUtil::toBinary<T>(i, str);
    cout << str.text() << endl;
}

int main() {

    printFloat<float, int>(1.0f);
    printFloat<float, int>(-1.0f);
    printFloat<float, int>(1.0e-1f);
    printFloat<float, int>(-1.0e-1f);

    printFloat<float64, int64>(1.0);
    printFloat<float64, int64>(-1.0);
    printFloat<float64, int64>(1.0e-1);
    printFloat<float64, int64>(-1.0e-1);
}