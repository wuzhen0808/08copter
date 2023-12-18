#pragma once
#include "a8/fc.h"
// lang-c-format off
#include <Arduino.h>
#include <MPU9250.h>
// lang-c-format on

namespace a8::fc::esp {
class EspRpy : public Rpy, public FlyWeight {
    MPU9250 *mpu;
    bool setupOk_;
    int continueUpdateFailed = 0;
    bool stableChecked_ = false;
    int setupMpu9250() {
        mpu->setup(0x68);
        mpu->selectFilter(QuatFilterSel::MADGWICK);
        mpu->setFilterIterations(1);
        log("success of setupMpu9250.");
        return 1;
    }

public:
    EspRpy(LoggerFactory *logFac) : FlyWeight(logFac, "EspRpy") {
        this->mpu = new MPU9250();
    }

    void setup() override {
        A8_LOG_DEBUG(logger, "setup");
        int ret = this->setupMpu9250();
        if (ret > 0) {
            this->setupOk_ = true;
        } else {
            this->setupOk_ = false;
        }
    }
    void tick() override {
        if (!this->setupOk_) {
            A8_LOG_DEBUG(logger, "missing setup? or you need setup again.");
            return; //
        }
        if (this->stableChecked_) {
            return;
        }

        if (this->continueUpdateFailed > 1000) {
            // do not check, mpu totally not available.
            // TODO set a alarm.
            // TODO stop timer
            return;
        }

        Result res;
        int ret = this->checkIfStable(false, res);
    }

    bool update() override {

        A8_LOG_DEBUG(logger, ">>EspRpy::update.");
        bool ret = mpu->update();
        A8_LOG_DEBUG(logger, String() << "<<EspRpy::update,ret:" << ret);
        if (ret) {
            this->continueUpdateFailed = 0;
        } else {
            this->continueUpdateFailed++;
        }
        return ret;
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

        int ret = this->checkIfStable(false, res);
        if (ret < 0) {
            return ret;
        }
        ret = this->checkIfBalance(res);
        if (ret < 0) {
            return ret;
        }

        return ret;
    }
    bool isStable() override{
        return this->stableChecked_;
    }
    bool isBalance() override{
        Result res;
        return this->checkIfBalance(res) > 0;
    }
    int checkIfStable(Result &res) override {
        return checkIfStable(false, res);
    }

    int checkIfStable(bool background, Result &res) {
        if (this->stableChecked_) {
            if (!background) {
                log("no need to do stable checking, already checked before.");
            }
            return 1;
        }

        if (!background) {
            log("checking rpy if stable ...");
        }

        float rollAvg = 0;
        float rollMin;
        float rollMax;

        // What's wrong with mpu9250? if i = 10 and delay 100, the mpu will read roll,pitch yaw all as a zero().
        for (int i = 0; i < 100; i++) { //
            bool ok = this->update();
            if (!ok) {
                delay(5);
                continue;
            }
            float roll = -this->getRoll();
            float pitch = -this->getPitch();
            if (!background) {
                log(String() << "roll:" << roll << ",pitch:" << pitch);
            }

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
            delay(5);
        }
        if (continueUpdateFailed >= 100) {
            res << "cannot check stable, rpy is not available, all update are failed.";
            return -1;
        }

        if (continueUpdateFailed > 50) {
            res << "cannot check stable, rpy is not available, too many update failed, continueUpdateFailed:" << continueUpdateFailed;
            return -1;
        }
        if (Math::abs<float>(rollAvg - rollMin) > 0.5f || Math::abs<float>(rollMax - rollAvg) > 0.5f) {
            res << "rpy is not stable,rollAvg:" << rollAvg << ",rollMin:" << rollMin << ",rollMax" << rollMax;
            return -1;
        }
        if (!background) {
            log("done of rpy stable check.");
        }

        this->stableChecked_ = true;
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
