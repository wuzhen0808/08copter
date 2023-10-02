#pragma once
#include "a8/link/GsApi.h"
#include "a8/link/FcsApi.h"
#include "a8/link/skeleton/FcsSkeleton.h"
#include "a8/link/skeleton/GsSkeleton.h"
#include "a8/link/stub/FcsStub.h"
#include "a8/link/stub/GsStub.h"

#include "a8/link/SimpleCodec.h"
#include "a8/util.h"

namespace a8::link {

class Links {

    Network *network;
    String host = "127.0.0.1";
    int fcsPort;
    int gsPort;
    //
    int gsAddress;
    int fcsAddress;

public:
    Links(Sockets *sockets) {
        Codec *codec = new SimpleCodec();
        network = new Network(sockets, codec, 0); //
        // register codecs
        network->add(CommonMessageType::PING, Functions::encodeString, Functions::decodeString);
        network->add(CommonMessageType::LOG, Functions::encodeString, Functions::decodeString);
        this->gsAddress = network->add(host, gsPort);
        this->fcsAddress = network->add(host, fcsPort);
    }

    int getGsStub(GsApi *&ret) {
        Channel *channel;
        int rst = network->connect(gsAddress, channel, 0, 0);
        if (rst > 0) {
            ret = new GsStub(channel);
        }

        return rst;
    }

    int getFcsStub(FcsApi *&ret) {
        Channel *channel;
        int rst = network->connect(gsAddress, channel, 0, 0);
        if (rst > 0) {
            ret = new FcsStub(channel);
        }

        return rst;
    }

    void bindGsAddress() {
        network->bind(gsAddress);
    }

    void bindFcsAddress() {
        network->bind(fcsAddress);
    }

    void listen(GsSkeleton *skeleton) {
        network->listen(gsAddress, GsSkeleton::handle, skeleton);
    }

    void listen(FcsSkeleton *skeleton) {
        network->listen(fcsAddress, FcsSkeleton::handle, skeleton);
    }
};
} // namespace a8::link