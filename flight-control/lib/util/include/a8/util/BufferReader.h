#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

namespace a8::util {

/**
 */
class BufferReader : public Reader, public Buffer<char> {
protected:
    int from = 0;

public:
    BufferReader(Buffer<char> buf) : Buffer<char>(buf) {
    }
    BufferReader() {
    }

    int read(char *buf, int bufLen) override {
        int len = this->len() - from;
        if (len > bufLen) {
            len = bufLen;
        }
        Lang::copy(this->buffer(), from, len, buf);
        from += len;
        return len;
    };
    void reset() {
        from = 0;
    }
};

} // namespace a8::util