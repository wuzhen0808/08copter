#pragma once
#include "a8/util/Runnable.h"
#include "a8/hal/Hal.h"
#include "a8/util/Socket.h"
#include "a8/util/String.h"

using namespace a8::util;
namespace a8::util {

class SocketReader : public Runnable {
private:
    Socket *socket;
    bool running = true;

public:
    SocketReader(Socket *socket) {
        this->socket = socket;
    }

    virtual void run() {
        while (running) {
            char buf[100];
            int len = socket->receive(buf, 100);
            if (len <= 0) {
                if (len == 0) {
                    // S->out->println("Socket connect closed by server.");
                } else {
                    int error = socket->getLastError();
                    // S->out->println(String::format("Failed with error:%i", error));
                }
                break; // end loop
            }

            String received = String(buf, len);
            // S->out->println(":" + received);
        }
        // S->out->println("Socket reader stop working.");
    }
};
} // namespace a8::util