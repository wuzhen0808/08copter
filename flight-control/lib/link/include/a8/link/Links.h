#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/GsApi.h"
#include "a8/link/skeleton/FcSkeleton.h"
#include "a8/link/skeleton/GsSkeleton.h"
#include "a8/link/stub/FcStub.h"
#include "a8/link/stub/GsStub.h"

#include "a8/util/net.h"

using namespace a8::util::net;
namespace a8::link {

typedef void *(*stubFacFunc)(Channel *);

typedef void *(*skeletonFacFunc)(Channel *);

class Links {

    Network *network;
    String host = "127.0.0.1";
    int fcsPort = 8001;
    int gsPort = 8002;
    //
    int gsAddress;
    int fcAddress;

public:
    Links(Sockets *sockets) {
        SimpleCodec *codec = new SimpleCodec();

        // register codecs
        codec->add(CommonMessageType::PING, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::LOG, CodecFunc::writeString, CodecFunc::readString);
        codec->add(CommonMessageType::CMD, CodecFunc::writeString, CodecFunc::readString);
        
        network = new Network(sockets, codec, 0); //

        // register address
        this->gsAddress = network->add(host, gsPort);
        this->fcAddress = network->add(host, fcsPort);
    }

    int getStub(FcApi *&api, Result &errorMessage) {
        Channel *channel;
        int rst = network->connect(fcAddress, channel, 0, 0, errorMessage);
        if (rst > 0) {
            api = new FcStub(channel);
        }
        return rst;
    }

    int getStub(Channel *&channel, GsApi *&api, FcSkeleton *skeleton, Result &res) {
        int rst = network->connect(gsAddress, channel, GsSkeleton::handle, skeleton, res);
        if (rst > 0) {
            api = new GsStub(channel);
            res.successMessage << "successfully connect to gs address:" << host << ":" << gsPort;
        }
        return rst;
    }

    int bindGs(Result &errorMessage) {
        return network->bind(gsAddress, errorMessage);
    }

    int listenGs(Result &errorMessage) {
        return network->listen(gsAddress, errorMessage);
    }

    int acceptGs(Channel *&channel, FcStub *&fcStub, GsSkeleton *skeleton, Result &rst) {
        int ret = network->accept(gsAddress, channel, GsSkeleton::handle, skeleton, rst);
        if (ret < 0) {
            return ret;
        }
        fcStub = new FcStub(channel);
        return ret;
    }

    int listenFc(FcSkeleton *skeleton, Result &em) {
        return network->listen(fcAddress, em);
    }
    int acceptFc(Channel *&channel, GsStub *&gsStub, FcSkeleton *skeleton, Result &rst) {
        int ret = network->accept(fcAddress, channel, FcSkeleton::handle, skeleton, rst);
        if (ret < 0) {
            return ret;
        }
        gsStub = new GsStub(channel);
        return ret;
    }
};
} // namespace a8::link