#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {

class Codec {  

public:
    Codec() {
    }

    virtual int write(Writer *writer, int type, void *data, Result &rst) = 0;

    virtual int read(Reader *reader, int &type, void *&data, Result &rst) = 0;
};

} // namespace a8::util::net
