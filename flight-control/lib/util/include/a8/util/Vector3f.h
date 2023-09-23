#pragma once
namespace a8::util {

class Vector3f {

public:
    float a;
    float b;
    float c;
    Vector3f() {
        a = b = c = 0;
    }
    Vector3f(float a, float b, float c) {
        this->a = a;
        this->b = b;
        this->c = c;
    }
    Vector3f &operator=(float x) {
        a = b = c = x;
        return *this;
    }
    Vector3f &operator*(float x) {
        this->a *= x;
        this->b *= x;
        this->c *= x;
        return *this;
    }
};

} // namespace a8::util
