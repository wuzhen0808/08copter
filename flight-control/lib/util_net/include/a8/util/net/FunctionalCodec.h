#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {
using namespace a8::util;


template <typename T>
class FunctionalCodec : public Codec {
    using writeF = int (*)(Writer *, T *);
    using readF = int (*)(Reader *, T *&);
    using freeF = void (*)(T *);

    int type;
    writeF write_;
    readF read_;
    freeF free_;

public:
    FunctionalCodec(int type, writeF writeF, readF readF) : Codec() {
        this->type = type;
        this->write_ = writeF;
        this->read_ = readF;
        this->free_ = Lang::free<T>;
    }
    int getHeaderLength() override {
        return 0;
    }
    /**
     * @override
     */
    int write(Writer *writer, int type, void *data, Result &rst) override {
        int ret = 0;
        if (this->type != type) {
            return -1;
        }
        ret += write_(writer, Lang::cast<T *>(data));
        return ret;
    }

    /**
     * @override
     */
    int read(Reader *reader, bridge bridgeF, void *context, Result &rst) override {
        T * dataPtr = 0;
        int ret = read_(reader, dataPtr);
        if (ret < 0) {            
            return ret;
        }
        bridgeF(type, dataPtr, context);
        free_(dataPtr);
        return ret;
        //
    }
};

} // namespace a8::util::net
