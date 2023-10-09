#pragma once
#include "a8/util.h"
using namespace a8::util;

namespace a8::link {

namespace CommonMessageType {

static const int PING = 1;
static const int LOG = 2;
static const int CMD = 3;
static const int LINE = 4;

}; // namespace CommonMessageType
namespace CopterMessageType {
static const int SENSORS_DATA = 101;

}

} // namespace a8::link