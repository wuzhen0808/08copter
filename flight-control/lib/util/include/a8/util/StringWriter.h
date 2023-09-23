#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

namespace a8::util {

/**
 */
class StringWriter : public Writer {
protected:
    Buffer<char> buffer;
    int from = 0;

public:
    StringWriter() {
    }

    ~StringWriter() {
    }

    virtual void write(const char *buf, int bufLen) override {
        buffer.append(buf, bufLen);
    };
    
    String toString() {
        return buffer;
    }
};

} // namespace a8::util