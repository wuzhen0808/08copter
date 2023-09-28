#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"

namespace a8::util {

/**
 */
class StringReader : public Reader {
protected:
    Buffer<char> buffer;
    int from = 0;

public:
    StringReader(const char *str) {
        buffer.append(str, Util::strLength(str));
    }

    ~StringReader() {
    }

    virtual int read(char *buf, int bufLen) override {
        int len = buffer.read(from, buf, bufLen);
        if (len > 0) {
            from += len;
        }
        return len;
    };
};

} // namespace a8::util