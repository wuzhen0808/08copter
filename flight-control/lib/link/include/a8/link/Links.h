#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/GsApi.h"
#include "a8/link/skeleton/FcSkeleton.h"
#include "a8/link/skeleton/GsSkeleton.h"
#include "a8/link/stub/FcStub.h"
#include "a8/link/stub/GsStub.h"

#include "a8/link/SimpleCodec.h"
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
        Codec *codec = new SimpleCodec();
        network = new Network(sockets, codec, 0); //

        // register address
        this->gsAddress = network->add(host, gsPort);
        this->fcAddress = network->add(host, fcsPort);

        // register codecs
        network->add(CommonMessageType::PING, Functions::encodeString, Functions::decodeString);
        network->add(CommonMessageType::LOG, Functions::encodeString, Functions::decodeString);
    }

    int getStub(FcApi *&api, Result &errorMessage) {
        Channel *channel;
        int rst = network->connect(fcAddress, channel, 0, 0, errorMessage);
        if (rst > 0) {
            api = new FcStub(channel);
        }
        return rst;
    }

    int getStub(GsApi *&api, Result &res) {
        Channel *channel;
        int rst = network->connect(gsAddress, channel, 0, 0, res);
        if (rst > 0) {
            api = new GsStub(channel);
            res.successMessage << "successfully connect to gs address:" << host << ":" << gsPort;
        }
        return rst;
    }

    int bindGs(Result &errorMessage) {
        return network->bind(gsAddress, errorMessage);
    }

    int listen(GsSkeleton *skeleton, Result &errorMessage) {
        return network->listen(gsAddress, GsSkeleton::handle, skeleton, errorMessage);
    }

    int listen(FcSkeleton *skeleton, Result &em) {
        return network->listen(fcAddress, FcSkeleton::handle, skeleton, em);
    }
};
} // namespace a8::link