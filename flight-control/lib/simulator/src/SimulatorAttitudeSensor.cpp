#include "a8/simulator/SimulatorAttitudeSensor.hpp"
namespace a8 {
namespace simulator {

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

} // namespace simulator
} // namespace a8