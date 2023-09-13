#pragma once

namespace a8 {
namespace util {

class String {
    
private:
    static int const UNKNOWN = -1;
    int length = UNKNOWN;
    char *text;
    
public:
    String();
    String(const char buf[]);
    String(const char *str, int len);
    String(const String &str); // Copy constructor
    ~String();

    char *getText() const;
    char getChar(int idx) const;
    int getLength() const;

    // static member funcs
    static int getLength(const char *str);
    static void copy(const char *str, int from1, int len, char *buff, int from2);

    // operators
    String& operator=(const char str1[]);
    friend String operator+(const String &str1, const String &str);
    friend bool operator==(const String &str1, const String &str2);

};

bool operator==(const String &str1, const String &str2);

} // namespace util

} // namespace a8
