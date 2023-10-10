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
class BufferLineReader {

    class BufferReader : public Reader {

        Buffer<char> buf_;
        int from = 0;

    public:
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

        void append(String msg) {
            buf_.append(msg.text(), 0, msg.len());
        }
    };

    BufferReader *input;

    LineReader *lReader;

    String buffer;

public:
    BufferLineReader() {
        this->input = new BufferReader();
        this->lReader = new LineReader(this->input);
    }
    ~BufferLineReader() {
        delete this->lReader;
        delete this->input;
    }

    int append(String msg) {
        input->append(msg);
        return msg.len();
    }

    int readLine(String &ret) {
        Result rst;
        return readLine(ret, true, rst);
    }

    int readLine(String &ret, Result &rst) {
        return readLine(ret, true, rst);
    }

    /**
     * Do not readLine unless append first.
     * Reading an empty buffer will lead to final result.
     */
    int readLine(String &line, bool appendSeparator, Result &rst) {

        String builder;
        int ret = lReader->readLine(builder, appendSeparator, rst);
        if (ret == 0) {
            lReader->setReader(this->input);
            ret = lReader->readLine(builder, appendSeparator, rst);
        }

        if (ret <= 0) {
            return ret;
        }

        if (builder.length() < ret || builder.endWith('\n')) {

            buffer << builder;
            line = buffer;
            ret = buffer.len();
            buffer.clear();
        } else {
            buffer << builder;
            ret = -ret;
        }

        return ret;
    }

    int bufferLen() {
        return this->buffer.len();
    }
};

} // namespace a8::util