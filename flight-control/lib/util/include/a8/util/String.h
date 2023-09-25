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
    static int getLength(char *str) {
        for (int i = 0;; i++) {
            if (str[i] == '\0') {
                return i;
            }
        }
    }

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
    void append(const char *str);
    void append(const char *str, int len);
    bool endWith(const char *str);

    String &operator=(const char *buf);
    String &operator=(const String &str);
    String &operator=(const int iValue);
    /*
    Use format "%e" to format the float value.
    For example 1.1 will convert to '1.100000e+00';

    */
    String &operator=(const float fValue);
    String &operator=(const Buffer<char> &iValue);

    // static member funcs
    static int getLength(const char *str);
    static void copy(const char *str, int from1, int len, char *buff, int from2);

    template <typename... Args>
    static String format(const char formatStr[], Args... args) {

        int inc = 100;
        int size = inc;

        while (true) {
            char *buf = new char[size]{0};
            int len = snprintf(buf, size, formatStr, args...);
            if (len > 0) {
                String ret(buf, len);
                delete[] buf;
                return ret; // success
            }
            delete[] buf;
            size += inc;
        }
    }

    // operators
    friend String operator+(const String &str1, const String &str);
    friend bool operator==(const String &str1, const String &str2);
};

bool operator==(const String &str1, const String &str2);

} // namespace util

} // namespace a8
