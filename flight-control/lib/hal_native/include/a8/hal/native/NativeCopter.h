
#pragma once
#include "a8/core/Copter.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/util/Result.h"
#include "a8/util/Socket.h"
#include <iostream>

using namespace a8::hal::native;
using namespace std;
using namespace a8::util;
using namespace a8::core;

namespace a8::hal::native {

class NativeCopter : public Copter {
private:
    JSBSimIO *jsbSimIo;
    String host;
    int port;

protected:
    virtual ServosControl *setupServosControl(Context * context) override;
    virtual AttitudeSensor *setupAttitudeSensor(Context * context) override;

public:
    NativeCopter(JSBSimIO *jio) : Copter(4) {
        this->jsbSimIo = jio;
    }
    ~NativeCopter(){}

    virtual void setup(Context *context);
    virtual void start(Context *context);
    virtual void stop();
};

} // namespace a8::hal::native
