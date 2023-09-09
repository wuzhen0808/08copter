#ifndef a8_ATTITUDE_SENSOR__
#define a8_ATTITUDE_SENSOR__
#include "a8/core/Component.hpp"

namespace a8 {
namespace core {

class AttitudeSensor : public Component {
public:
    AttitudeSensor(Copter *copter);
    ~AttitudeSensor();
    virtual void update() = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
};
} // namespace core
} // namespace a8
#endif