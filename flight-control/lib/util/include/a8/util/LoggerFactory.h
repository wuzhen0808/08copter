#pragma once

#include "a8/util/Logger.h"
#include "a8/util/String.h"

namespace a8::util {
using namespace a8::util;
class WrapperLogger : public Logger {
    Logger *root;

public:
    WrapperLogger(String name, Logger *root) : Logger(name) {
        this->root = root;
    }

    void log(Level level, const String &msg) {
        String msg2 = String() << " [" << this->name << "] " << msg;
        root->log(level, msg2);
    }
};

class LoggerFactory {
protected:
    virtual Logger *getRootLogger() = 0;

public:
    LoggerFactory() {}
    ~LoggerFactory() {}
    Logger *getLogger() {
        return getLogger(String() << "default");
    }

    Logger *getLogger(const String &name) {
        Logger *root = this->getRootLogger();
        return new WrapperLogger(name, root);
    }
};

} // namespace a8::util
