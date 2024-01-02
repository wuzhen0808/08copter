#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Imu.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class RpyConfigItem : public ConfigItem {
    Imu *imu;
    float &unBalanceLimit;
    float deg;

public:
    RpyConfigItem(Imu *imu, float &unBalanceLimit) : unBalanceLimit(unBalanceLimit) {
        this->imu = imu;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("balance", imu->isBalance(unBalanceLimit, true));
    }

    void onAttached() override {

        ConfigItems::addReturn<RpyConfigItem *>(this, "Check stable.", this, [](RpyConfigItem *this_, Result &res) {
            return this_->imu->checkStable(res);
        });

        ConfigItems::addReturn<RpyConfigItem *>(this, "Check balance.", this, [](RpyConfigItem *this_, Result &res) {
            return this_->imu->checkBalance(false, this_->unBalanceLimit, this_->deg, res);
        });
    }

    bool isValid() override {
        return true;
    }

    void enter(ConfigContext &cc) override {
    }
};

} // namespace a8::fc