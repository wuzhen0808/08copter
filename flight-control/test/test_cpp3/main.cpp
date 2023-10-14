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
    short int16 = 0;
    char ch = -128;

    print("ch:", ch);
    print("(int)ch:", (int)ch);
    print("(unsigned char)ch:", (unsigned char)ch);
    print("(unsigned short)ch:", (unsigned short)ch);
    print("(short)(unsigned short)ch:", (short)(unsigned short)ch);
    print("(unsigned int)ch:", (unsigned int)ch);
    print("(int)(unsigned int)ch:", (int)(unsigned int)ch);
    print("(unsigned short)(unsigned char)ch:", (unsigned short)(unsigned char)ch);
    print("(unsigned int)(unsigned char)ch:", (unsigned int)(unsigned char)ch);
    //
    print("ch:", ch);
    print("ch<<8:", ch << 8);
    print("(int)ch:", (int)ch);
    print("((int)ch<<8:):", ((int)ch << 8));
    print("(long long)ch:", (long long)ch);
    print("(long long)ch<<8:", (long long)ch << 8);
    print("(ch<<8:):", (ch << 8));
    print("((unsigned char)ch<<8:):", ((unsigned char)ch << 8));
    print("(int)(ch<<8:):", (int)(ch << 8));
    print("(int)((unsigned char)ch<<8:):", (int)((unsigned char)ch << 8));
    print("(ch<<32:):", (ch << 32));
    print("(ch>>32:):", (ch >> 32));
    print("((long long)ch<<32:):", ((long long)ch << 32));
    print("(ch<<8:):", (ch << 8));
}