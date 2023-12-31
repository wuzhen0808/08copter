#pragma once
#include "a8/util/Debug.h"
#include "a8/util/Buffer.h"
#include "a8/util/Result.h"
#include "a8/util/Writer.h"

namespace a8::util {
class MultiWriter : public Writer {

    Buffer<Writer *> writers;

    int write(const char *buf, int bufLen, Result &res) override {
        A8_TRACE(">>MultiWriter::write");
        for (int i = 0; i < writers.len(); i++) {
            int ret = writers[i]->write(buf, bufLen);
            if (ret < 0) {
                return ret;
            }
        }
        A8_TRACE("<<MultiWriter::write");
        return bufLen;
    }

public:
    void add(Writer *writer) {
        this->writers.append(writer);
    }
};
} // namespace a8::util