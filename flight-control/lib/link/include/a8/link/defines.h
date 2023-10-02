#pragma once
#include "a8/util.h"
using namespace a8::util;

namespace a8::link {

namespace CommonMessageType {

static const int PING = 1;
static const int LOG = 2;
}; // namespace MessageType


namespace Functions {
static int encodeString(void *string, char *&buf) {
    String *str = static_cast<String *>(string);
    int len;
    int cap = 0;
    Lang::appendStr(buf, len, cap, 128, str->getText(), 0, str->getLength());
    return len;
}

static void *decodeString(char *buf, int len) {
    String *str = new String(buf, len);
    return str;
}
}; // namespace Functions

} // namespace a8::net