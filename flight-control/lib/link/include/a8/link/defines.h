#pragma once
#include "a8/util.h"
namespace a8::link {
using namespace a8::util;


namespace CommonMessageType {

static const int PING = 1;
static const int LOG = 2;
static const int CMD = 3;
static const int LINE = 4;
static const int TEST = 5;

}; // namespace CommonMessageType
namespace CopterMessageType {
static const int SENSORS_DATA = 21;
static const int THROTTLE = 22;
static const int ROLL = 23;
static const int PITCH = 24;
static const int YAW = 25;



}

} // namespace a8::link