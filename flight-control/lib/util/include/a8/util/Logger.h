#pragma once

#include "a8/util/Shared.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {

class Logger {

private:
    String name;

protected:
public:
    Logger(String name) {
        this->name = name;
    }

    ~Logger() {
    }

    virtual void log(int level, const String &msg){

    }
    
    void error(const String &msg) {
        log(1, msg);
    }

    void warn(const String &msg) {
        log(2, msg);
    }

    void info(const String &msg) {
        log(3, msg);
    }

    void debug(const String &msg) {
        log(4, msg);
    }

    void trace(String &msg) {
        log(5, msg);
    }
};

} // namespace a8::util
