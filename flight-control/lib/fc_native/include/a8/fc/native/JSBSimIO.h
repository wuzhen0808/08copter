#pragma once
#include "a8/fc/native/FGSocketLineReader.h"
#include "a8/fc/native/FGSocketStructReader.h"
#include "a8/hal.h"
#include "a8/hal/native.h"
#include "a8/util.h"

using namespace a8::util;
using namespace a8::hal::native;
using namespace a8::hal;

namespace a8::fc::native {

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

    void populate(Context *context) override {
        Component::populate(context);
    }
    void setup(Context *context) override {

        this->fgSocketReader = new FGSocketLineReader(this->sockets);
        this->addChild(context, fgSocketReader);

        Result res;
        int ret = this->sockets->socket(client, res);
        if (ret < 0) {
            context->stop(res);
            return;
        }
        log("Connecting to JSBSim ...");
        String connectHost = context->properties->getString("todo", "127.0.0.1");
        int connectPort = context->properties->getInt("todo", 5126);

        Result connected;
        ret = this->sockets->connect(client, connectHost, connectPort, connected);
        if (ret < 0) {
            context->stop(connected);
            return;
        }
        log("Successfully connected to JSBSim.");

        Component *com = new RunnerComponent("rwr",                                               //
                                             new ReadWriteRunner(                                 //
                                                 new SocketReader(sockets, client), consoleWriter //
                                                 )                                                //
        );
        this->addChild(context, com);
    }

    void setThrottleNorm(int id, float throttle) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", id, throttle);
        int sent = sockets->send(client, sb.getText(), sb.getLength());
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

    double getAltitude() {
        SocketData *fdm = fgSocketReader->getLastData();
        return fdm->altitude;
    }
};
} // namespace a8::fc::native