#include "a8/native/NativeServosControl.h"
#include "a8/native/NativeServo.h"

namespace a8::native {

NativeServosControl::NativeServosControl(int totalServos, int *servoAttachPins) : ServosControl(totalServos, servoAttachPins) {
}
NativeServosControl::~NativeServosControl() {
}

Servo *NativeServosControl::newServo(int servoId) {
    return new NativeServo();
}

} // namespace a8::native