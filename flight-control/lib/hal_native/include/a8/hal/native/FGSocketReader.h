#pragma once
#include "a8/fcs/defines.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/Component.h"
#include "a8/util/Math.h"
#include "a8/util/Rate.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/Sockets.h"
#include "a8/util/String.h"
#include "a8/util/Vector3f.h"
#include "input_output/net_fdm.hxx"
#include "winsock.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {
class SocketData {
public:
    double altitude;
    double agl;
    float phi;
    float theta;
    float psi;
    float A_X_pilot;
    float A_Y_pilot;
    float A_Z_pilot;
};

class FGSocketReader : public Component {

protected:
    Sockets *sockets;
    SOCK server;
    SOCK sockIn;
    SocketData *lastData;
    Writer *dataFileWriter;

public:
    FGSocketReader(Sockets *socketFactory) : Component("sor") {
        rate = Rate::ForEver;
        this->sockets = socketFactory;
        this->server = 0;
        this->sockIn = 0;
        this->lastData = 0;
    }

    SocketData *getLastData() {
        return lastData;
    }

    virtual void setup(Context *context) override {

        int bindPort = context->properties->getInt(P_sim_jsb_sim_bind_port, 5502);
        String dataFile = context->properties->getString(P_sim_jsb_sim_data_file, "");

        this->dataFileWriter = new FileWriter(dataFile); // todo write data log

        // TODO move the server to the context ?
        server = this->sockets->socket();
        if (server == 0) {
            context->stop(String::format("cannot create new socket from socket factory."));
            return;
        }
        String address = "127.0.0.1";

        Result bResult = this->sockets->bind(server, address, bindPort); //
        if (bResult.error != 0) {
            context->stop(String::format("cannot bind to port:%i, error:%i", bindPort, bResult.error));
            return;
        }

        Result lResult = this->sockets->listen(server);

        if (lResult.error != 0) {
            context->stop(String::format("cannot listen on port:%i", bindPort));
            return;
        }

        log(String::format("Waiting the JSBSim start up and connect to the the address:%s:%i", address.getText(), bindPort));

        this->sockIn = this->sockets->accept(server);
        if (sockIn == 0) {
            context->stop("cannot accept connect from JSBSim.");
            return;
        }
        log("JSBSim connected in, trying to receive the first package of data.");
    }

    virtual void postSetup(Context *context) override {
        Component::postSetup(context);
    }

    virtual bool update() {
        SocketData *data = new SocketData();
        bool success = update(data);
        if (success) {
            SocketData *llData = this->lastData;
            this->lastData = data;
            if (llData != 0) {
                delete llData;
            }
            return true;
        } else {
            delete data;
            return false;
        }
    }
    virtual void run() override {
        bool running = true;
        while (running) {
            running = update();
        }
        log("Error,cannot receive data from JSBSim. Socket connection may broken?");
    }

    virtual bool update(SocketData *data) = 0;
};

} // namespace a8::hal::native