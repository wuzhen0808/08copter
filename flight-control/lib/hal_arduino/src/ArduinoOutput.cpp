

#include "a8/hal/arduino/ArduinoOutput.h"
namespace a8::hal::arduino_ {

// functions
Output *createOutput(int serial) {
    if (serial == 1) {
        return new ArduinoOutput1();
    } else {
        return new ArduinoOutput();
    }
}

Output *createOutput() {
    return createOutput(0);
}

} // namespace a8::hal::arduino_