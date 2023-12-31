#pragma once
#include "a8/util/net.h"

namespace a8::util::rf24 {
using namespace a8::util;

class Rf24NetData {

    void init(int type, int node1, int port1, int node2, int port2, int responseCode, const char *buf, int len) {
        this->type = type;
        this->node1 = node1;
        this->port1 = port1;
        this->node2 = node2;
        this->port2 = port2;
        this->responseCode = responseCode;
        this->buffer = new Buffer<char>(buf, len);
    }
    Rf24NetData() {
        this->init(0, 0, 0, 0, 0, 0, 0, 0);
    }

public:
    //
    static const int TYPE_EMPTY = 0;
    static const int TYPE_CONNECT_REQUEST = 1;
    static const int TYPE_CONNECT_RESPONSE = 2;
    static const int TYPE_USER_DATA = 3;
    static const int TYPE_USER_DATA_RESPONSE = 4;

    static int read(char *buf, int len, Rf24NetData *&data) {
        WriterReaderBuffer wrb;
        (static_cast<Writer *>(&wrb))->write(buf, len);
        return read(&wrb, data);
    }

    static int read(Reader *reader, Rf24NetData *&data) {
        Rf24NetData *data2 = new Rf24NetData();
        int ret = read(reader, *data2);
        if (ret < 0) {
            delete data2;
            return ret;
        }
        data = data2;
        return ret;
    }
    static int read(Reader *reader, Rf24NetData &data) {
        int len = 0;
        int ret = CodecUtil::readInt<int>(reader, data.type);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::readInt<int>(reader, data.node1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::readInt<int>(reader, data.port1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::readInt<int>(reader, data.node2);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        // port2
        ret = CodecUtil::readInt<int>(reader, data.port2);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        //
        ret = CodecUtil::readInt<int>(reader, data.responseCode);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        //

        int size = 0;
        ret = CodecUtil::readInt<int>(reader, size);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        if (size < 0) {
            return -1; // error of data.
        }

        if (size > 0) {
            char buf[size];
            Result res;
            ret = reader->fullRead(buf, size, res);
            if (ret < 0) {
                return -1; // error of read.
            }
            if (ret < size) {
                return -2; // error of data.
            }
            data.buffer->append(buf, size);
        }

        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }
    static int write(Writer *writer, Rf24NetData &data) {
        int len = 0;
        int ret = CodecUtil::writeInt<int>(writer, data.type);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::writeInt<int>(writer, data.node1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        //
        ret = CodecUtil::writeInt<int>(writer, data.port1);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        // node2
        ret = CodecUtil::writeInt<int>(writer, data.node2);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        // port2
        ret = CodecUtil::writeInt<int>(writer, data.port2);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        // responseCode
        ret = CodecUtil::writeInt<int>(writer, data.responseCode);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        // write size of payload.
        int size = data.buffer->len();
        ret = CodecUtil::writeInt<int>(writer, size);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        // write payload
        if (size > 0) {
            ret = writer->write(data.buffer->buffer(), data.buffer->len());
            if (ret < 0) {
                return ret;
            }
            len += ret;
        }
        if (size == 0) {
            // no data payload.
        }

        return len;
    }
    static Rf24NetData *copy(Rf24NetData *uData, Rf24NetData &uData2) {
        WriterReaderBuffer buf;
        Rf24NetData::write(&buf, *uData);
        Rf24NetData::read(&buf, uData2);
        return &uData2;
    }

    friend String &operator<<(String &str, const Rf24NetData *data) {
        if (data == 0) {
            return str << "0";
        }
        return operator<<(str, *data);
    }

    friend String &operator<<(String &str, const Rf24NetData &data) {

        return str << "Rf24NetData(" << data.port1 << "@" << data.node1 << "==>" << data.port2 << "@" << data.node2 << "," << data.type << "," << data.responseCode << "," << StringUtil::toHexString(*data.buffer) << ")";
    }

    int type;
    int node1;
    int port1;
    int node2;
    int port2;
    int responseCode;
    Buffer<char> *buffer; // payload.

    Rf24NetData(int type, int node1, int port1, int node2, int port2, int responseCode) {
        this->init(type, node1, port1, node2, port2, responseCode, 0, 0);
    }

    Rf24NetData(int type, int node1, int port1, int node2, int port2) {
        this->init(type, node1, port1, node2, port2, 0, 0, 0);
    }
    Rf24NetData(int type, int node1, int port1, int node2, int port2, int responseCode, const Buffer<char> *buf) {
        this->init(type, node1, port1, node2, port2, responseCode, buf->buffer(), buf->len());
    }

    Rf24NetData(int type, int node1, int port1, int node2, int port2, int responseCode, const char *buf, int len) {
        this->init(type, node1, port1, node2, port2, responseCode, buf, len);
    }

    Rf24NetData(Rf24NetData *data) {
        this->init(data->type, data->node1, data->port1, data->node2, data->port2, data->responseCode, data->buffer->buffer(), data->buffer->len());
    }
    ~Rf24NetData() {
        delete this->buffer;
    }
    int getResponseType() {
        int type2 = 0;
        switch (this->type) {
        case TYPE_CONNECT_REQUEST:
            type2 = TYPE_CONNECT_RESPONSE;
            break;
        case TYPE_USER_DATA:
            type2 = TYPE_USER_DATA_RESPONSE;
            break;
        }
        return type2;
    }
};

} // namespace a8::util::rf24