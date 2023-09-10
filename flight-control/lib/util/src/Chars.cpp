#include "a8/util/Chars.hpp"
#include "stdio.h"

#define INC (10)
namespace a8 {
namespace util {
typedef void (*ConsumerFunc)(void *context, char *);

static int MAX_LEN = 100;

template <typename T>
char *tryFormat(char *format, T v1, void *context, ConsumerFunc func) {

    char buffer[MAX_LEN] ;
    int len = snprintf(buffer, MAX_LEN, format, v1);
    if (len < 0) {
        func(context, format);
    } else {
        func(context, buffer);
    }
}

int Chars::getLength(char *string) {

    for (int i = 0;; i++) {
        if (string[i] == '\0') {
            return i;
        }
    }
}
Chars::Chars(char *string) {
    this->buffer = new Buffer<char>();
    this->append(string);
}

Chars::~Chars() {
    delete this->buffer;
    this->buffer = 0;
}
int Chars::getLength() {
    return this->buffer->getLength() - 1;
} // namespace util

Chars *Chars::append(char *string) {
    int len = this->getLength(string);
    return append(string, len);
}

Chars *Chars::append(char *string, int len) {
    this->buffer->remove(this->buffer->getLength() - 1);
    this->buffer->appendAll(string, len);
    this->buffer->append('\0');
    return this;
}
void myAppendFunc(void *stringObj, char *formatted) {
    Chars *strObj = static_cast<Chars *>(stringObj);
    strObj->append(formatted);
}

Chars *Chars::append(float value) {
    tryFormat<float>("%f", value, this, myAppendFunc);
    return this;
}

Chars *Chars::append(int value) {
    tryFormat<int>("%i", value, this, myAppendFunc);
    return this;
}

Chars *Chars::append(Chars *string) {
    char *chars = string->buffer->getAll();
    append(chars);
    return this;
}

char *Chars::getChars() {
}
} // namespace util
} // namespace a8