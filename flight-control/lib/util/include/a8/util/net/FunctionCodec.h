#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/defines.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::util::net {

class FunctionCodec : public Codec {
    a8::util::net::write writeF;
    a8::util::net::read readF;

public:
    FunctionCodec(a8::util::net::write writeF, a8::util::net::read readF) : Codec() {
        this->writeF = writeF;
        this->readF = readF;
    }

    /**
     * @override
     */
    int write(Writer *writer, int type, void *data, Result &rst) override {
        return writeF(writer, type, data, rst);
    }

    /**
     * @override
     */
    int read(Reader *reader, int type, bridge bridgeF, void *context, Result &rst) override {
        return readF(reader, type, bridgeF, context, rst);
        //
    }
};

} // namespace a8::util::net
