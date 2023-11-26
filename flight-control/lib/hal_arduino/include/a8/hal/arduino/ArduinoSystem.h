#pragma once
#include "a8/util.h"

namespace a8::hal::arduino_ {
using namespace a8::util;
class ArduinoSystem : public System {
public:
    ArduinoSystem();
    long long getSysTime() override;
    long long getSteadyTime() override;
    void formatTime(long long ms, String *str) override;
    int openFile(String fpath, Reader *&fReaderCb) override;
    String getEnv(String name) override;
    void setPinMode(int pin, PinMode pinMode) override;
    int analogRead(int pin) override;
    bool digitalRead(int pin) override;
    void delay(long ms) override;
};
} // namespace a8::hal::arduino_