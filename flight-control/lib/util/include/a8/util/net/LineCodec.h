#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"

using namespace a8::util;
using namespace a8::util::net;
#define SIMPLE_CODEC_ (0)

namespace a8::util::net {

class LineCodec : public Codec {
    char separator = 0;

public:
    LineCodec(char sep) : Codec() {
        this->separator = sep;
    }
    void *startCodec() {
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
    int read(Reader *reader, consume consume, void *context, Result &rst) override {

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
        if (ret >= 0) {
            consume(0, str, context);
        }
        delete str;
        return ret;
    }
};

} // namespace a8::util::net
