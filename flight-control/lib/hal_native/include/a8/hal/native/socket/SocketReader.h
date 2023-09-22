#pragma once
#include "a8/core/Runnable.h"
#include "a8/hal/Hal.h"
#include "a8/hal/Socket.h"
#include "a8/util/String.h"

namespace a8::hal::native::socket {
using a8::hal::S;
using a8::hal::socket::Socket;
using a8::util::String;

class SocketReader : public a8::core::Runnable {
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
                    S->out->println("Socket connect closed by server.");
                } else {
                    int error = socket->getLastError();
                    S->out->println(String::format("Failed with error:%i", error));
                }
                break; // end loop
            }
            
            String received = String(buf, len);
            S->out->println(":" + received);
        }
        S->out->println("Socket reader stop working.");
    }
};
} // namespace a8::hal::native::socket