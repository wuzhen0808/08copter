#pragma once
namespace a8::fc {

class Servo {
protected:
    int id;

public:
    Servo(int id) {
        this->id = id;
    }
    int getId() {
        return this->id;
    }

    /**
     * Velocity here a normalized scope: (-1, 1)
     */
    virtual void setThrottleNorm(float norm) = 0;
};

} // namespace a8::hal