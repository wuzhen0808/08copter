#pragma once
#include "a8/link/FcBridge.h"
#include "a8/link/GsBridge.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util::net;

namespace a8::link {

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Links : public Network {

    SimpleCodec *codec;
    //
    String host = "127.0.0.1";
    int fcPort = 8001;
    int gsPort = 8002;

    Address *fcAddress_;
    Address *gsAddress_;

public:
    static Codec *buildCodec() {
        SimpleCodec *codec = new SimpleCodec();
        // register codecs
        codec->add(CommonMessageType::PING, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::LOG, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::CMD, CodecFunc::writeString, CodecFunc::readString);
        return codec;
    }
    Links(Sockets *sockets, Scheduler *scheduler, LoggerFactory *logFac) : Network(sockets, buildCodec(), scheduler, logFac) {
        this->gsAddress_ = address(GsBridge::bridge, host, gsPort);
        this->fcAddress_ = address(FcBridge::bridge, host, fcPort);
    }

    Address *gsAddress() {
        return this->gsAddress_;
    }

    Address *fcAddress() {
        return this->fcAddress_;
    }
};

} // namespace a8::link
