#pragma once
#include "a8/link/FcsApi.h"
#include "a8/link/defines.h"
#include "a8/link/stub/FcsStub.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class FcsSkeleton : public FcsApi {

public:
    static void handle(int type, void *data, void *context) {
        FcsSkeleton *fcs = static_cast<FcsSkeleton *>(context);
        switch (type) {
        case CommonMessageType::PING:
            fcs->ping(*static_cast<String *>(data));
            break;

        }
    }

public:
    FcsSkeleton() {
    }
    void ping(String msg) override {
    }
};

} // namespace a8::link
