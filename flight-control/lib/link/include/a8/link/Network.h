#pragma once
#include "a8/link/FcBridge.h"
#include "a8/link/GsBridge.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

namespace a8::link {

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Network {

    Sockets *sockets;
    SimpleCodec *codec;
    //
    String host = "127.0.0.1";
    int fcPort = 8001;
    int gsPort = 8002;

    Address *fcAddress_;
    Address *gsAddress_;

public:
        
    Network(Sockets *sockets) {

        this->sockets = sockets;
        codec = new SimpleCodec();
        // register codecs
        codec->add(CommonMessageType::PING, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::LOG, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::CMD, CodecFunc::writeString, CodecFunc::readString);

        this->gsAddress_ = new Address(this->sockets, this->codec, GsBridge::bridge, host, gsPort);
        this->fcAddress_ = new Address(this->sockets, this->codec, FcBridge::bridge, host, fcPort);
    }

    Address *gsAddress() {
        return this->gsAddress_;
    }

    Address *fcAddress() {
        return this->fcAddress_;
    }
};

} // namespace a8::link
