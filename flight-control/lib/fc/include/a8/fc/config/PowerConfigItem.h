#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class PowerConfigItem : public ConfigItem {
    PowerManage *pm;

public:
    PowerConfigItem(PowerManage *pm) {
        this->pm = pm;
    }

    bool isValid() override {
        Result res;
        int ret = pm->isReady(res);
        return ret > 0;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<float>("voltage", pm->getVoltage());
    }

    void enter(ConfigContext &cc) override {
        ConfigItem::enter(cc);
    }
};

} // namespace a8::fc