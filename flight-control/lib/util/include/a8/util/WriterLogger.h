#pragma once
#include "a8/hal.h"
#include "a8/util/Logger.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

using namespace a8::util;

namespace a8::util {

class WriterLogger : public Logger {
protected:
    Writer *writer;

public:
    WriterLogger(String name, Writer *writer) : Logger(name) {
        this->writer = writer;
    }
    ~WriterLogger() {
    }

    virtual void log(int level, const String &msg) override {
        a8::hal::S->out->println(msg);
        writer->write(msg.text(), msg.length());
        writer->write("\n", 1);
    };
};

} // namespace a8::util
