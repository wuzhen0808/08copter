#pragma once
#include "a8/hal/Hal.h"
#include "a8/hal/native/ConsoleWriter.h"
#include "a8/hal/native/FileWriter.h"
#include "a8/util/Component.h"
#include "a8/util/Math.h"
#include "a8/util/Rate.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/Socket.h"
#include "a8/util/String.h"
#include "a8/util/Vector3f.h"
#include "input_output/net_fdm.hxx"
#include "a8/core/defines.h"
#include "winsock.h"

using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {
class FGSocketOuputReader : public Component {

private:
    SocketFactory *socketFactory;
    FGNetFDM lastFGNetData;

    uint32_t cur_us;


    SocketServer *server;
    Socket *socketIn;
    Writer *dataFileWriter;
    

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
    FGSocketOuputReader(SocketFactory *socketFactory) : Component("sor") {
        rate = Rate::ForEver;
        this->socketFactory = socketFactory;
    }

    FGNetFDM getLastFGNetData() {
        return lastFGNetData;
    }

    virtual void setup(Context &context) override {
        
        int bindPort = context.properties.getInt(P_jsb_bind_port, 5502);
        String dataFile = context.properties.getString(P_jsb_data_file,"");
        
        this->dataFileWriter = new FileWriter(dataFile);//todo write data log

        // TODO move the server to the context ?
        this->server = this->socketFactory->socketServer();
        String address = "127.0.0.1";

        Result bResult = this->server->bind(address, bindPort); //
        if (bResult.error != 0) {
            context.stop(String::format("cannot bind to port:%i, error:%i", bindPort, bResult.error));
            return;
        }

        Result lResult = this->server->listen();

        if (lResult.error != 0) {
            context.stop(String::format("cannot listen on port:%i", bindPort));
            return;
        }

        log(String::format("Waiting the JSBSim start up and connect to the the address:%s:%i", address.getText(), bindPort));

        this->socketIn = this->server->accept();
        if (!this->socketIn) {
            context.stop("cannot accept connect from JSBSim.");
            return;
        }
        log("JSBSim connected in, trying to receive the first package of data.");

        bool success = receive(lastFGNetData);
        if (!success) {
            context.stop("Failed to the get the connection ready from JSBSim.");
        }
    }

    virtual void run() override {
        while (true) {
            bool success = receive(lastFGNetData);
            if (success) {
                continue;
            }
            // exit loop?
            break;
        }
        log("Error, socket connection broken from JSBSim.");
    }

    bool receive(FGNetFDM &data) {
        int size = sizeof(FGNetFDM);
        char buf[size];

        int received = 0;
        char *from = buf;
        bool broken = false;
        while (true) {

            int len = socketIn->receive(from, size - received);
            if (len <= 0) {
                if (len == 0) {
                    // remote closed the connection
                }
                // error occurred while reading.
                broken = true;
                break;
            }

            received += len;
            if (received == size) {
                break;
            }
        }
        if (broken) {
            return false;
        }
        byteSwap(buf, size);
        data = *(FGNetFDM *)buf;
        log("Data got from JSBSim.");
        return true;
    }
};

} // namespace a8::hal::native