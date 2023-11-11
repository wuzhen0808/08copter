#pragma once
#include "a8/link/FcBridge.h"
#include "a8/link/GsBridge.h"
#include "a8/link/LineBridge.h"
#include "a8/link/TsBridge.h"
#include "a8/link/defines.h"
#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::link {
using namespace a8::util;
using namespace a8::util::net;

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Links : public Network {

    //
    String tsHost = "127.0.0.1";
    int tsPort = 8001;

    String fcHost = "127.0.0.1";
    int fcPort = 8002;

    String gsHost = "127.0.0.1";
    int gsPort = 8003;

    String simHost = "127.0.0.1";
    int simOutPort = 5126;
    int simInPort = 5502;

    Address *fcAddress_;
    Address *gsAddress_;
    Address *tsAddress_;

    Address *simOutAddress_;
    Address *simInAddress_;

public:
    Links(Sockets *sockets, Scheduler *scheduler, LoggerFactory *logFac) : Network(sockets, scheduler, logFac) {
    }
    Links *ts(String host, int port) {
        this->tsHost = host;
        this->tsPort = port;
        return this;
    }
    Links *fc(String host, int port) {
        this->fcHost = host;
        this->fcPort = port;
        return this;
    }
    Links *gs(String host, int port) {
        this->gsHost = host;
        this->gsPort = port;
        return this;
    }

    Links *build() {
        SimpleCodec *codec = new SimpleCodec();

        // register codecs
        codec->add<String>(CommonMessageType::PING, CodecUtil::writeString, CodecUtil::readString, [](String &def) { def << "hi"; });
        codec->add<String>(CommonMessageType::LOG, CodecUtil::writeString, CodecUtil::readString, [](String &def) { def << "hi"; });
        codec->add<String>(CommonMessageType::CMD, CodecUtil::writeString, CodecUtil::readString, [](String &def) { def << "hi"; });
        codec->add(CommonMessageType::TEST);
        codec->add<SensorsData>(CopterMessageType::SENSORS_DATA, SensorsData::write, SensorsData::read);

        // address

        this->tsAddress_ = this->address(TsBridge::bridge, tsHost, tsPort, codec);
        this->fcAddress_ = this->address(FcBridge::bridge, fcHost, fcPort, codec);
        this->gsAddress_ = this->address(GsBridge::bridge, gsHost, gsPort, codec);

        // address
        Codec *codec2 = new LineCodec(0, '\n');
        this->simOutAddress_ = this->address(LineBridge::bridge, simHost, simOutPort, codec2);
        this->simInAddress_ = this->address(LineBridge::bridge, simHost, simInPort, codec2);

        return this;
    }
    Address *tsAddress() {
        return this->tsAddress_;
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
