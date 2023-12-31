#pragma once
#include "a8/fc/Propellers.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/collect/Collector.h"
#include "a8/fc/throttle/Throttle.h"
#include "a8/util.h"
namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

class Mission : public FlyWeight {
private:
    Mission(const Mission &);
    Mission &operator=(const Mission &);

protected:
    System *sys;
    ConfigContext &configContext;
    Throttle &throttle;
    SyncQueue<int> *signalQueue;
    Collector *collector;
    long id;

public:
    Mission(long id, ConfigContext &configContext, Throttle &throttle, SyncQueue<int> *signalQueue, Collector *collector, System *sys, LoggerFactory *logFac, String name)
        : FlyWeight(logFac, name), configContext(configContext), throttle(throttle) {
        this->id = id;
        this->sys = sys;
        this->signalQueue = signalQueue;
        this->collector = collector;
    }

    ~Mission() {
    }
    
    void signal(int signal) {
        this->signalQueue->offer(signal);
    }
    
    long getId() {
        return id;
    }
    Collector *getCollector() {
        return this->collector;
    }
    virtual long getPreStartDelaySec() {
        return 0;
    }
    virtual int setup(Result &res) {
        return 0;
    }

    virtual ConfigItem *getForeground() {
        return 0;
    }
    virtual int run(Result &res) = 0;
};

} // namespace a8::fc::mission