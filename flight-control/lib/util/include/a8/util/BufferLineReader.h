#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/LineReader.h"
#include "a8/util/Math.h"
#include "a8/util/Reader.h"
#include "a8/util/Result.h"
#include "a8/util/String.h"
#include "a8/util/StringReader.h"
#include "a8/util/StringUtil.h"
#include "a8/util/BufferReader.h"

#define LR_MAX_LEN_ (1024) // 1k

namespace a8::util {

/**
 */
class BufferLineReader {

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
        input->append(msg.text(), msg.len());
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