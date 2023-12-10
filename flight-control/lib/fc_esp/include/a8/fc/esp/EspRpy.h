#pragma once
#include "a8/fc.h"
// lang-c-format off
#include <Arduino.h>
#include <MPU9250.h>
// lang-c-format on

namespace a8::fc::esp {
class EspRpy : public Rpy, public FlyWeight {
    MPU9250 *mpu;

public:
    EspRpy(MPU9250 *mpu, LoggerFactory *logFac) : FlyWeight(logFac, "EspRpy") {
        this->mpu = mpu;
    }
    bool update() override {
        return mpu->update();
    }
    float getRoll() override {
        return mpu->getRoll();
    }
    float getPitch() override {
        return mpu->getPitch();
    }
    float getYaw() override {
        return mpu->getYaw();
    }
    int checkIfReady(Result &res) override {

        int ret = this->checkIfStable(res);
        if (ret < 0) {
            return ret;
        }
        ret = this->checkIfBalance(res);
        if (ret < 0) {
            return ret;
        }

        return ret;
    }
    int checkIfStable(Result &res) override {
        log("checking rpy if stable ...");
        float rollAvg = 0;
        float rollMin;
        float rollMax;

        // What's wrong with mpu9250? if i = 10 and delay 100, the mpu will read roll,pitch yaw all as a zero().

        for (int i = 0; i < 100; i++) { //
            this->update();
            float roll = -this->getRoll();
            float pitch = -this->getPitch();
            log(String() << "roll:" << roll << ",pitch:" << pitch);
            rollAvg = (rollAvg * i + roll) / (i + 1);
            if (i == 0) {
                rollMax = rollMin = roll;
            } else {
                if (roll < rollMin) {
                    rollMin = roll;
                }
                if (roll > rollMax) {
                    rollMax = roll;
                }
            }
            delay(10);
        }

        if (Math::abs<float>(rollAvg - rollMin) > 0.5f || Math::abs<float>(rollMax - rollAvg) > 0.5f) {
            res << "rpy is not stable,rollAvg:" << rollAvg << ",rollMin:" << rollMin << ",rollMax" << rollMax;
            return -1;
        }

        log("done of rpy stable check.");
        return 1;
    }

    int checkIfBalance(Result &res) {
        float roll = this->getRoll();
        float pitch = this->getPitch();
        log(String() << "check rpy if balance, roll:" << roll << ",pitch:" << pitch);

        if (Math::abs<float>(roll) > 1) {
            res << "rpy is not balance, roll(" << roll << ") value showing the attitude of the drone is not in a balance position, please adjust and try again.";
            return -1;
        }
        if (Math::abs<float>(pitch > 1)) {
            res << "rpy is not balance, pitch(" << pitch << ") value showing the attitude of the drone is not in a balance position, please adjust and try again.";
            return -2;
        }
        log("done of rpy balance check.");
        return 1;
    }
};

} // namespace a8::fc::esp
