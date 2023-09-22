#include "a8/hal/native/NativeServo.h"

namespace a8::hal::native {
using a8::util::String;

void NativeServo::writeMicroseconds(int pw) {

    String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", this->id, 1.0f);
    int sent = socket_->send(&sb);

}

void NativeServo::setup() {
    
}

} // namespace a8::hal::native