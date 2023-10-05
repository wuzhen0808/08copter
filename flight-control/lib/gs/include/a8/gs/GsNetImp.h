#pragma once
#include "a8/gs/Dashboard.h"
#include "a8/link.h"
using namespace a8::link;

namespace a8::gs {

class GsNetImp : public GsSkeleton, public Component {
    Dashboard *dashboard;

public:
    GsNetImp(Dashboard *db) : Component("skeleton") {
        this->dashboard = db;
    }    
    void ping(String msg) override {
        log(String() << "on ping message:" << msg);
        dashboard->print(msg);
    }

    void log(String msg) override {
        Component::log(String() << "on log message:" << msg);
        dashboard->print(msg);
    }
};
} // namespace a8::gs