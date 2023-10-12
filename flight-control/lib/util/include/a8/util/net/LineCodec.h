#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"

using namespace a8::util;
using namespace a8::util::net;
#define SIMPLE_CODEC_ (0)

namespace a8::util::net {

class LineCodec : public Codec {
    char separator;
    int type;

public:
    LineCodec(int type, char sep) : Codec() {
        this->type = type;
        this->separator = sep;
    }
    int getHeaderLength() override {
        return 0;
    }
    /**
     * @override
     */
    int write(Writer *writer, int type, void *data, Result &rst) override {
        String *str = static_cast<String *>(data);
        int len = 0;
        int ret = writer->write(*str);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = writer->write(separator);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        //
        return len;
    }

    /**
     * @override
     */
    int read(Reader *reader, bridge bridgeF, void *context, Result &rst) override {

        String *str = new String();
        char ch;
        int ret = -1;
        while (true) {
            ret = reader->read(ch);

            if (ret <= 0) {
                break;
            }
            if (ch == separator) {
                break;
            }
            str->append(ch);
        }
        if (ret < 0) {
            return ret;
        }
        ret = bridgeF(type, str, context, rst);
        Lang::free(str);
        if (ret < 0) {
            return ret;
        }
        return ret;
    }
};

} // namespace a8::util::net
