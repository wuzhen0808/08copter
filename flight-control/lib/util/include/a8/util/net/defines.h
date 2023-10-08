#pragma once

#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"

namespace a8::util::net {
using bridge = void (*)(int, void *, void *);
using write = int (*)(Writer *, int, void *, Result &);        // writer,type,data,result
using read = int (*)(Reader *, int, bridge, void *, Result &); // reader,type,bridge,context,result.

namespace CodecFunc {

static int writeInt8_(Writer *writer, int data) {
    char ch = data & 0x00FF;
    writer->write(ch);
    return 1;
}

static int readInt8_(Reader *reader, int &data) {
    char ch;
    int ret = reader->read(ch);
    if (ret < 0) {
        return ret;
    }
    data = (ch & 0x00FF) << 0;
    return ret;
}

static int writeInt16_(Writer *writer, int iValue) {
    char ch1 = iValue >> 0 & 0x000000FF;
    char ch2 = iValue >> 8 & 0x000000FF;
    writer->write(ch1);
    writer->write(ch2);
    return 2;
}

static int readInt16_(Reader *reader, int &intV) {
    char buf[2];
    int ret = reader->read(buf, 2);
    if (ret < 0) {
        return ret;
    }

    intV = buf[0] & 0x000000FF;
    intV = intV + ((buf[1] & 0x000000FF) << 8);

    return ret;
}

static int writeString(Writer *writer, int type, void *data, Result &rst) {
    String *str = static_cast<String *>(data);
    int len = 0;
    int len2 = str->length();
    // write length
    int ret = writeInt16_(writer, len2);
    if (ret < 0) {
        return ret;
    }
    len += ret;
    // write text
    ret = writer->write(str->text(), str->len());
    if (ret < 0) {
        return ret;
    }
    len += ret;
    return len;
}

static int readString(Reader *reader, int type, bridge bridge, void *context, Result &rst) {

    // read length;
    int len;
    int ret = readInt16_(reader, len);
    if (ret < 0) {
        return ret;
    }
    // read text
    char buf[len];
    reader->read(buf, len);
    String *data = new String(buf, len);
    bridge(type, data, context);
    ret += len;
    return ret;
}
} // namespace CodecFunc
} // namespace a8::util::net
