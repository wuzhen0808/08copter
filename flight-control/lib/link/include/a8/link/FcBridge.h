#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/defines.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class FcBridge {

public:
    static void bridge(int type, void *data, void *context) {
        FcApi *fcApi = Lang::cast<FcApi *>(context);
        switch (type) {
        case CommonMessageType::PING: {

            String *pData = Lang::cast<String *>(data);
            fcApi->ping(*pData);
        } break;
        case CommonMessageType::TEST: {

            fcApi->test();
        } break;
        }
    }
};

} // namespace a8::link
