#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

namespace a8::util {

/**
 */
class WriterReaderBuffer : public Writer, public Reader, public Buffer<char> {
    int from = 0;

public:
    int write(const char *buf, int bufLen, Result &res) override {
        this->append(buf, bufLen);
        return bufLen;
    };

    int read(char *buf, int bufLen, Result &res) override {
        int len = this->len() - from;
        if (len > bufLen) {
            len = bufLen;
        }
        Array::copy<char>(this->buffer(), from, len, buf);
        from += len;
        return len;
    };
    void reset() {
        from = 0;
    }
};

} // namespace a8::util