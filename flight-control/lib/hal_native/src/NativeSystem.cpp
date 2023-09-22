#include "a8/hal/native/NativeSystem.h"
#include "a8/hal/native/NativeOutput.h"

namespace a8::hal::native {
using namespace std;

NativeSystem::NativeSystem() {
    out = new NativeOutput();
}
long NativeSystem::millis() {
    return 0;
}

} // namespace a8::hal::native
