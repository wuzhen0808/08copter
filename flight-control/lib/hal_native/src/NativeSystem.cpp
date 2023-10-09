
#include "a8/hal/native.h"
#include "a8/hal/native/winsock2windows.h"
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
using namespace a8::util;
using namespace a8::hal;
namespace a8::hal::native {

NativeSystem::NativeSystem() {
    out = new NativeOutput();
}
void NativeSystem::sleep(long ms) {
    Sleep(ms);
}
long NativeSystem::millis() {
    timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return time.tv_sec * 1000 + time.tv_nsec / 1.0e6;
}

} // namespace a8::hal::native
