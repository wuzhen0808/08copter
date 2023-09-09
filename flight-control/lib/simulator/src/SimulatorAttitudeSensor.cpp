#include "a8/SimulatorAttitudeSensor.hpp"
namespace a8 {

SimulatorAttitudeSensor::SimulatorAttitudeSensor(Copter *copter) : AttitudeSensor(copter) {
}
SimulatorAttitudeSensor::~SimulatorAttitudeSensor() {
}
void SimulatorAttitudeSensor::update() {
}
float SimulatorAttitudeSensor::getRoll() {
    return .0f;
}
float SimulatorAttitudeSensor::getPitch() {
    return .0f;
}
float SimulatorAttitudeSensor::getYaw() {
    return .0f;
}

} // namespace a8