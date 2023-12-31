
#pragma once

#include "a8/hal.h"
#include "a8/hal/native/NativeOutput.h"
#include "a8/util.h"

namespace a8::hal::native {
using namespace std;
using namespace a8::util;
using namespace a8::hal;

class NativeSystem : public System {
private:
    String timeFormat;

public:
    NativeSystem();
    long long getSysTime() override;
    long long getSteadyTime() override;
    void formatTime(long long ms, String *str) override;
    int openFile(String fpath, Reader *&fReaderCb) override;
    String getEnv(String name) override;
};

} // namespace a8::hal::native
