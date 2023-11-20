#pragma once
#include "a8/hal/rf24/Rf24NetData.h"

namespace a8::hal::rf24 {
class Rf24NetRequest {

public:
    friend String &operator<<(String &str, const Rf24NetRequest *req) {
        if (req == 0) {
            return str << "0";
        }
        return operator<<(str, *req);
    }

    friend String &operator<<(String &str, const Rf24NetRequest &req) {
        return str << "Rf24NetRequest(" << req.data << "," << req.port << "," << req.consumeLater << "," << req.responseType << ")";
    }

    Rf24NetData *data;
    bool consumeLater = false;
    int responseType;
    int port;
    int responseCode = 0;
    void (*responseSender)(Rf24NetRequest* req, int port1);
    Rf24NetRequest(Rf24NetData *data) {
        this->data = data;
        this->responseType = data->getResponseType();
        this->port = data->port2;
        this->responseSender = 0;
    }
    
    ~Rf24NetRequest() {
       delete this->data;
    }
};
} // namespace a8::hal::rf24