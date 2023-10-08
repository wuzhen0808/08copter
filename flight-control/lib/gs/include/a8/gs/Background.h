#pragma once
#include "a8/gs/GsSkeleton.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
namespace a8::gs {

using bridgeHandle = void (*)(Bridge *, int, void *); // bridge,isCreate,context.

class Background : public FlyWeight {
    class Entry {
    public:
        bridgeHandle handle;
        void *context;
        void doHandle(Bridge *bridge, int isCreate) {
            handle(bridge, isCreate, context);
        }
    };
    Links *network;
    FcApi *fcApi;
    Address *gsAddress;
    Buffer<Entry> entries;

public:
    Background(Links *network, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->network = network;
        this->gsAddress = network->gsAddress();
    }

    void add(void *context, bridgeHandle bridgeHandle) {
        Entry entry;
        entry.handle = bridgeHandle;
        entry.context = context;
        entries.append(entry);
    }

    int open(Result &rst) {
        // waiting fcs to connect
        int ret = this->gsAddress->bind(rst);
        if (ret < 0) {
            return ret;
        }
        ret = gsAddress->listen(rst);
        if (ret < 0) {
            return ret;
        }
        log("listening connect in on port of gs.");
        return ret;
    }
    void close() {
        this->gsAddress->close();
    }

    int run(TickingContext *ticking) {
        log("GS net accepter running...");
        Bridge *bridge = 0;
        while (true) {
            Bridge *bridge2 = 0;
            Result rst;
            int ret = gsAddress->accept(
                bridge2,
                new GsSkeleton(this->loggerFactory), GsSkeleton::release, //                                       //
                rst                                                       //
            );

            if (ret < 0) {
                log(rst.errorMessage);
                break;
            }

            bridge2->createStub<FcApi>(FcStub::create, FcStub::release);

            if (bridge != 0) {
                bridge->close();
                for (int j = 0; j < entries.len(); j++) {
                    entries[j].doHandle(bridge, 0);
                }
                delete bridge;
            }

            for (int j = 0; j < entries.len(); j++) {
                entries[j].doHandle(bridge2, 1);
            }
            bridge = bridge2;
            log("A new GS client connected in.");
        }

        if (bridge != 0) {
            bridge->close();
            delete bridge;
        }
        log(String() << "Warning: GS main loop exit.");
        return 1;
    }
};

} // namespace a8::gs