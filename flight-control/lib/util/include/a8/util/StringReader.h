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
    StringReader(String str) {
        buffer.append(str.getText(), str.getLength());
    }

    ~StringReader() {
    }

    virtual int read(char *buf, int bufLen) override {
        return -1;
    };
};

} // namespace a8::util