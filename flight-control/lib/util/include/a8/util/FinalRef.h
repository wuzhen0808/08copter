#pragma once

#define M_PI (3.141592653589793238462643383279502884)
#define FEET_TO_METERS (0.3048)
#define RAD_TO_DEG (180.0f / M_PI)
#define ZERO_NEAR (0.000001)

namespace a8::util {
template <typename T>
class FinalRef {

public:
    T *ptr;
    FinalRef() {
        ptr = 0;
    }
    FinalRef(T *ptr) {
        this->ptr = ptr;
    }

    FinalRef(FinalRef<T> &ref) {
        this->ptr = ref->ptr;
        ref->ptr = 0;
    }

    ~FinalRef() {
        if (ptr != 0) {
            delete ptr;
        }
    }

    void operator=(T *ptr) {
        if (this->ptr != 0) {
            delete this->ptr;
        }
        this->ptr = ptr;
    }

    friend bool operator==(FinalRef<T> &ref, int zero) {
        if (zero == 0) {
            return ref.ptr == 0;
        }
        return false;
    }
};
} // namespace a8::util