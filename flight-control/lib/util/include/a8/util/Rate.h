#pragma once
#include "a8/util/Math.h"
#include "a8/util/String.h"
namespace a8 {
namespace util {

class Rate {
    float hz;

public:
    static Rate RUN;

    Rate() {
        this->hz = 0.0f;
    }
    Rate(float hz) {
        this->hz = hz;
    }
    float Hz() const {
        return hz;
    }

    float mHz() const {
        return hz * 1000.0f;
    }

    float kHz() const {
        return hz * 1000;
    }

    float cps() const {
        return 1.0f / hz;
    }

    float rpm() const {
        return 60.0f / hz;
    }
    float radPs() const {
        return 2.0f * M_PI / hz;
    }

    float degPs() const {
        return 360.0f / hz;
    }

    float sec() const {
        return 1.0f / hz;
    }

    float ms() const {
        return 1.0e3 / hz;
    }

    float us() const {
        return 1.0e6 / hz;
    }

    float ns() const {
        return 1.0e9 / hz;
    }

    /**
     * Zero means no frequency.
     * Do not schedule zero rate.
     */
    bool isZero() {
        return Math::isNear(hz, 0.0f);
    }
    /*
     * RUN means it's a runner to be called in separate thread.
     */
    bool isRun() {
        return Math::isNear(hz, -1.0f);
    }

    Rate &operator=(int hz) {
        this->hz = hz;
        return *this;
    }
    Rate &operator=(const float hz) {
        this->hz = hz;
        return *this;
    }

    //
    friend String &operator<<(String &str, Rate &rate) {
        return str << rate.hz;
    }
};

} // namespace util

} // namespace a8
