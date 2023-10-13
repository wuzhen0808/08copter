#pragma once
namespace a8::gs {
class EventTypes {
public:
    const static int AFTER_BRIDGE_CREATE_ = 1;
    const static int BEFORE_BRIDGE_FREE_ = 2;
    const static int BEFORE_QUIT = 3;
    const static int ON_SENSORS_DATA = 4;
};
} // namespace a8::gs