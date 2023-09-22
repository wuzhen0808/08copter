#include "a8/hal/native/NativeAttitudeSensor.h"
namespace a8::hal::native {
// TODO move the code into a separated project which should be called simulator.
// All the code finally should be same regard less the copter is real or virtual.
// The difference should be constrained into a configuration part.

NativeAttitudeSensor::NativeAttitudeSensor() : AttitudeSensor() {
}
NativeAttitudeSensor::~NativeAttitudeSensor() {
}
void NativeAttitudeSensor::update() {
}
float NativeAttitudeSensor::getRoll() {
    return .0f;
}
float NativeAttitudeSensor::getPitch() {
    return .0f;
}
float NativeAttitudeSensor::getYaw() {
    return .0f;
}

} // namespace a8::native