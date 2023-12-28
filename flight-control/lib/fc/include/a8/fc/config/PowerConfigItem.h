#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class PowerConfigItem : public ConfigItem {
    PowerManage *pm;
    bool &ignorePowerWarning;

public:
    PowerConfigItem(PowerManage *pm, bool &ignorePowerWarning) : ignorePowerWarning(ignorePowerWarning) {
        this->pm = pm;
    }

    void onAttached() {
        ConfigItems::add(this,"ignorePowerWarning", ignorePowerWarning);
    }

    bool isValid() override {
        if (this->ignorePowerWarning) {
            return true;
        }
        Result res;
        int ret = pm->isReady(res);
        return ret > 0;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<float>("voltage", pm->getVoltage());
        title.set<bool>("ignorePowerWarning", this->ignorePowerWarning);
    }

    void enter(ConfigContext &cc) override {
        ConfigItem::enter(cc);
    }
};

} // namespace a8::fc