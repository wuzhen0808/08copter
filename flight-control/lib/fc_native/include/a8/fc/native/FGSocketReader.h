#pragma once
#include "a8/fc/defines.h"
#include "a8/hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/net.h"

#include "winsock.h"

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::hal;
using namespace a8::hal::native;

namespace a8::fc::native {
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
        Result res;
        int ret = this->sockets->socket(server, res);
        if (ret < 0) {
            context->stop(res.errorMessage);
            return;
        }
        String address = "127.0.0.1";

        Result bResult;
        ret = this->sockets->bind(server, address, bindPort, bResult); //
        if (ret < 0) {
            context->stop(bResult.errorMessage);
            return;
        }

        Result lResult;
        ret = this->sockets->listen(server, lResult);

        if (ret < 0) {

            context->stop(lResult.errorMessage);

            return;
        }

        log(String::format("Waiting the JSBSim start up and connect to the the address:%s:%i", address.getText(), bindPort));

        ret = this->sockets->accept(server, this->sockIn);
        if (ret < 0) {
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

} // namespace a8::fc::native