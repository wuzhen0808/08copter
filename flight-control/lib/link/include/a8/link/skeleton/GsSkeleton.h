#pragma once
#include "a8/link/defines.h"
#include "a8/link/GsApi.h"
#include "a8/link/stub/GsStub.h"
#include "a8/link/defines.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class GsSkeleton : public GsApi {

public:
    static void handle(int type, void *data, void *context) {
        GsSkeleton *gss = static_cast<GsSkeleton *>(context);
        switch (type) {
        case CommonMessageType::PING:
            gss->ping(*static_cast<String *>(data));
            break;
        case CommonMessageType::LOG:
            gss->log(*static_cast<String *>(data));
            break;
        }
    }
    
    GsSkeleton() {
        
    }
};

} // namespace a8::net
