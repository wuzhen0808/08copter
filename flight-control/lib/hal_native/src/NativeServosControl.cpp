#include "a8/hal/native/NativeServosControl.h"
#include "a8/hal/native/NativeServo.h"
#include "a8/util/SocketReader.h"

using namespace a8::util;

namespace a8::hal::native {

void setupTheSocketReaderThread(Scheduler *scheduler) {
    
}

Result NativeServosControl::setup(){    
    ServosControl::setup();
    return true;
}
Servo *NativeServosControl::setupServo(int servoId) {
    // return new NativeServo();
    return new NativeServo(jio, servoId);
}

} // namespace a8::hal::native