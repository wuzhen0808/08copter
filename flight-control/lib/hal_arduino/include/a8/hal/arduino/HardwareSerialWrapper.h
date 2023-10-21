#pragma once
#include "a8/util.h"
namespace arduino {
class HardwareSerial;
class HardwareI2C;
} // namespace arduino

template <typename T>
class MPU9250_;

namespace a8::hal::arduino_ {
using namespace arduino;
using namespace a8::util;

class HardwareSerialWrapper {

public:
    HardwareSerial *serial;
    HardwareSerialWrapper(HardwareSerial *serial);
    void begin(unsigned long baud);
    void print(const String &msg);
    void println(const String &msg);
};

} // namespace a8::hal::arduino_