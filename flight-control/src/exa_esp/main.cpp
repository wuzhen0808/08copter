
#include "a8/fc/esp.h"
#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"

namespace example {

using namespace a8::fc::esp;
using namespace a8::hal::esp;
void setup() {
    a8::hal::esp::EspUtil::setupSerial();
    EspAssembler *exa = new EspAssembler();
    Serial.println("setupApp");
    Result res;
    int ret = EspUtil::start("fc", exa, res);
    if (ret < 0) {
        Serial.println(res.errorMessage.text());
    }
}
} // namespace example

void setup() {
    example::setup();
}
int skips = 0;
void loop() {    
    delay(1000);
}