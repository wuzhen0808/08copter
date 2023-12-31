#pragma once
#include "a8/fc/Factory.h"
#include "a8/fc/esp/EspImu.h"
#include "a8/fc/esp/EspPropeller.h"
#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/util.h"
namespace a8::fc::esp {
using namespace a8::hal::esp;
using namespace a8::hal::arduino_;
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
    Filter *newLowPassFilter(Rate cutOffRate, Rate samplingRate, int order) {
        return new ArduinoLowPassFilter(cutOffRate,samplingRate, order);
    }
};
} // namespace a8::fc::esp