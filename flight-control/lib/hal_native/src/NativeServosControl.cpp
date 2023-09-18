#include "a8/native/NativeServosControl.h"
#include "a8/native/NativeServo.h"
#include "a8/jsbsim/JsbsimServo.h"
namespace a8::native {

NativeServosControl::NativeServosControl(int totalServos, int *servoAttachPins) : ServosControl(totalServos, servoAttachPins) {
}
NativeServosControl::~NativeServosControl() {
}

Servo *NativeServosControl::newServo(int servoId) {
    //return new NativeServo();
    return new a8::jsbsim::JsbsimServo();
}

} // namespace a8::native