#ifndef a8_ATTITUDE_SENSOR__
#define a8_ATTITUDE_SENSOR__
#include "a8/util/Component.h"
using namespace a8::util;
namespace a8::core {

class AttitudeSensor : public Component {
public:
    AttitudeSensor();
    ~AttitudeSensor();
    virtual void update() = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
};
} // namespace a8::core
#endif