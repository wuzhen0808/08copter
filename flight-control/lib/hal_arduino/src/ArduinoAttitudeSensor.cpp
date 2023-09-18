#include "a8/arduino/ArduinoAttitudeSensor.h"
namespace a8::arduino {
// TODO move the code into a separated project which should be called simulator.
// All the code finally should be same regard less the copter is real or virtual.
// The difference should be constrained into a configuration part.

ArduinoAttitudeSensor::ArduinoAttitudeSensor() : AttitudeSensor() {
}
ArduinoAttitudeSensor::~ArduinoAttitudeSensor() {
}
void ArduinoAttitudeSensor::update() {
}
float ArduinoAttitudeSensor::getRoll() {
    return .0f;
}
float ArduinoAttitudeSensor::getPitch() {
    return .0f;
}
float ArduinoAttitudeSensor::getYaw() {
    return .0f;
}

} // namespace a8::native