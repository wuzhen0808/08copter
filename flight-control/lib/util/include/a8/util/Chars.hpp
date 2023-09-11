#ifndef Chars__
#define Chars__
#include "a8/util/Buffer.hpp"
#include "a8/util/Shared.hpp"
namespace a8 {
namespace util {

class Chars {
public:
    static int getLength(char *string);
    Chars();
    Chars(char *str);
    Chars(Chars& chars);
    ~Chars();
    int getLength();
    Chars *append(char *string, int len); // return this
    Chars *append(char *string);          // return this
    Chars *append(int value);             // return this
    Chars *append(float value);           // return this
    Chars *append(Chars *string);         // return this

    char *getChars(); // in Chars's lifecycle

private:
    Buffer<char> *buffer;
};
} // namespace util

} // namespace a8

#endif