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
    int &maxUpdateRetries;
    int &movingAvgWindowWidth;

    float rpy[3];

    float rawRpy[3];

    float offset[3];

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
    Rpy(Imu *imu, int maxUpdateRetries, int &movingAvgWindowWidth) : movingAvgWindowWidth(movingAvgWindowWidth), maxUpdateRetries(maxUpdateRetries) {
        this->imu = imu;
        this->rpy[0] = 0.0f;
        this->rpy[1] = 0.0f;
        this->rpy[2] = 0.0f;
        this->rawRpy[0] = 0.0f;
        this->rawRpy[1] = 0.0f;
        this->rawRpy[2] = 0.0f;
        this->offset[0] = 0.0f;
        this->offset[1] = 0.0f;
        this->offset[2] = 0.0f;
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
            ret = collector->add("rawRoll", rawRpy[0], res);
        if (ret > 0)
            ret = collector->add("rawPitch", rawRpy[1], res);
        if (ret > 0)
            ret = collector->add("rawYaw", rawRpy[2], res);
        return ret;
    }
    void reset() {
        freeMAs();
        createMAs();
        this->offset[0] = 0;
        this->offset[1] = 0;
        this->offset[2] = 0;
    }

    int calibrateYaw(Result &res) {
        int ret = this->update(res);
        if (ret < 0) {
            return ret;
        }
        this->offset[2] = -this->rawRpy[2];
        return 1;
    }

    int continueUpdate(System *sys, long time, long interval, Result &res) {
        long timeStart = sys->getSteadyTime();
        int ret = 1;        
        while (true) {
            ret = this->update(res);
            if (ret < 0) {
                break;
            }
            sys->delay(interval);
            long timeRemain = timeStart + time - sys->getSteadyTime();
            if (timeRemain < 0) {
                break;
            }
        }
        return ret;
    }

    int update(float *buf, Result &res) {
        int ret = this->update(res);
        if (ret > 0) {
            buf[0] = rpy[0];
            buf[1] = rpy[1];
            buf[2] = rpy[2];
        }
        return ret;
    }
    int update(Result &res) {
        int retries = 0;
        while (true) {
            bool ok = imu->update();
            if (ok) {
                break;
            }
            if (retries >= maxUpdateRetries) {
                res << "failed to update rpy after retries:" << retries;
                return -1;
            }
            retries++;
            A8_DEBUG("<<Bal.update.failed");
        }

        imu->get(rawRpy);
        rpy[0] = rawRpy[0] + offset[0];
        rpy[1] = rawRpy[1] + offset[1];
        rpy[2] = rawRpy[2] + offset[2];

        if (movingAvgWindowWidth > 1) {
            rpy[0] = rMA->update(rpy[0]);
            rpy[1] = pMA->update(rpy[1]);
            rpy[2] = yMA->update(rpy[2]);
        }

        return 1;
    }

    void get(float * buf) {
        buf[0] = this->rpy[0];
        buf[1] = this->rpy[1];
        buf[2] = this->rpy[2];
    }
};

} // namespace a8::fc