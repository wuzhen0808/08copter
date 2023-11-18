#pragma once
#include "a8/util/Writer.h"
#include "a8/util/Logger.h"
#include "a8/util/System.h"


namespace a8::util {
using namespace a8::util;


class WriterLogger : public Logger {
protected:
    Writer *writer;
    System *sys;

public:
    WriterLogger(String name, Writer *writer, System *sys) : Logger(name) {
        this->writer = writer;
        this->sys = sys;
    }
    ~WriterLogger() {
    }

    void writeLevel(Logger::Level level, String &msg) {
    }

    void writeTime() {
    }

    void log(Logger::Level level, const String &msg) override {
        long long stime = sys->getSysTime();
        String formatTime;
        sys->formatTime(stime, &formatTime);
        String msg2;
        msg2 << "[" << formatTime << "] [" << level << "] " << msg;        
        writer->write(msg2.text(), msg2.length());
        writer->write("\n", 1);
    };
};

} // namespace a8::util
