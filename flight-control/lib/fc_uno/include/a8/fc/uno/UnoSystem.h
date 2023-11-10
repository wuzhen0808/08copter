#pragma once
#include "a8/util.h"

namespace a8::fc::arduino_ {
using namespace a8::util;

class UnoSystem : public System {
public:
    UnoSystem();
    virtual long millis();
};
} // namespace a8::fc::arduino_