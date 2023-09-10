#ifndef Chars__
#define Chars__
#include "a8/util/Buffer.hpp"

namespace a8 {
namespace util {

class Chars {
public:
    static int getLength(char * string);    
    
    Chars(char* str);
    ~Chars();
    int getLength();
    Chars * append(char * string, int len);// return this
    Chars * append(char * string);// return this
    Chars * append(int value);// return this
    Chars * append(float value);// return this
    Chars * append(Chars * string);// return this

    char * getChars();//same with String's lifecycle
    
private:
    Buffer<char> * buffer;
};
} // namespace util

} // namespace a8

#endif