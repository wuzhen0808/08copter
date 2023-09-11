#include <Arduino.h>
#include "a8/core/Hal.hpp"

namespace a8 {
namespace core {
int Hal::map(int value, int from1, int to1, int from2, int to2) {
    return map(value, from1, to1, from2,to2);
}
ulong Hal::millis() {
}
void Hal::print(char *msg) {}
void Hal::println(char *msg) {}
} // namespace core
} // namespace a8