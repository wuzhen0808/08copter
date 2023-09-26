#include "a8/util/String.h"
#include "a8/util/Buffer.h"
#include <stdio.h>
// #define A8_DEBUG_STRING
#ifdef A8_DEBUG_STRING
#include <iostream>
using std::cout;
using std::endl;
#define LOG(msg) cout << msg << endl;
#define LOG2(msg, msg2) cout << msg << msg2 << endl;
#else
#define LOG(msg)
#define LOG2(msg, msg2)
#endif
#define INC (10)

namespace a8::util {

String::String() {
    LOG(">>String::String()");
    this->text = new char[1]{'\0'};
    this->length = 0;
    LOG("<<String::String()");
}
String::String(const char (&buf)[]) {
    replace(buf, -1, false);
}

String::String(const String &str) {
    LOG(">>String::String(const String &str)");
    replace(str.text, str.length, false);
    LOG("<<String::String(const String &str)");
}
String::String(const Buffer<char> &buf) {
    replace(buf.getAll(), buf.getLength(), false);
}
String::String(const char *buf, int len) {
    LOG(">>String::String(const char *buf, int len)");
    replace(buf, len, false);
    LOG("<<String::String(const char *buf, int len)");
}

void String::replace(const char *buf, int len, bool deleteText) {
    if (deleteText) {
        delete[] text;
    }
    if (len == -1) {
        len = String::getLength(buf);
    }
    char *buf2 = new char[len + 1]{0};
    String::copy(buf, 0, len + 1, buf2, 0);
    this->text = buf2;
    this->length = len;
}
char *String::getText() const {
    return this->text;
}

int String::getLength(const char *str) {
    for (int i = 0;; i++) {
        if (str[i] == '\0') {
            return i;
        }
    }
}

void String::copy(const char *sourceStr, int from1, int len, char *toBuff, int from2) {
    for (int i = 0; i < len; i++) {
        toBuff[from2 + i] = sourceStr[from1 + i];
    }
}

String::~String() {
    delete[] this->text;
    this->text = 0;
    this->length = 0;
}

int String::getLength() const {
    return this->length;
}

char String::getChar(int idx) const {
    return this->text[idx];
}

void String::append(const char *str) {
    append(str, getLength(str));
}

void String::append(const char ch) {
    append(&ch, 1);
}

void String::append(const float fValue) {
    append(String::format("%e", fValue));
}

void String::append(const double fValue) {
    append(String::format("%e", fValue));
}

void String::append(const int fValue) {
    append(String::format("%i", fValue));
}

void String::append(const long fValue) {
    append(String::format("%i", fValue));
}

void String::append(const unsigned fValue) {
    append(String::format("%i", fValue));
}

void String::append(const String *str) {
    append(str->getText(), str->getLength());
}

void String::append(const char *str, int len) {

    char *buf = new char[length + len + 1]{0};
    String::copy(text, 0, length, buf, 0);
    String::copy(str, 0, len, buf, length);
    delete[] text;
    text = buf;
    length = length + len;
}
void String::append(const String &str) {
    append(str.text, str.length);
}

// operators
/**
String &String::operator=(const char (&buf)[]) {
    replace(buf, -1, true);
    return *this;
}
*/

String &String::operator=(const char *buf) {
    replace(buf, -1, true);
    return *this;
}

String &String::operator=(const String &str) {
    replace(str.text, str.length, true);
    return *this;
}

String &String::operator=(const char &str) {
    replace(&str, 1, true);
    return *this;
}

String &String::operator=(const int iValue) {
    String str = String::format("%i", iValue);
    replace(str.text, str.length, true);
    return *this;
}

String &String::operator=(const float fValue) {
    String str;
    str.append(fValue);
    this->operator=(str);
    return *this;
}

String &String::operator=(const Buffer<char> &buf) {
    replace(buf.getAll(), buf.getLength(), true);
    return *this;
}

String &String::operator<<(const float fValue) {
    append(fValue);
    return *this;
}
String &String::operator<<(const double fValue) {
    append(fValue);
    return *this;
}
String &String::operator<<(const int fValue) {
    append(fValue);
    return *this;
}
String &String::operator<<(const long fValue) {
    append(fValue);
    return *this;
}
String &String::operator<<(const unsigned fValue) {
    append(fValue);
    return *this;
}

String &String::operator<<(const char *buf) {
    append(buf);
    return *this;
}

String &String::operator<<(const char ch) {
    append(ch);
    return *this;
}

String &String::operator<<(const String &str) {
    append(str);
    return *this;
}
// other

bool String::endWith(const char *str) {
    int len = getLength(str);
    if (this->length < len) {
        return false;
    }

    for (int i = 0; i < len; i++) {
        int idx = this->length - len + i;
        if (text[idx] != str[i]) {
            return false;
        }
    }
    return true;
}

Buffer<String> String::split(const char separator) {

    Buffer<String> buffer;
    String str;

    for (int i = 0; i < length; i++) {
        if (text[i] == separator) {
            buffer.append(str);
            str = "";
        }
        str.append(text[i]);
    }
    buffer.append(str);

    return buffer;
}

// friend static member
String operator+(String const &str1, String const &str2) {
    int len1 = str1.getLength();
    int len2 = str2.getLength();
    char *buf = new char[len1 + len2 + 1]{0};
    char *text1 = str1.getText();
    char *text2 = str2.getText();
    String::copy(text1, 0, len1, buf, 0);
    String::copy(text2, 0, len2, buf, len1);
    String ret(buf, len1 + len2);
    return ret;
}

bool operator==(String const &str1, String const &str2) {
    int len1 = str1.getLength();
    int len2 = str2.getLength();
    if (len1 != len2) {
        return false;
    }
    for (int i = 0; i < len1; i++) {
        if (str1.getChar(i) != str2.getChar(i)) {
            return false;
        }
    }
    return true;
}

} // namespace a8::util