#pragma once
#include "a8/hal.h"
#include "a8/util/Logger.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

using namespace a8::util;
using namespace a8::hal;

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

    void writeLevel(Logger::Level level, String &msg) {
    }

    void writeTime() {
    }

    virtual void log(Logger::Level level, const String &msg) override {
        long long stime = S->getSysTime();
        String formatTime;
        S->formatTime(stime, &formatTime);
        String msg2;
        msg2 << "[" << formatTime << "] [" << level << "] " << msg;
        S->out->println(msg2);
        writer->write(msg2.text(), msg2.length());
        writer->write("\n", 1);
    };
};

} // namespace a8::util
