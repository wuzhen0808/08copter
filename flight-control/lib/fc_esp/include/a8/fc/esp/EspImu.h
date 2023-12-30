#pragma once
#include "a8/fc.h"
// lang-c-format off
#include <Arduino.h>
#include <MPU9250.h>
// lang-c-format on

namespace a8::fc::esp {
class EspImu : public Imu, public FlyWeight {
    MPU9250 *mpu;
    bool setupOk_;
    int continueUpdateFailed = 0;
    bool firstStableCheckedIsOk = false;
    String lastStableCheckError;
    String lastBalanceCheckError;
    int setupMpu9250() {
        mpu->setup(0x68);
        mpu->selectFilter(QuatFilterSel::MADGWICK);
        mpu->setFilterIterations(5);
        log("success of setupMpu9250.");
        return 1;
    }

public:
    EspImu(LoggerFactory *logFac) : FlyWeight(logFac, "EspRpy") {
        this->mpu = new MPU9250();
        this->lastBalanceCheckError = "balance is unknown.";
        this->lastStableCheckError = "stable is unknown.";
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

    String getLastError() override {
        return String() << this->lastStableCheckError << ";" << this->lastBalanceCheckError;
    }

    bool update() override {

        A8_TRACE(">>EspRpy::update.");
        bool ret = mpu->update();
        A8_TRACE((String() << "<<EspRpy::update,ret:" << ret).text());
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
    void get(float &roll, float &pitch, float &yaw) override {
        roll = mpu->getRoll();
        pitch = mpu->getPitch();
        yaw = mpu->getYaw();
    }
    int checkReady(float limit, Result &res) override {

        int ret = this->doCheckStable(false, res);
        if (ret < 0) {
            return ret;
        }
        float deg;
        ret = this->checkBalance(false, limit, deg, res);
        if (ret < 0) {
            return ret;
        }

        return ret;
    }

    bool isBalance(float limit, bool silence) override {
        Result res;
        float deg;
        return this->checkBalance(silence, limit, deg, res) > 0;
    }

    int checkStable(Result &res) override {
        int ret = doCheckStable(false, res);
        if (ret < 0) {
            this->lastStableCheckError = String() << ret << ":" << res.errorMessage;
        } else {
            this->lastStableCheckError.clear();
        }
        return ret;
    }

    int doCheckStable(bool silence, Result &res) {

        if (!silence) {
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
            if (!silence) {
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
        if (!silence) {
            log("done of rpy stable check.");
        }

        return 1;
    }
    int checkBalance(bool silence, float limit, float &deg, Result &res) {
        A8_LOG_DEBUG(logger, String() << ">>checkBalance.");
        int ret = this->doCheckBalance(silence, limit, deg, res);
        if (ret < 0) {
            this->lastBalanceCheckError = String() << ret << ":" << res.errorMessage;
        } else {
            this->lastBalanceCheckError.clear();
        }
        A8_LOG_DEBUG(logger, String() << "<<checkBalance.");
        return ret;
    }
    int doCheckBalance(bool silence, float limit, float &deg, Result &res) {
        float roll = this->getRoll();
        float pitch = this->getPitch();
        A8_LOG_DEBUG(logger, String() << "check rpy if balance, roll:" << roll << ",pitch:" << pitch);

        float aPitch = Math::abs<float>(pitch);
        float aRoll = Math::abs<float>(roll);

        if (aPitch > limit || aRoll > limit) {
            res << "rpy is not balance, roll(" << roll << ") or pitch(" << pitch << ") value showing the attitude of the drone is not in a balance position, please adjust and try again.";
            deg = Math::max(aPitch, aRoll);
            return -1;
        }

        if (!silence) {
            log("done of rpy balance check.");
        }
        return 1;
    }
};

} // namespace a8::fc::esp
