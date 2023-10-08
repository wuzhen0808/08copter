#pragma once
#include "a8/link/FcBridge.h"
#include "a8/link/GsBridge.h"
#include "a8/link/LineBridge.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util::net;

namespace a8::link {

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Links : public Network {

    //
    String host = "127.0.0.1";
    int fcPort = 8001;
    int gsPort = 8002;
    int simOutPort = 5126;
    int simInPort = 5502;

    Address *fcAddress_;
    Address *gsAddress_;
    Address *simOutAddress_;
    Address *simInAddress_;

public:
    Links(Sockets *sockets, Scheduler *scheduler, LoggerFactory *logFac) : Network(sockets, scheduler, logFac) {

        SimpleCodec *codec = new SimpleCodec();
        // register codecs
        codec->add(CommonMessageType::PING, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::LOG, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::CMD, CodecFunc::writeString, CodecFunc::readString);
        // address
        this->gsAddress_ = address(GsBridge::bridge, host, gsPort, codec);
        this->fcAddress_ = address(FcBridge::bridge, host, fcPort, codec);
        // address
        Codec *codec2 = new LineCodec('\n');
        this->simOutAddress_ = address(LineBridge::bridge, host, simOutPort, codec2);
        this->simInAddress_ = address(LineBridge::bridge, host, simInPort, codec2);
    }

    Address *gsAddress() {
        return this->gsAddress_;
    }

    Address *fcAddress() {
        return this->fcAddress_;
    }

    Address *simOutAddress() {
        return this->simOutAddress_;
    }

    Address *simInAddress() {
        return this->simInAddress_;
    }
};

} // namespace a8::link
