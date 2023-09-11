#include "a8/util/string.hpp"
#include "stdio.h"

#define INC (10)
namespace a8 {
namespace util {

string::string() {

    this->init(new char[1]{'\0'}, UNKNOWN);
}

string::string(char *text) {
    this->init(text, UNKNOWN);
}

string::string(float fValue) {
    char *buf = new char[100];
    int len = snprintf(buf, 100, "%f", fValue);
    if (len < 0) {
        buf[99] = '\0';
        len = 99;
    }

    this->init(buf, len);
}

string::string(int iValue) {
    char *buf = new char[100];
    int len = snprintf(buf, 100, "%i", iValue);
    if (len < 0) {
        buf[99] = '\0';
        len = 99;
    }

    this->init(buf, len);
}

string::string(string &string) {
    this->init(string.text, string.getLength());
}

void string::init(char *text, int len) {
    this->text = text;
    this->length = length;
}
char *string::getText() {
    return this->text;
}
int string::getLength() {
    if (this->length == UNKNOWN) {
        for (int i = 0;; i++) {
            if (this->text[i] == '\0') {
                this->length = i;
                break;
            }
        }
    }
    return this->length;
}

string::~string() {
    delete this->text;
    this->text = 0;
    this->length = 0;
}

} // namespace util
} // namespace a8