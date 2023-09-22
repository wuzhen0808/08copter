#include "a8/hal/native/NativeServo.h"

namespace a8::hal::native {
using a8::util::String;

void NativeServo::setThrottleNorm(float throttle) {
    this->jio->setThrottleNorm(this->id, throttle);
}

void NativeServo::setup() {
}

} // namespace a8::hal::native