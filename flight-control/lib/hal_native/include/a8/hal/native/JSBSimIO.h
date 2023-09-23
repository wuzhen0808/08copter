#pragma once
#include "a8/hal/Hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/Component.h"
#include "a8/util/Math.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/Socket.h"
#include "a8/util/String.h"
#include "a8/util/Vector3f.h"
#include "input_output/net_fdm.hxx"
#include "winsock.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {
class FGSocketOuputReadRunner : public Runnable {

private:
    FGNetFDM lastFGNetData;

    Socket *socket;
    uint32_t cur_us;

    static void byteSwap(char *buffer, int len) {
        uint32_t *buf = (uint32_t *)buffer;
        for (int i = 0; i < len / 4; i++) {
            buf[i] = ntohl(buf[i]);
        }
        // fixup the 3 doubles
        doubleSwap(buf, offsetof(FGNetFDM, longitude));
        doubleSwap(buf, offsetof(FGNetFDM, altitude));
        doubleSwap(buf, offsetof(FGNetFDM, latitude));
    }
    static void doubleSwap(uint32_t *buf, int offset) {
        uint32_t tmp = buf[offset];
        buf[offset] = buf[offset + 1];
        buf[offset + 1] = tmp;
    }

public:
    FGSocketOuputReadRunner(Socket *socket) {
        this->socket = socket;
    }

    FGNetFDM getLastFGNetData() {
        return lastFGNetData;
    }

    virtual void run() override {

        int size = sizeof(FGNetFDM);
        char buf[size];

        int received = 0;
        char *from = buf;
        while (true) {

            int len = socket->receive(from, size - received);
            if (len <= 0) {
                if (len == 0) {
                    // remote closed the connection
                }
                // error occurred while reading.
                break;
            }

            received += len;
            if (received == size) {
                break;
            }
        }
        byteSwap(buf, size);

        lastFGNetData = *(FGNetFDM *)buf;
    }
};

class JSBSimIO : public Component {
private:
    SocketFactory *socketFactory;
    SocketClient *client;
    SocketServer *server;
    String connectHost;
    int connectPort;

    int bindPort;
    Socket *socketIn;
    String dataFile;
    Writer *dataFileWriter;
    Writer *consoleWriter;

    FGSocketOuputReadRunner *runner;

public:
    JSBSimIO(SocketFactory *socketFactory, String dataFile) {
        this->socketFactory = socketFactory;
        this->dataFile = dataFile;
        this->dataFileWriter = new FileWriter(dataFile);
        this->consoleWriter = new ConsoleWriter();
        this->runner = 0;
    }

    ~JSBSimIO() {
        delete dataFileWriter;
        delete consoleWriter;
        // TODO release runner from scheduler, and delete it here;
    }
    void setConnectionAddress(String host, int port) {
        this->connectHost = host;
        this->connectPort = port;
    }
    void setBindPort(int port) {
        this->bindPort = port;
    }
    void populate(Context &context) override;
    void setup(Context &context) override;

    void setThrottleNorm(int id, float throttle) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", id, throttle);
        int sent = client->send(&sb);
    }
    Vector3f getAngVel() {
        FGNetFDM fdm = runner->getLastFGNetData();
        Vector3f ret = Vector3f(fdm.phi, fdm.theta, fdm.psi);
        return Math::radian2Degree(ret);
    }
    Vector3f getAccVel() {
        FGNetFDM fdm = runner->getLastFGNetData();
        Vector3f ret = Vector3f(fdm.A_X_pilot, fdm.A_Y_pilot, fdm.A_Z_pilot);
        return Math::feetToMeters(ret);
    }
};
} // namespace a8::hal::native