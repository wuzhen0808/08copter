#include "a8/util/String.h"
#include "a8/util/Buffer.h"
#include "a8/util/Util.h"


namespace a8::util {

// static methods

String String::string(const char *str) {
    return String(str, Util::strLength(str));
}

// dynamic methods


int String::getLength() const {
    return this->length;
}

// operators
/**
String &String::operator=(const char (&buf)[]) {
    replace(buf, -1, true);
    return *this;
}
*/

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
    int len = Util::strLength(str);
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
    return split<String>(separator, [](String &s) { return s; });
}

// friend static member
String operator+(String const &str1, String const &str2) {
    String ret;
    ret.append(str1);
    ret.append(str2);
    return ret;
}

bool operator==(String const &str1, String const &str2) {
    return !operator!=(str1, str2);
}
bool operator==(String const &str1, const int len) {
    return str1.getLength() == len;
}
bool operator!=(String const &str1, const int len) {
    return str1.getLength() != len;
}

bool operator!=(String const &str1, String const &str2) {
    int len1 = str1.getLength();
    int len2 = str2.getLength();
    if (len1 != len2) {
        return true;
    }
    for (int i = 0; i < len1; i++) {
        if (str1.getChar(i) != str2.getChar(i)) {
            return true;
        }
    }
    return false;
}

} // namespace a8::util