#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include <math.h>

namespace a8::util::net {
using bridge = void (*)(int, void *, void *);
// using write = int (*)(Writer *, void *, Result &); // writer,data,result
// using read = int (*)(Reader *, void *&, Result &); // reader,data,result.
// using free = void (*)(void *);                // free data pointer.

} // namespace a8::util::net
