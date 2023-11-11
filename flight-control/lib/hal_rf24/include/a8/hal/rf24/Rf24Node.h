#pragma once

#include "a8/hal/rf24/Rf24ChannelData.h"
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/util/net.h"
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

namespace a8::hal::rf24 {

class Rf24Node {
    int id;
    Rf24Hosts *hosts;
    RF24 *radio;
    RF24Network *network;

public:
    Rf24Node(Rf24Hosts *hosts, int id, int cePin, int csPin) {
        this->hosts = hosts;
        this->id = id;
        this->radio = new RF24(cePin, csPin);
        this->network = new RF24Network(*radio);
    }
    ~Rf24Node() {
        delete this->network;
        delete this->radio;
    }

    int getId() {
        return this->id;
    }
};

} // namespace a8::hal::rf24