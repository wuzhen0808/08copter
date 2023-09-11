#include "a8/Hal.hpp"
#include <Arduino.h>

namespace a8 {
namespace hal {
int map(int value, int from1, int to1, int from2, int to2) {
    return 0; //;map(value, from1, to1, from2, to2);
}
unsigned long millis() {
    return 0;
}
void print(char *msg) {}
void println(char *msg) {}
} // namespace hal
} // namespace a8