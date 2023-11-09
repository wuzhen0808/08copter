#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/LineReader.h"
#include "a8/util/Math.h"
#include "a8/util/Reader.h"
#include "a8/util/Result.h"
#include "a8/util/String.h"
#include "a8/util/StringReader.h"
#include "a8/util/StringUtil.h"

#define LR_MAX_LEN_ (1024) // 1k

namespace a8::util {

/**
 */
class BufferReader : public Reader {

    Buffer<char> buf_;
    int from = 0;

public:
    BufferReader() {
    }
    BufferReader(char *buf, int len) {
        this->append(buf, len);
    }
    int read(char *buf, int bufLen) override {
        int ret = 0;
        if (from < this->buf_.len()) {
            ret = this->buf_.read(from, buf, bufLen);
        }
        if (ret <= 0) {
            return ret;
        }
        from += ret;
        if (from > LR_MAX_LEN_) {
            this->buf_ = this->buf_.subBuffer(from);
            this->from = 0;
        }
        return ret;
    }

    bool hasMore() {
        return this->from < buf_.len() - 1;
    }

    void append(char *buf, int len) {
        buf_.append(buf, len);
    }
};

} // namespace a8::util