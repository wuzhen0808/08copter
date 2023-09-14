#include "a8/util/String.h"
#include <stdio.h>

#ifdef A8_DEBUG_STRING
#include <iostream>
using std::cout;
using std::endl;
#define LOG(msg) cout << msg << endl;
#else
#define LOG(msg)
#endif
#define INC (10)

namespace a8::util {

String::String() {
    LOG(">>String::String()");
    this->text = new char[1]{'\0'};
    this->length = 0;
    LOG("<<String::String()");
}

String::String(const char buf[]) {
    LOG(">>String::String(const char buf)");
    int len = String::getLength(buf);
    char *buf2 = new char[len + 1]{0};
    String::copy(buf, 0, len + 1, buf2, 0);
    this->text = buf2;
    this->length = len;
    LOG("<<String::String(const char buf)");
}

String::String(const String &str) {
    LOG(">>String::String(const String &str)");
    int len = str.length;
    char *buff = new char[len + 1]{0};
    String::copy(str.text, 0, len + 1, buff, 0);
    this->text = buff;
    this->length = len;
    LOG("<<String::String(const String &str)");
}

String::String(const char *text, int len) {
    LOG(">>String::String(const char *text, int len)");
    char *buff = new char[len + 1]{0};
    copy(text, 0, len + 1, buff, 0);
    this->text = buff;
    this->length = len;
    LOG("<<String::String(const char *text, int len)");
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

void String::copy(const char *str, int from1, int len, char *buff, int from2) {
    for (int i = 0; i < len; i++) {
        buff[from2 + i] = str[from1 + i];
    }
}
String String::format(const char formatStr[], int arg) {

    char buf[100] = {0};
    int len = snprintf(buf, 100, formatStr, arg);
    if (len < 0) {
        String ret(formatStr);
        LOG(ret.getText());
        return ret; // Failed to format? need to throw a exception?
    } else {
        String ret(buf);
        LOG(ret.getText());
        return ret; // success
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

String &String::operator=(const char str[]) {
    delete[] text;
    int len = getLength(str);
    char *buff = new char[len + 1]{0};
    copy(text, 0, len + 1, buff, 0);
    this->text = buff;
    this->length = len;
    return (*this);
}

} // namespace a8::util