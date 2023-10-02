#pragma once

#include "a8/fc/AttitudeControl.h"
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/ServosControl.h"
#include "a8/hal.h"
#include "a8/util/Result.h"
#include "a8/util/Scheduler.h"
#include "a8/util/String.h"

using a8::hal::S;

using namespace a8::util;
namespace a8::fc {

class FlightControl : public Component {
private:
protected:
    // members
    int totalServos_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
    // member functions
    virtual ServosControl *createServosControl(Context *context) = 0;
    virtual AttitudeSensor *createAttitudeSensor(Context *context) = 0;

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

    virtual void populate(Context *context) override {
        Component::populate(context);

        attitudeSensor_ = createAttitudeSensor(context);
        this->addChild(context, attitudeSensor_);

        servosControl_ = createServosControl(context);
        this->addChild(context, servosControl_);

        attitudeControl_ = new AttitudeControl(servosControl_, attitudeSensor_);
        this->addChild(context, attitudeControl_);
        //

    }
    virtual void setup(Context *context) override {
        Component::setup(context);
    }
    virtual void start(Context *context) override {
        Component::start(context);
    }

    int getServoCount() {
        return this->totalServos_;
    }

    virtual void stop() override {
    }
};

} // namespace a8::fc