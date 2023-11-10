#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {
using namespace a8::util;


template <typename T>
class FunctionalCodec2 : public Codec {
    using writeF = int (*)(Writer *, T);

    using readF = int (*)(Reader *, T &);
    using defaultF = void (*)(T &);

    int type;
    writeF write_;
    readF read_;
    defaultF default_;

public:
    FunctionalCodec2(int type, writeF writeF, readF readF, defaultF defaultF) : Codec() {
        this->type = type;
        this->write_ = writeF;
        this->read_ = readF;
        this->default_ = defaultF;
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
        if (data == 0) {
            T defaultV;
            if (default_) {
                default_(defaultV);
            }
            ret += write_(writer, defaultV);
        } else {
            T *dataPtr = Lang::cast<T *>(data);
            ret += write_(writer, *dataPtr);
        }
        return ret;
    }

    /**
     * @override
     */
    int read(Reader *reader, bridge bridgeF, void *context, Result &rst) override {
        T data;
        int ret = read_(reader, data);
        if (ret < 0) {
            return ret;
        }
        ret = bridgeF(type, &data, context, rst);
        if (ret < 0) {
            return ret;
        }
        return ret;
        //
    }
};

} // namespace a8::util::net
