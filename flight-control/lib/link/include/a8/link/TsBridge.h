#pragma once
#include "a8/link/TsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

namespace a8::link {
using namespace a8::util;
using namespace a8::util::net;


// Bridge to transmitter interface.
class TsBridge {
public:
    static int bridge(int type, void *data, void *context, Result &rst) {
        TsApi *tss = Lang::cast<TsApi *>(context);
        int ret = -1;
        switch (type) {
        case CommonMessageType::PING:
            ret = tss->ping(*Lang::cast<String *>(data), rst);
            break;
        default:
            ret = -1;
        }
        return ret;
    }
};

} // namespace a8::link
