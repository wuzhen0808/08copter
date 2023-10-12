#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/defines.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class FcBridge {

public:
    static int bridge(int type, void *data, void *context, Result &rst) {
        FcApi *fcApi = Lang::cast<FcApi *>(context);
        int ret = -1;
        switch (type) {
        case CommonMessageType::PING: {

            String *pData = Lang::cast<String *>(data);
            ret = fcApi->ping(*pData, rst);
        } break;
        case CommonMessageType::TEST: {

            ret = fcApi->test(rst);
        } break;
        }
        return ret;
    }
};

} // namespace a8::link
