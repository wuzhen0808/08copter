#pragma once
#include "a8/hal/Hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FGSocketLineReader.h"
#include "a8/hal/native/FGSocketStructReader.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/Component.h"
#include "a8/util/Math.h"
#include "a8/util/Rate.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/Sockets.h"
#include "a8/util/String.h"
#include "a8/util/Vector3f.h"
#include "a8/util/ReadWriteRunner.h"
#include "a8/util/RunnerComponent.h"


using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {

class JSBSimIO : public Component {
private:
    Sockets *sockets;
    SOCK client;

    Writer *consoleWriter;

    FGSocketReader *fgSocketReader;

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

    void populate(Context &context) override {
        Component::populate(context);
    }
    void setup(Context &context) override {

        this->fgSocketReader = this->addChild<FGSocketReader>(context, new FGSocketLineReader(this->sockets));

        client = this->sockets->socket();
        if (client == 0) {
            context.stop("Error, cannot create socket.");
            return;
        }
        log("Connecting to JSBSim ...");
        String connectHost = context.properties.getString("todo", "127.0.0.1");
        int connectPort = context.properties.getInt("todo", 5126);

        Result connected = this->sockets->connect(client, connectHost, connectPort);
        if (connected.error) {
            context.stop("Failed connect to JSBSim");
            return;
        }
        log("Successfully connected to JSBSim.");

        Runnable *runner2 = new ReadWriteRunner(new SocketReader(sockets, client), consoleWriter);
        Component *com = this->addChild<RunnerComponent>(context, new RunnerComponent("rwr", runner2));
    }

    void setThrottleNorm(int id, float throttle) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", id, throttle);
        int sent = sockets->send(client, sb);
    }
    Vector3f getAngVel() {
        SocketData *fdm = fgSocketReader->getLastData();
        Vector3f ret = Vector3f(fdm->phi, fdm->theta, fdm->psi);
        return Math::radian2Degree(ret);
    }
    Vector3f getAccVel() {
        SocketData *fdm = fgSocketReader->getLastData();
        Vector3f ret = Vector3f(fdm->A_X_pilot, fdm->A_Y_pilot, fdm->A_Z_pilot);
        return Math::feetToMeters(ret);
    }
    double getAgl() {
        SocketData *fdm = fgSocketReader->getLastData();
        return fdm->agl;
    }
    double getAlt() {
        SocketData *fdm = fgSocketReader->getLastData();
        return fdm->altitude;
    }
};
} // namespace a8::hal::native