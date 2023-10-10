#include "a8/hal/native/NativeOutput.h"
#include <iostream>
using namespace std;

namespace a8::hal::native {
using a8::util::Output;
using a8::util::String;

void NativeOutput::print(const String msg) {
    cout << msg.text();
}

} // namespace a8::hal::native
