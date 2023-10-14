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

int main() {
    char ch = -1;
    print("uchar",(unsigned char)ch);
    print("int",(int)ch);
    print("int2",(int)(unsigned char)ch);
    
    
}