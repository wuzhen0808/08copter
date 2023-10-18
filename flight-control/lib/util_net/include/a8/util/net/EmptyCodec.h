#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/defines.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::util::net {

class EmptyCodec : public Codec {
    int type;

public:
    EmptyCodec(int type) : Codec() {
        this->type = type;
    }

    int getHeaderLength() override {
        return 0;
    }
    /**
     * @override
     */
    int write(Writer *writer, int type, void *data, Result &rst) override {
        return 0;
    }

    /**
     * @override
     */
    int read(Reader *reader, bridge bridgeF, void *context, Result &rst) override {
        return bridgeF(type, 0, context, rst);
    }
};

} // namespace a8::util::net
