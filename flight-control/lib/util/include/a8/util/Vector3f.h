#pragma once
namespace a8::util {

class Vector3f {

public:
    float x;
    float y;
    float z;
    Vector3f() {
        x = y = z = 0;
    }
    Vector3f(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3f &operator=(float v) {
        x = y = z = v;
        return *this;
    }
    Vector3f &operator*(float f) {
        this->x *= f;
        this->y *= f;
        this->z *= f;
        return *this;
    }
};

} // namespace a8::util
