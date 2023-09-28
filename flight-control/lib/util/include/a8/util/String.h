#pragma once
#include "a8/util/Buffer.h"//TODO remove Buffer dependence.
#include <stdio.h>
namespace a8 {
namespace util {

class String {

private:
    int length;
    char *text;
    void replace(const char *buf, int len, bool deleteText);

public:
    // static member funcs
    static int getLength(const char *str);
    static Buffer<String> &strings(int argc, char **argv, Buffer<String> &&buf);
    static String string(const char *str);

    // Template method must be implemented in header file .
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

    template <typename T>
    Buffer<T> split(const char separator, T (*convert)(String &)) {

        Buffer<T> buffer;
        String str;

        for (int i = 0; i < length; i++) {
            if (text[i] == separator) {

                buffer.append(convert(str));
                str = "";
                continue;
            }
            str.append(text[i]);
        }
        buffer.append(convert(str));

        return buffer;
    }

    String();
    // String(const char * str), this constructor is not defined.
    // Because for some reason if this method is added then the literal C++ string could not be recognized as String automatically by compiler.
    // So we remove this constructor for now.
    //
    String(const char (&buf)[]);
    String(const char *str, int len);
    String(const String &str); // Copy constructor
    // Move constructor does not work for some unknow reason。
    // Looks like the deconstructor is called unexpectedly when execution split().
    // The split method take a function pointer as parameter to convert each string split out;
    // So we remove this constructor for now.
    // String(const String &&str);
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

    int lastIndexOf(char ch) {
        for (int i = length - 1; i >= 0; i--) {
            if (this->text[i] == ch) {
                return i;
            }
        }
        return -1;
    }
    int indexOf(char ch) const {
        for (int i = 0; i < length; i++) {
            if (this->text[i] == ch) {
                return i;
            }
        }
        return -1;
    }
    String subStr(int from) const {
        return subStr(from, length - from);
    }
    String subStr(int from, int len) const {
        String ret;
        char *buf = this->text + from;
        ret.append(buf, len);
        return ret;
    }

    String trim() {
        int from = 0;
        for (int i = 0; i < this->length; i++) {
            char ch = this->text[i];
            if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
                from = i;
                continue;
            }
            break;
        }

        if (from > 0) {
            return subStr(from, length - from);
        } else {
            return *this;
        }
    }
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

    // Friend operators
    friend String operator+(const String &str1, const String &str);
    friend bool operator==(const String &str1, const String &str2);
    friend bool operator==(const String &str1, const int len);
    friend bool operator!=(const String &str1, const int len);
    friend bool operator!=(const String &str1, const String &str2);
};

bool operator==(const String &str1, const String &str2);

} // namespace util

} // namespace a8
