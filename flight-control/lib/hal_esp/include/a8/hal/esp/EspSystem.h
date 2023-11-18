#pragma once
#include "a8/hal/arduino.h"
#include <Arduino.h>
namespace a8::hal::esp {
using namespace a8::hal::arduino_;
class EspSystem : public ArduinoSystem {

    Properties &getProperties(Properties &pts) override{        
        pts.set("flashChipSize", (int)ESP.getFlashChipSize());        
        pts.set("psramSize", (int)ESP.getPsramSize()); 
        pts.set("heapSize", (int)ESP.getHeapSize());
        pts.set("sketchSize", (int)ESP.getSketchSize());
        pts.set("freePsram", (int)ESP.getFreePsram()); 
        pts.set("freeHeapSize", (int)ESP.getFreeHeap());
        pts.set("freeSketchSize", (int)ESP.getFreeSketchSpace());

        return pts;
    }

};
} // namespace a8::hal::esp