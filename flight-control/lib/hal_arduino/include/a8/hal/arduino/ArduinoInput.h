#pragma once
#include "a8/util/Reader.h"
namespace a8::hal::arduino_ {
using namespace a8::util;

class ArduinoInput : public Reader {

public:
    ArduinoInput() ;
    ~ArduinoInput();
    int read(char *buf, int bufLen, Result &res) override;
};

} // namespace a8::hal::arduino_