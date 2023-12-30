#pragma once
#include "a8/fc/Imu.h"
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;
class Rpy {
    Imu *imu;
    MovingAvg<float> *rMA;
    MovingAvg<float> *pMA;
    MovingAvg<float> *yMA;
    int &movingAvgWindowWidth;

    float roll;
    float pitch;
    float yaw;

    float rawRoll;
    float rawPitch;
    float rawYaw;

    void freeMAs() {
        delete this->rMA;
        delete this->pMA;
        delete this->yMA;
    }

    void createMAs() {
        this->rMA = new MovingAvg<float>(movingAvgWindowWidth);
        this->pMA = new MovingAvg<float>(movingAvgWindowWidth);
        this->yMA = new MovingAvg<float>(movingAvgWindowWidth);
    }

public:
    Rpy(Imu *imu, int &movingAvgWindowWidth) : movingAvgWindowWidth(movingAvgWindowWidth) {
        this->imu = imu;
        this->roll = 0;
        this->pitch = 0;
        this->yaw = 0;
        this->rawRoll = 0;
        this->rawPitch = 0;
        this->rawYaw = 0;
        createMAs();
    }
    ~Rpy() {
        freeMAs();
    }
    Imu *getImu() {
        return imu;
    }

    int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        if (ret > 0)
            ret = collector->add("rawRoll", rawRoll, res);
        if (ret > 0)
            ret = collector->add("rawPitch", rawPitch, res);
        if (ret > 0)
            ret = collector->add("rawYaw", rawYaw, res);
        return ret;
    }
    void reset() {
        freeMAs();
        createMAs();
    }

    int update(int maxRetries, Result &res) {
        int retries = 0;
        while (true) {
            bool ok = imu->update();
            if (ok) {
                break;
            }
            if (retries >= maxRetries) {
                res << "failed to update rpy after retries:" << retries;
                return -1;
            }
            retries++;
            A8_DEBUG("<<Bal.update.failed");
        }

        imu->get(rawRoll, rawPitch, rawYaw);
        if (movingAvgWindowWidth > 1) {
            roll = rMA->update(rawRoll);
            pitch = pMA->update(rawPitch);
            yaw = yMA->update(rawYaw);
        } else {
            roll = rawRoll;
            pitch = rawPitch;
            yaw = rawYaw;
        }
        return 1;
    }
    void get(float &roll, float &pitch, float &yaw) {
        roll = this->roll;
        pitch = this->pitch;
        yaw = this->yaw;
    }
};

} // namespace a8::fc