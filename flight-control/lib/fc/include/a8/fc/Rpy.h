#pragma once
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;

class Rpy {
public:
    virtual bool update() = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
};
} // namespace a8::fc