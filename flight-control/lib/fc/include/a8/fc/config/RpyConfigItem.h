#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class RpyConfigItem : public ConfigItem {
    Rpy *rpy;

public:
    RpyConfigItem(Rpy *rpy) {
        this->rpy = rpy;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("balance", rpy->isBalance(true));
    }

    void onAttached() override {

        ConfigItems::addReturn<RpyConfigItem *>(this, "Check stable.", this, [](RpyConfigItem *this_, Result &res) {
            return this_->rpy->checkStable(res);
        });

        ConfigItems::addReturn<RpyConfigItem *>(this, "Check balance.", this, [](RpyConfigItem *this_, Result &res) {
            return this_->rpy->checkBalance(false, res);
        });
    }

    bool isValid() override {
        return true;
    }

    void enter(ConfigContext &cc) override {
    }
};

} // namespace a8::fc