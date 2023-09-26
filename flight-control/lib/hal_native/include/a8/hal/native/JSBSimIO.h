#pragma once
#include "a8/hal/Hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FGSocketOutputReader.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/Component.h"
#include "a8/util/Math.h"
#include "a8/util/Rate.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/Sockets.h"
#include "a8/util/String.h"
#include "a8/util/Vector3f.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {

class JSBSimIO : public Component {
private:
    Sockets *sockets;
    SOCK client;

    Writer *consoleWriter;

    FGSocketOuputReader *fgSocketReader;

public:
    JSBSimIO(Sockets *socketFactory) : Component("jsb") {
        this->sockets = socketFactory;
        this->consoleWriter = new ConsoleWriter();
        this->fgSocketReader = 0;
    }

    ~JSBSimIO() {
        delete consoleWriter;
        // TODO release runner from scheduler, and delete it here;
    }

    void populate(Context &context) override;
    void setup(Context &context) override;

    void setThrottleNorm(int id, float throttle) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", id, throttle);
        int sent = sockets->send(client, sb);
    }
    Vector3f getAngVel() {
        FGNetFDM fdm = fgSocketReader->getLastFGNetData();
        Vector3f ret = Vector3f(fdm.phi, fdm.theta, fdm.psi);
        return Math::radian2Degree(ret);
    }
    Vector3f getAccVel() {
        FGNetFDM fdm = fgSocketReader->getLastFGNetData();
        Vector3f ret = Vector3f(fdm.A_X_pilot, fdm.A_Y_pilot, fdm.A_Z_pilot);
        return Math::feetToMeters(ret);
    }
    double getAlt() {
        FGNetFDM fdm = fgSocketReader->getLastFGNetData();
        return fdm.altitude;
    }
};
} // namespace a8::hal::native