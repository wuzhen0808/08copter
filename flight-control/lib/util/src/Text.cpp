#include "a8/util/Text.hpp"
#include "a8/Hal.hpp"
#include "stdio.h"

#define INC (10)
namespace a8 {
namespace util {
using namespace a8::hal;

Text::Text() {
    // println("Text::Text()");
    // this->init(new char[1]{'\0'}, UNKNOWN);
}

// Text::Text(Text &text) {
//     println("Text::Text(Text &text)");
//     this->init(text.text, text.getLength());
// }

Text::Text(char *text) {
    this->init(text, UNKNOWN);
}

Text::Text(float fValue) {
    char *buf = new char[100];
    int len = snprintf(buf, 100, "%f", fValue);
    if (len < 0) {
        buf[99] = '\0';
        len = 99;
    }

    this->init(buf, len);
}

Text::Text(int iValue) {
    char *buf = new char[100];
    int len = snprintf(buf, 100, "%i", iValue);
    if (len < 0) {
        buf[99] = '\0';
        len = 99;
    }

    this->init(buf, len);
}

void Text::init(char *text, int len) {
    println("inti");
    this->text = text;
    this->length = length;
}
char *Text::getText() {
    return this->text;
}
int Text::getLength() {
    if (this->length == UNKNOWN) {

        for (int i = 0;; i++) {
            println("getLength():");
            if (this->text[i] == '\0') {
                println(i);
                this->length = i;
                break;
            }
        }
    }
    return this->length;
}

Text::~Text() {
    println("Text::~Text()");
    if (this->text != 0) {
        delete this->text;
    }
    println("delete this->text");
    this->text = 0;
    this->length = 0;
}

} // namespace util
} // namespace a8