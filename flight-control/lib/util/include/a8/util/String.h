#pragma once
#include "a8/util/Buffer.h"
#include <stdio.h>
namespace a8 {
namespace util {

class String {

private:
    int length;
    char *text;
    void replace(const char *buf, int len, bool deleteText);

public:
    String();
    String(const char (&buf)[]);
    String(const char *str, int len);
    String(const String &str); // Copy constructor
    String(const Buffer<char> &buf);
    ~String();

    char *getText() const;
    char getChar(int idx) const;
    int getLength() const;
    void append(const String *str);
    void append(const String &str);
    void append(const char *str);
    void append(const char *str, int len);
    void append(const char ch);
    void append(const float fValue);
    void append(const double fValue);
    void append(const int fValue);
    void append(const long fValue);
    void append(const unsigned fValue);

    //
    // operators
    String &operator=(const char *buf);
    // String &operator=(const char (&buf)[]); DO not declare this method, will error of ambiguous.
    String &operator=(const String &str);
    String &operator=(const char &buf);
    String &operator=(const int iValue);
    /*
    Use format "%e" to format the float value.
    For example 1.1 will convert to '1.100000e+00';
    */
    String &operator=(const float fValue);
    String &operator=(const Buffer<char> &iValue);

    String &operator<<(const float fValue);
    String &operator<<(const double fValue);
    String &operator<<(const int fValue);
    String &operator<<(const long fValue);
    String &operator<<(const char *buf);
    String &operator<<(const char buf);
    String &operator<<(const unsigned uValue);
    String &operator<<(const String &str);

    // Other methods
    bool endWith(const char *str);
    Buffer<String> split(const char separator);

    // static member funcs
    static int getLength(const char *str);
    static void copy(const char *str, int from1, int len, char *buff, int from2);

    template <typename... Args>
    static String format(const char formatStr[], Args... args) {

        int size = 100;
        char *buf = new char[size]{0};
        int len = snprintf(buf, size, formatStr, args...);
        if (len + 1 > size) { // str truncate.
            delete[] buf;
            size = len + 1;
            buf = new char[size];
            len = snprintf(buf, size, formatStr, args...);
            if (len >= size) {
                // error processing?
                // exit(1);
            }
        }
        String ret(buf, len);
        delete[] buf;
        return ret;
    }

    // Friend operators
    friend String operator+(const String &str1, const String &str);
    friend bool operator==(const String &str1, const String &str2);
};

bool operator==(const String &str1, const String &str2);

} // namespace util

} // namespace a8
