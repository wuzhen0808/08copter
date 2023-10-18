#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Writer.h"

namespace a8::util {
class MultiWriter : public Writer {

    Buffer<Writer *> writers;
    int write(const char *buf, int bufLen) override {
        for (int i = 0; i < writers.len(); i++) {
            int ret = writers[i]->write(buf, bufLen);
            if (ret < 0) {
                return ret;
            }
        }
        return bufLen;
    }

public:
    void add(Writer *writer) {
        this->writers.append(writer);
    }
};
} // namespace a8::util