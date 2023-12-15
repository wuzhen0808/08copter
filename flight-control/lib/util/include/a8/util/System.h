#pragma once
#include "a8/util/Output.h"
#include "a8/util/Reader.h"
#include "a8/util/Properties.h"
namespace a8::util {
enum PinMode{
    IN,
    OUT
};

class System {
protected:
    long long started;

public:
    Output *out;
    Reader *input;

    System() {
    }

    virtual long long getSysTime() = 0;

    virtual long long getSteadyTime() = 0;

    virtual void formatTime(long long ms, String *str) = 0;

    virtual int openFile(String fpath, Reader *&fReaderCb) = 0;

    virtual String getEnv(String name) = 0;

    virtual void setPinMode(int pin, PinMode mode);

    virtual int analogRead(int pin);

    virtual int analogReadMilliVolts(int pin);

    virtual bool digitalRead(int pin);

    virtual void digitalWrite(int pin, bool high);

    virtual void delay(long ms);

    Reader *getInput() {
        return this->input;
    }

    virtual Properties &getProperties(Properties &pts) {
        return pts;
    }

    System &operator<<(const char *str) {
        out->print(String() << str);
        return *this;
    }
    System &operator<<(String &str) {
        out->print(str);
        return *this;
    }

    friend String &operator<<(String &str, System *sys) {
        Properties pts;
        sys->getProperties(pts);
        Buffer<String> lines = pts.getAllLines();
        for (int i = 0; i < lines.len(); i++) {
            str << lines.get(i) << "\n";
        }
        return str;
    }
};

} // namespace a8::util
