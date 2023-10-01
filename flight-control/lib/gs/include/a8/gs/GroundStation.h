#pragma once
#include "a8/util/Component.h"
#include "a8/util/Sockets.h"

#include <ncurses/panel.h>

using namespace a8::util;

namespace a8::gs {

class GroundStation : public Component {
    Sockets *sockets;
    SOCK sock;
    int port;
    GroundStation(Sockets *sockets, int port) : Component("gs") {
        this->sockets = sockets;
        this->port = port;
    }

    void setup(Context *context) override {
        Component::setup(context);
        // connect to fcs
        this->sock = sockets->socket();
        Result rst = sockets->connect(sock, "127.0.0.1", port);
        if (rst.error != 0) {
            context->stop(String::format("cannot connect to fcs with port:%i", port));
            return;
        }
    }

    void run() override {
        bool running = true;
        char 
        while (running) {

            sockets->receive(sock, buf, len);
        }
    }

    int decodeMessageLen() {

    }
    

    void shutdown(Context *context) override {
        this->sockets->close(sock);
        Component::shutdown(context);
    }
};

} // namespace a8::gs