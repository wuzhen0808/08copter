#pragma once
#include "a8/hal/Hal.h"
#include "a8/util/Component.h"
#include "a8/util/Result.h"
#include "a8/util/Socket.h"
#include "a8/util/String.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {

class JSBSimIO : public Component {
private:
    SocketFactory *socketFactory;
    SocketClient *client;
    SocketServer *server;
    String connectHost;
    int connectPort;

    int bindPort;
    Socket *socketIn;

public:
    JSBSimIO(SocketFactory *socketFactory);
    void setConnectionAddress(String host, int port) {
        this->connectHost = host;
        this->connectPort = port;
    }
    void setBindPort(int port) {
        this->bindPort = port;
    }
    void populate(Context *context) override;
    void setup(Context *context) override;

    void setThrottleNorm(int id, float throttle) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", id, throttle);
        int sent = client->send(&sb);
    }
};
} // namespace a8::hal::native