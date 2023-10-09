#pragma once

#include "a8/fc/AttitudeControl.h"
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/ServosControl.h"
#include "a8/hal.h"
#include "a8/util/Result.h"
#include "a8/util/String.h"
#include "a8/util/thread.h"

using a8::hal::S;

using namespace a8::util;
using namespace a8::util::thread;
using namespace a8::util::comp;

namespace a8::fc {

class FlightControl : public Component {
protected:
    // members
    int totalServos_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
   
public:
    FlightControl(const String &name) : Component(name) {
        this->init();
        this->totalServos_ = 4;
    }
    FlightControl(const String &name, int totalServos) : Component(name) {
        this->init();
        this->totalServos_ = totalServos;
    }
    void init() {
        
    }
    ~FlightControl() {
    }

    virtual void populate(StagingContext *context) override {
        Component::populate(context);        
        attitudeControl_ = new AttitudeControl(servosControl_, attitudeSensor_);
        this->addChild(context, attitudeControl_);
        //
    }
    virtual void setup(StagingContext *context) override {
        Component::setup(context);
    }
    virtual void start(StagingContext *context) override {
        Component::start(context);
    }

    int getTotalServos() {
        return this->totalServos_;
    }

};

} // namespace a8::fc