#pragma once
#include "a8/util/comp.h"
#include "a8/util/Vector3f.h"

using namespace a8::util;
using namespace a8::util::comp;

namespace a8::fc {

class AttitudeSensor : public Component {
public:
    AttitudeSensor();
    ~AttitudeSensor();
    
    virtual Vector3f getAngVel() = 0;
    virtual Vector3f getAccVel() = 0;
    virtual double getAlt() = 0;
    virtual double getAgl() = 0;
};
} // namespace a8::fc