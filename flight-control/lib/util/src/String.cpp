#include "a8/util/String.hpp"

#define INC (10)

namespace a8 ::util {

String::String() {
    this->string = new char[1]{'\0'};
}

// String::String(String &str) {
//   this->string = copy(str.string);
// this->length = str.length;
//}

String::String(const char *str) {
    this->string = copy(str);
}

char *String::getText() {
    return this->string;
}
int String::getLength() {
    if (this->length == UNKNOWN) {

        for (int i = 0;; i++) {

            if (this->string[i] == '\0') {

                this->length = i;
                break;
            }
        }
    }
    return this->length;
}

char *String::copy(const char *str) {

    int inc = 100;
    int len = inc;
    char *buf = new char[len]{0};

    for (int i = 0;; i++) {
        if (i >= len) {
            int len2 = len + inc;
            char *buf2 = new char[len2]{0};
            for (int j = 0; j < len; j++) {
                buf2[j] = buf[j];
            }
            delete[] buf;
            buf = buf2;
            len = len2;
        }
        buf[i] = str[i];
        if (buf[i] == '\0') {
            break;
        }
    }
    return buf;
}
String::~String() {
    delete[] this->string;
    this->string = 0;
    this->length = 0;
}

} // namespace a8::util