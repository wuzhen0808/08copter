#pragma once
#include "a8/util.h"
#include "a8/util/rf24.h"

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

namespace a8::hal::rf24 {
using namespace a8::util::rf24;

class Rf24HalImpl : public Rf24Hal, public FlyWeight {
    class RadioImpl : public Rf24Hal::Radio {
    public:
        RF24 *radio;
        RadioImpl(int ce, int cs) {
            this->radio = new RF24(ce, cs);
        }
        ~RadioImpl() {
            delete this->radio;
        }
        bool begin() override {
            return this->radio->begin();
        }
        void setChannel(int channel) override {
            this->radio->setChannel(channel);
        }
    };
    class NetworkImpl : public Rf24Hal::Network {
    public:
        RF24Network *net;
        NetworkImpl(RF24 &radio) {
            this->net = new RF24Network(radio);
        }
        void begin(int nodeId) override {
            return this->net->begin(nodeId);
        }
        void update() override {
            this->net->update();
        }

        bool available() override {
            return this->net->available();
        }
        int peek() override {
            RF24NetworkHeader header;
            return this->net->peek(header);
        }

        int read(char *buf, int bLen) override {
            RF24NetworkHeader header;
            int size = this->net->peek(header);
            if (size == 0) {
                return 0;
            }
            int size2 = this->net->read(header, buf, size);
            if (size2 != size) {
                // return -1;
            }
            return size2;
        }
        int write(int nodeId, char *buf, int bLen) override {
            RF24NetworkHeader header(nodeId);
            bool ok = this->net->write(header, buf, bLen);
            if (!ok) {
                return -1;
            }
            return bLen;
        }
    };

public:
    Rf24HalImpl(LoggerFactory *logFac) : FlyWeight(logFac, "Rf24HalImpl") {
    }

    ~Rf24HalImpl() {
    }
    Radio *newRadio(int ce, int cs) {

        return new RadioImpl(ce, cs);
    }

    Network *newNetwork(Radio *radio) {
        RadioImpl *radioImpl = static_cast<RadioImpl *>(radio);
        return new NetworkImpl(*radioImpl->radio);
    }
};

} // namespace a8::hal::rf24