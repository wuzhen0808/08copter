#pragma once
#include "a8/link/GsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to ground station interface.
class GsBridge {
public:
    static void bridge(int type, void *data, void *context) {
        GsApi *gss = static_cast<GsApi *>(context);
        switch (type) {
        case CommonMessageType::PING:
            gss->ping(*static_cast<String *>(data));
            break;
        case CommonMessageType::LOG:
            gss->log(*static_cast<String *>(data));
            break;
        case CopterMessageType::SENSORS_DATA:
            gss->sensors(*static_cast<SensorsData *>(data));
            break;
        default:
            gss->unknownMessageReceived(type, data);
        }
    }
};

} // namespace a8::link
