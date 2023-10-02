#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/defines.h"
#include "a8/link/stub/FcStub.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class FcSkeleton : public FcApi {

public:
    static void handle(int type, void *data, void *context) {
        FcSkeleton *fcs = static_cast<FcSkeleton *>(context);
        switch (type) {
        case CommonMessageType::PING:
            fcs->ping(*static_cast<String *>(data));
            break;

        }
    }

public:
    FcSkeleton() {
    }
    
};

} // namespace a8::link
