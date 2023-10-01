#pragma once
#include "a8/net/Encoder.h"
#include "a8/net/Types.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

using namespace a8::util;

namespace a8::net {

// Stub to ground station interface.
class GsStub {

    static int encodeString(void *string, char *&buf) {
        String *str = static_cast<String *>(string);
        int len;
        int cap = 0;
        Util::appendStr(buf, len, cap, 128, str->getText(), 0, str->getLength());
        return len;
    }

protected:
    Encoder *encoder;
    Writer *writer;
    virtual void send(int type, void *msg) {
        this->encoder->write(writer, type, msg);
    }

public:
    GsStub(Writer *writer) {
        this->writer = writer;
        this->encoder = new Encoder();
        this->encoder->set(Types::PING, encodeString);
        this->encoder->set(Types::LOG, encodeString);
    }

    // check if the GS responsible or not.
    void ping(String msg) {
        this->send(Types::PING, &msg);
    }

    // send log message to GS.
    void log(String msg) {
        this->send(Types::LOG, &msg);
    }
};

} // namespace a8::net
