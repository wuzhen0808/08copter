#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;

class Output {
public:
    virtual void print(const String msg) = 0;

    void print(const char ch) {
        print(String() << ch);
    }

    template <typename T>
    void print(const T ele) {
        print(String() << ele);
    }

    void print(const char *buf, int len) {
        print(String(buf, len));
    }
    void println() {
        this->print("\n");
    }

    void println(const String msg) {
        this->print(msg);
        this->print("\n");
    }

    Output &operator<<(String &str) {
        this->print(str);
        return *this;
    }
};
} // namespace a8::util