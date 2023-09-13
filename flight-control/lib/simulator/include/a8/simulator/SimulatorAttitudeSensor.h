#ifndef simulator_AttitudeSensor__
#define simulator_AttitudeSensor__
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Copter.h"

namespace a8 {
namespace simulator {
using namespace a8::core;
class SimulatorAttitudeSensor : public AttitudeSensor {
public:
    SimulatorAttitudeSensor(Copter *copter);
    ~SimulatorAttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();
};
} // namespace simulator
} // namespace a8
#endif