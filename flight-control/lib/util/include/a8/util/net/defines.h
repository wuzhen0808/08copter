#pragma once

#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"

namespace a8::util::net {
namespace FuncType {

typedef int (*encode)(void *, char *&);

typedef void *(*decode)(char *, int len); //

typedef void (*handle)(int type, void *, void *);
} // namespace FuncType

} // namespace a8::util::net
