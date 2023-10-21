#include "a8/hal/arduino/HardwareSerialWrapper.h"
#include "a8/util.h"
#include <api/HardwareSerial.h>

namespace a8::hal::arduino_ {
using namespace arduino;
using namespace a8::util;

HardwareSerialWrapper::HardwareSerialWrapper(HardwareSerial *serial) {
    this->serial = serial;
}

void HardwareSerialWrapper::begin(unsigned long baud) {
    this->serial->begin(baud);
}
void HardwareSerialWrapper::print(const String &msg){
    this->serial->print(msg.text());
}
void HardwareSerialWrapper::println(const String &msg){
    this->serial->println(msg.text());
}

} // namespace a8::hal::arduino_
