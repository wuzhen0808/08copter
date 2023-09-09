#ifndef simulator_AttitudeSensor__
#define simulator_AttitudeSensor__
#include "a8/AttitudeSensor.hpp"
#include "a8/Copter.hpp"

namespace a8 {

class SimulatorAttitudeSensor : public AttitudeSensor {
public:
    SimulatorAttitudeSensor(Copter *copter);
    ~SimulatorAttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();
};

} // namespace a8
#endif