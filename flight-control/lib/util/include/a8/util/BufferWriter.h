#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

namespace a8::util {

/**
 */
class BufferWriter : public Writer, public Buffer<char> {
protected:
public:
    virtual int write(const char *buf, int bufLen) override {
        this->append(buf, bufLen);
        return bufLen;
    };
};

} // namespace a8::util