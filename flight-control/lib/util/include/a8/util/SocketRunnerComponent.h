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
#include "a8/util/SocketConnector.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {


class SocketRunnerComponent : public Component {

protected:
    Sockets *sockets;
    SOCK server;
    int port;    
    SocketConnector * connector;
public:
    SocketRunnerComponent(String name, Sockets *socketFactory, int port, SocketConnector * connector) : Component(name) {
        rate = Rate::ForEver;
        this->sockets = socketFactory;
        this->server = 0;
        this->connector = connector;        
    }

    virtual void setup(Context *context) override {

        // TODO move the server to the context ?
        server = this->sockets->socket();
        if (server == 0) {
            context->stop(String::format("cannot create new socket from socket factory."));
            return;
        }
        String address = "127.0.0.1";

        Result bResult = this->sockets->bind(server, address, port); //
        if (bResult.error != 0) {
            context->stop(String::format("cannot bind to port:%i, error:%i", port, bResult.error));
            return;
        }

        Result lResult = this->sockets->listen(server);

        if (lResult.error != 0) {
            context->stop(String::format("cannot listen on port:%i", port));
            return;
        }
    }

    virtual void postSetup(Context *context) override {
        Component::postSetup(context);
    }

    virtual void run() override {
        bool running = true;
        while (running) {

            SOCK sockIn = this->sockets->accept(server); // TODO async accept?
            if (sockIn == 0) {                           // What's wrong?
                continue;
            }
            // TODO setup a new thread to run the interaction.
            connector->onConnection(sockets, sockIn);
        }

        log("Error,Socket connection may broken?");
    }
    
};

} // namespace a8::hal::native