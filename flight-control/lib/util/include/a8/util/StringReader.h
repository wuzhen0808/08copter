#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Result.h"

namespace a8::util {

/**
 */
class StringReader : public Reader {
protected:
    Buffer<char> buffer;
    int from = 0;

public:
    StringReader() {
    }
    StringReader(const char *str) {
        buffer.append(str, Array::strLength(str));
    }
    StringReader(const String str) {
        buffer.append(str.text(), str.len());
    }

    ~StringReader() {
    }
    void append(String msg) {
        buffer.append(msg.text(), msg.length());
    }

    int read(char *buf, int bufLen, Result &res) override {
        if (from == buffer.len()) {
            return 0; // end of reading.
        }
        int ret = buffer.read(from, buf, bufLen);
        if (ret < 0) {
            return ret;
        }
        int len = ret;
        from += len;
        return len;
    };
};

} // namespace a8::util