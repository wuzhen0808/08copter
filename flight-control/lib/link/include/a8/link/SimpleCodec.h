#pragma once
#include "a8/util/net/Codec.h"
#include "a8/util.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

class SimpleCodec : public Codec {

    int writeInt8(Writer *writer, int iValue) {
        char ch = iValue & 0x000000FF;
        writer->write(&ch, 1);
        return 1;
    }

    int writeInt16(Writer *writer, int iValue) {
        char ch1 = iValue >> 0 & 0x000000FF;
        char ch2 = iValue >> 8 & 0x000000FF;
        writer->write(&ch1);
        writer->write(&ch2);
        return 2;
    }
    int readInt8(Reader *reader, char *buf, int len) {
        reader->read(buf, 1);
        int i = 0;
        i = (i + buf[0] & 0x000000FF) << 0;
        return i;
    }

    int readInt16(Reader *reader, char *buf, int len) {
        reader->read(buf, 2);
        int i = 0;
        i = (i + buf[0] & 0x000000FF) << 0;
        i = (i + buf[1] & 0x000000FF) << 8;
        return i;
    }

public:
    SimpleCodec() {
    }


    /** 
     * @override
    */
    int write(Writer *writer, int type, void *data) override {

        writeInt8(writer, 0); // ver
        writeInt8(writer, type);
        EnDe *codec = codecs.get(type);
        int len = 0;
        char *buf = 0;
        if (codec != 0) {
            len = codec->encode(data, buf);
        }
        if (len >= 0) {
            writeInt16(writer, len);
            writer->write(buf, len);
        } else {
            writeInt16(writer, 0); // write a negative len?
        }
        //
        return len;
    }

    /** 
     * @override
    */
    int read(Reader *reader, int &type, void *&data) override {
        int len0 = 2;
        char buf[len0];

        int ver = readInt8(reader, buf, len0);
        int type_ = readInt8(reader, buf, len0);
        int len2 = readInt16(reader, buf, len0);
        char buf2[len2];
        int rst = reader->read(buf2, len2);
        if (rst < len2) {
            // some error or the connection broken.
            data = 0; // write ref back
            return -1;
        }
        EnDe *codec = codecs.get(type);
        if (codec == 0) {
            data = 0;  // write ref back
            return -2; // failed to decode buffer.
        }
        type = type_;
        data = (codec->decode)(buf2, len2); // write ref back

        return rst;
        //
    }
};

} // namespace  a8::net
