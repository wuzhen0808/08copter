#pragma once

namespace a8 {
namespace util {

class String {
public:
    String();

    String(const char *str);
    //String(String &str); // Copy constructor    
    ~String();
    int getLength();

    char *getText();

    static char *copy(const char *str);

private:
    static int const UNKNOWN = -1;
    int length = UNKNOWN;
    char *string;
    void init(char *content, int length);
};

} // namespace util

} // namespace a8
