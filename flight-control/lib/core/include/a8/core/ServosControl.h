#ifndef SERVOS_CONTROL__
#define SERVOS_CONTROL__
#include "a8/core/Component.h"
#include <stdlib.h>
namespace a8 {
namespace core {
class ServosControl : public Component {

public:
    ServosControl(void *copter);
    ~ServosControl();
    virtual void attachAll() = 0;
    virtual void setVelocity(int servoId, float velocity) =0;
    virtual void setVelocities(int id1, float vel1,int id2, float vel2,int id3, float vel3,int id4, float vel4) =0;
    
};
} // namespace core
} // namespace a8
#endif