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
            String *pData = static_cast<String *>(data);
            fcs->ping(*pData);
            delete pData;
            break;
        }
        delete fcs;
    }

public:
    FcSkeleton() {
    }
};

} // namespace a8::link
