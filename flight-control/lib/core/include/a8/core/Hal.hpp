#ifndef A8_HAL__
#pragma once
#include "a8/core/core.hpp"

namespace a8 {
namespace core {
namespace hal {
int map(int value, int from1, int to1, int from2, int to2);
ulong millis();

void println(char *msg);
void println(char *msg);

} // namespace hal

} // namespace core

} // namespace a8

#endif