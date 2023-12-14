#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"

namespace a8::util {
/**
 */
class PushBackReader : public Reader {
protected:
    Reader *reader;
    Buffer<char> buffer;

public:
    PushBackReader(Reader *reader) {
        this->reader = reader;
    }

    void pushBack(char ch) {
        buffer.append(ch);
    }

    int read(char *buf, int bufLen, Result &res) {
        int len = 0;
        int localLen = buffer.len();
        if (localLen == 0) {
            return this->reader->read(buf, bufLen, res);
        }
        len = Math::min(bufLen, localLen);
        Lang::copy<char>(buffer.buffer(), 0, len, buf, 0);
        this->buffer = this->buffer.subBuffer(len);

        return len;
    }
};

} // namespace a8::util