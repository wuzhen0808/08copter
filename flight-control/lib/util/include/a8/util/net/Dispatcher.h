#pragma once
#include "a8/util/net/defines.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {

class Dispatcher {

    FuncType::handle defaultHandle;

    Buffer<FuncType::handle> *handles;

public:
    Dispatcher(FuncType::handle handle) {
        this->defaultHandle = handle;
        this->handles = new Buffer<FuncType::handle>();
    }
    void setDefaultHandle(FuncType::handle handle) {
        this->defaultHandle = handle;
    }
    void addHandle(int type, FuncType::handle handle) {
        this->handles->set(type, defaultHandle, handle);
    }

    void dispatch(int type, void *data, void *context) {
        FuncType::handle handle = this->handles->get(type);
        if (handle == 0) {
            handle = this->defaultHandle;
        }
        handle(type, data, context);
    }
};

} // namespace a8::util::net
