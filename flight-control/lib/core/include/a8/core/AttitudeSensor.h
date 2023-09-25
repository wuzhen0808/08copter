#pragma once
#include "a8/util/Component.h"
#include "a8/util/Vector3f.h"

using namespace a8::util;

namespace a8::core {

class AttitudeSensor : public Component {
public:
    AttitudeSensor();
    ~AttitudeSensor();
    
    virtual Vector3f getAngVel() = 0;
    virtual Vector3f getAccVel() = 0;
    virtual double getAlt() = 0;
};
} // namespace a8::core