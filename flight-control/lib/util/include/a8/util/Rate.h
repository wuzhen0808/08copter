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
        return hz * 1.0e3f;
    }

    float kHz() const {
        return hz * 1.0e-3f;
    }
    /**
     * circle per second
     */
    float cps() const {
        return hz;
    }
    /**
     * rotation per minute
     */
    float rpm() const {
        return 60.0f * hz;
    }
    float radPs() const {
        return 2.0f * M_PI_ * hz;
    }

    float degPs() const {
        return 360.0f * hz;
    }
    /**
     * seconds per rotation
     */
    float sec() const {
        if (isZero()) {
            return -1;
        }
        return 1.0f / hz;
    }
    /**
     * millisec per round
     */
    float ms() const {
        if (isZero()) {
            return -1;
        }
        return 1.0e3 * sec();
    }
    /**
     * time per round
     */
    float us() const {
        if (isZero()) {
            return -1;
        }
        return 1.0e6 * sec();
    }
    /**
     * time per round
     */
    float ns() const {
        if (isZero()) {
            return -1;
        }
        return 1.0e9 * sec();
    }

    /**
     * Zero means no frequency.
     * Do not schedule zero rate.
     */
    bool isZero() const {
        return Math::isNear(hz, 0.0f);
    }
    /*
     * RUN means it's a runner to be called in separate thread.
     */
    bool isRun() const {
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
    friend String &operator<<(String &str, const Rate &rate) {
        return str << rate.hz;
    }
};

} // namespace util

} // namespace a8
