#pragma once
#include "a8/util/Writer.h"
namespace a8::util {
class EmptyWriter : public Writer {
    int write(const char *buf, int bufLen) override {
        // do nothing.
    }
};
} // namespace a8::util