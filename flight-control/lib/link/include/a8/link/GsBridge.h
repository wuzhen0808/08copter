#pragma once
#include "a8/link/GsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

namespace a8::link {
using namespace a8::util;
using namespace a8::util::net;


// Stub to ground station interface.
class GsBridge {
public:
    static int bridge(int type, void *data, void *context, Result &rst) {
        GsApi *gss = Lang::cast<GsApi *>(context);
        int ret = -1;
        switch (type) {
        case CommonMessageType::PING:
            ret = gss->ping(*Lang::cast<String *>(data), rst);
            break;
        case CommonMessageType::LOG:
            ret = gss->log(*Lang::cast<String *>(data), rst);
            break;
        case CopterMessageType::SENSORS_DATA:
            ret = gss->sensors(*Lang::cast<SensorsData *>(data), rst);
            break;
        default:
            ret = gss->unknownMessageReceived(type, data, rst);
        }
        return ret;
    }
};

} // namespace a8::link
