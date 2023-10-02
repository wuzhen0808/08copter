#pragma once
#include "a8/link.h"
#include "a8/gs/Dashboard.h"
using namespace a8::link;

namespace a8::gs {

class GsNetImp : public GsSkeleton {
    Dashboard *dashboard;

public:
    GsNetImp(Dashboard *db) {
        this->dashboard = db;
    }
    void ping(String msg) override {
        dashboard->print(msg);
    }

    void log(String msg) override {
        dashboard->print(msg);
    }
};
} // namespace a8::gs