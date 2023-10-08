#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::util::net {

class FunctionCodec : public Codec {
    FuncType::write writeF;
    FuncType::read readF;

public:
    FunctionCodec(FuncType::write write, FuncType::read read) : Codec() {
        this->writeF = write;
        this->readF = read;
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
    int read(Reader *reader, consume consume, void *context, Result &rst) override {
        return readF(reader, consume, context, rst);
        //
    }
};

} // namespace a8::util::net
