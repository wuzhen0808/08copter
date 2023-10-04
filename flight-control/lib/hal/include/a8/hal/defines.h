#pragma once
#include "a8/util/sys/System.h"

namespace a8::hal {

// This S pointer global variable should be initialized in a concrete environment(aka. System).
// Typically assigned from a main source file.
// May be we should not us hal as the package, instead of it the concept of port is more accurate?
extern a8::util::sys::System *S;

} // namespace a8::hal
