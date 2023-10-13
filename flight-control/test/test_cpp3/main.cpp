#include <iostream>

int main() {
    short int16 = 0;
    char ch = -128;

    std::cout << ch << std::endl;
    std::cout << (int)ch << std::endl;
    std::cout << (unsigned char)ch << std::endl;
    std::cout << (unsigned short)ch << std::endl;
    std::cout << (short)(unsigned short)ch << std::endl;
    std::cout << (unsigned int)ch << std::endl;
    std::cout << (int)(unsigned int)ch << std::endl;
    std::cout << (unsigned short)(unsigned char)ch << std::endl;
    std::cout << (unsigned int)(unsigned char)ch << std::endl;
}