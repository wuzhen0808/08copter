#pragma once
#include "a8/fc/Factory.h"
#include "a8/fc/esp/EspPropeller.h"
#include "a8/fc/esp/EspImu.h"
#include "a8/hal/esp.h"

namespace a8::fc::esp {
using namespace a8::hal::esp;
class EspFactory : public Factory {

public:
    EspFactory() {
    }

    Imu *newImu() override {
        return new EspImu(logFac);
    }

    void setupWire() override {
        EspUtil::setupWire();
    }
    Propeller *newPropeller(String name, int idx) override {
        return new EspPropeller(name, idx);
    }
};
} // namespace a8::fc::esp