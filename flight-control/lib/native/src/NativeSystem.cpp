#include "a8/native/NativeSystem.h"
#include <iostream>

namespace a8 {
namespace native {
using namespace std;

NativeSystem::NativeSystem(){

}
long NativeSystem::millis() {
    return 0;
}

void NativeSystem::memcpy(void * to, const void * from, int size){
    //memcpy(to, from, size);
}

} // namespace hal
} // namespace a8
