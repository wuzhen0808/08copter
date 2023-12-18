#pragma once
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;

class Rpy {
public:
    virtual void setup() = 0;
    virtual void tick() = 0;
    virtual bool update() = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
    virtual int checkIfReady(Result &res) = 0;
    virtual int checkIfStable(Result &res) = 0;
    virtual int checkIfBalance(Result &res) = 0;
    virtual bool isStable() = 0;
    virtual bool isBalance() = 0;
};
} // namespace a8::fc