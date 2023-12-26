#pragma once
#include "a8/util/Debug.h"
#include "a8/util/Output.h"
#include "a8/util/Writer.h"

namespace a8::util {
class OutputWriter : public Writer {

    Output *out;

    int write(const char *buf, int bufLen, Result &res) override {
        A8_TRACE(">>OutputWriter::write");
        out->print(buf, bufLen);
        A8_TRACE("<<OutputWriter::write");
        return bufLen;
    }

public:
    OutputWriter(Output *output) {
        this->out = output;
    }
};
} // namespace a8::util