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
        FcApi *fcs = static_cast<FcApi *>(context);
        switch (type) {
        case CommonMessageType::PING:
            String *pData = static_cast<String *>(data);
            fcs->ping(*pData);
            break;
        }
        delete fcs;
    }
};

} // namespace a8::link
