#pragma once
#include "a8/fc/Factory.h"
#include "a8/fc/Imu.h"
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;
class Rpy {
    Imu *imu;
    int &maxUpdateRetries_;

    float rpy_[3];

    float fRpy_[3]; // filtered rpy0.

    float aRpy_[3];

    float afRpy_[3]; // filtered rpy0.

    float offset[3];

    Filter *filter_;
    void (*releaseFilter_)(Filter *);

public:
    Rpy(Imu *imu, int &maxUpdateRetries) : maxUpdateRetries_(maxUpdateRetries) {
        this->imu = imu;
        this->rpy_[0] = 0.0f;
        this->rpy_[1] = 0.0f;
        this->rpy_[2] = 0.0f;

        this->fRpy_[0] = 0.0f;
        this->fRpy_[1] = 0.0f;
        this->fRpy_[2] = 0.0f;

        this->aRpy_[0] = 0.0f;
        this->aRpy_[1] = 0.0f;
        this->aRpy_[2] = 0.0f;
        this->afRpy_[0] = 0.0f;
        this->afRpy_[1] = 0.0f;
        this->afRpy_[2] = 0.0f;
        this->offset[0] = 0.0f;
        this->offset[1] = 0.0f;
        this->offset[2] = 0.0f;
        this->filter_ = 0;
    }
    ~Rpy() {
    }
    void releaseFilter() {
        if (this->filter_ == 0) {
            return;
        }
        this->releaseFilter_(this->filter_);
        this->filter_ = 0;
    }
    void setFilter(Filter *filter, void (*release)(Filter *)) {
        this->releaseFilter();
        this->filter_ = filter;
        this->releaseFilter_ = release;
    }

    Imu *getImu() {
        return imu;
    }

    int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        if (ret > 0)
            ret = collector->add("afRoll", afRpy_[0], res);
        if (ret > 0)
            ret = collector->add("afPitch", afRpy_[1], res);
        if (ret > 0)
            ret = collector->add("afYaw", afRpy_[2], res);
        if (ret > 0)
            ret = collector->add("aRoll", aRpy_[0], res);
        if (ret > 0)
            ret = collector->add("aPitch", aRpy_[1], res);
        if (ret > 0)
            ret = collector->add("aYaw", aRpy_[2], res);
        return ret;
    }
    void reset() {
        this->offset[0] = 0;
        this->offset[1] = 0;
        this->offset[2] = 0;
    }

    int alignYaw(Result &res) {
        int ret = this->update(res);
        if (ret < 0) {
            return ret;
        }
        this->offset[2] = -this->rpy_[2];
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
            buf[0] = aRpy_[0];
            buf[1] = aRpy_[1];
            buf[2] = aRpy_[2];
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
            if (retries >= maxUpdateRetries_) {
                res << "failed to update rpy after retries:" << retries;
                return -1;
            }
            retries++;
            A8_DEBUG("<<Bal.update.failed");
        }

        imu->get(this->rpy_);
        this->fRpy_[0] = filter_->update(rpy_[0]);
        this->fRpy_[1] = filter_->update(rpy_[1]);
        this->fRpy_[2] = filter_->update(rpy_[2]);

        aRpy_[0] = this->rpy_[0] + offset[0];
        aRpy_[1] = this->rpy_[1] + offset[1];
        aRpy_[2] = this->rpy_[2] + offset[2];
        afRpy_[0] = this->fRpy_[0] + offset[0];
        afRpy_[1] = this->fRpy_[1] + offset[1];
        afRpy_[2] = this->fRpy_[2] + offset[2];
        return 1;
    }
    void get(float *aRpy, float *afRpy) {
        aRpy[0] = this->aRpy_[0];
        aRpy[1] = this->aRpy_[1];
        aRpy[2] = this->aRpy_[2];
        afRpy[0] = this->afRpy_[0];
        afRpy[1] = this->afRpy_[1];
        afRpy[2] = this->afRpy_[2];
    }

    void get(float *aRpy) {
        aRpy[0] = this->aRpy_[0];
        aRpy[1] = this->aRpy_[1];
        aRpy[2] = this->aRpy_[2];
    }
};

} // namespace a8::fc