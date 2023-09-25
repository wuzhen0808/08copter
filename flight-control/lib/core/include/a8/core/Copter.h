#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__

#include "a8/core/AttitudeControl.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Hal.h"
#include "a8/util/Result.h"
#include "a8/util/Scheduler.h"
#include "a8/util/String.h"

using a8::hal::S;
using namespace a8::util;
namespace a8::core {

class Copter : public Component {
private:
protected:
    // members
    int totalServos_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
    // member functions
    virtual ServosControl *createServosControl(Context &context) = 0;
    virtual AttitudeSensor *createAttitudeSensor(Context &context) = 0;

public:
    Copter(const String & name) : Component(name){
        this->init();
        this->totalServos_ = 4;
    }
    Copter(const String & name , int totalServos) :Component(name){
        this->init();
        this->totalServos_ = totalServos;
    }
    void init(){
    }
    ~Copter() {
    }

    virtual void populate(Context &context) override {
        Component::populate(context);
        this->log(String::format(">>Copter::setup(),totalServos:%i", totalServos_));
        
        attitudeSensor_ = createAttitudeSensor(context);
        this->addChild(attitudeSensor_, context);
        
        servosControl_ = createServosControl(context);
        this->addChild(servosControl_, context);

        attitudeControl_ = new AttitudeControl(servosControl_, attitudeSensor_);
        this->addChild(attitudeControl_, context);
        //

        this->log("<<Copter::setup()");
    }
    virtual void setup(Context &context) override {
        Component::setup(context);
    }
    virtual void start(Context &context) override {
        Component::start(context);
    }

    int getServoCount() {
        return this->totalServos_;
    }

    virtual void stop() override {
    }
};

} // namespace a8::core
#endif