
#pragma once
#include "a8/core/Copter.h"
#include "a8/hal/Socket.h"
#include "a8/util/Result.h"
#include <iostream>

namespace a8::hal::native {
using namespace std;
using namespace a8::util;
using namespace a8::core;
using a8::hal::socket::Socket;
using a8::hal::socket::SocketFactory;

class NativeCopter : public Copter {
private:
    SocketFactory *socketFactory_;
    Socket *socket_;
    
    String host;
    int port;

protected:
    virtual ServosControl *setupServosControl();
    virtual AttitudeSensor *setupAttitudeSensor();
    virtual AttitudeControl *setupAttitudeControl(ServosControl *sc, AttitudeSensor *as);

public:
    NativeCopter(Scheduler *scheduler, SocketFactory *sFac, String host, int port);
    ~NativeCopter();
    virtual Result setup();
    virtual Result start();
    virtual void stop();
};

} // namespace a8::hal::native
