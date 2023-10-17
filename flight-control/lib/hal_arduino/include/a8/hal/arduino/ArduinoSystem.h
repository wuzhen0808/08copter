#pragma once
#include "a8/util.h"

namespace a8::hal::arduino {
class ArduinoSystem : public System {
public:    
    ArduinoSystem();
    void sleep(long ms) override;
    long long getSysTime() override;
    long long getSteadyTime() override;
    void formatTime(long long ms, String *str) override;
    int openFile(String fpath, Reader *&fReaderCb) override;
    String getEnv(String name) override;
};
} // namespace a8::hal::arduino