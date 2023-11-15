#pragma once
#include "a8/hal/rf24/Rf24ConnectRequest.h"
#include "a8/hal/rf24/Rf24ConnectResponse.h"
#include "a8/hal/rf24/Rf24UserData.h"
#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;

class Rf24NodeData {
public:
    static const int TYPE_Rf24ConnectRequest = 1;
    static const int TYPE_Rf24ConnectResponse = 2;
    static const int TYPE_Rf24UserData = 3;

    static int read(Reader *reader, Rf24NodeData &data) {
        int len = 0;
        int ret = CodecUtil::readInt<int>(reader, data.type);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        switch (data.type) {
        case TYPE_Rf24ConnectRequest:
            data.connectionRequest = new Rf24ConnectRequest();
            ret = Rf24ConnectRequest::read(reader, *data.connectionRequest);
            break;
        case TYPE_Rf24ConnectResponse:
            data.connectionResponse = new Rf24ConnectResponse();
            ret = Rf24ConnectResponse::read(reader, *data.connectionResponse);
            break;
        case TYPE_Rf24UserData:
            data.userData = new Rf24UserData();
            ret = Rf24UserData::read(reader, *data.userData);
            break;
        default:
            ret = -1;
        }
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }
    static int write(Writer *writer, Rf24NodeData &data) {
        int len = 0;
        int ret = CodecUtil::writeInt<int>(writer, data.type);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        switch (data.type) {
        case TYPE_Rf24ConnectRequest:
            ret = Rf24ConnectRequest::write(writer, *data.connectionRequest);
            break;
        case TYPE_Rf24ConnectResponse:
            ret = Rf24ConnectResponse::write(writer, *data.connectionResponse);
            break;

        case TYPE_Rf24UserData:
            ret = Rf24UserData::write(writer, *data.userData);
            break;
        default:
            ret = -1;
        }
        if (ret < 0) {
            return ret;
        }

        return len;
    }

    int type;
    Rf24ConnectRequest *connectionRequest;
    Rf24ConnectResponse *connectionResponse;
    Rf24UserData *userData;

    Rf24NodeData() {
        this->connectionRequest = 0;
        this->connectionResponse = 0;
        this->userData = 0;
        this->type = 0;
    }
    ~Rf24NodeData() {
        if (this->connectionRequest != 0) {
            delete this->connectionRequest;
        }
        if (this->userData != 0) {
            delete this->userData;
        }
    }
};

} // namespace a8::hal::rf24