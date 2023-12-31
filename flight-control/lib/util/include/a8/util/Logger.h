#pragma once

#include "a8/util/Shared.h"
#include "a8/util/String.h"

namespace a8::util {
using namespace a8::util;

class Logger {
private:
protected:
    String name;

public:
    enum Level {
        Error = 1,
        Warn,
        Info,
        Debug,
        Trace
    };
    Logger(String name) {
        this->name = name;
    }

    ~Logger() {
    }
    String getName() {
        return this->name;
    }
    virtual void log(Level level, const String &msg) = 0;

    void error(const String &msg) {
        log(Error, msg);
    }

    void warn(const String &msg) {
        log(Warn, msg);
    }

    void info(const String &msg) {
        log(Info, msg);
    }

    void debug(const String &msg) {
        log(Debug, msg);
    }

    void trace(const String &msg) {
        log(Trace, msg);
    }

    Logger *operator<<(String &msg) {
        log(Info, msg);
        return this;
    }
    friend Logger *operator<<(Logger *logger, String &msg) {
        logger->log(Info, msg);
        return logger;
    }

    friend String &operator<<(String &str, Level level) {
        switch (level) {
        case Trace:
            str << "trace";
            break;
        case Debug:
            str << "debug";
            break;
        case Info:
            str << "info";
            break;
        case Warn:
            str << "warn";
            break;
        case Error:
            str << "error";
            break;
        default:
            str << (int)level;
            break;
        }
        return str;
    }
};

} // namespace a8::util
