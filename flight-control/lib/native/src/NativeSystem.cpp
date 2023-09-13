#include "a8/native/NativeSystem.h"
#include "a8/native/NativeOutput.h"

namespace a8 {
namespace native {
using namespace std;

NativeSystem::NativeSystem(){
    out = new NativeOutput();
}
long NativeSystem::millis() {
    return 0;
}

} // namespace hal
} // namespace a8
