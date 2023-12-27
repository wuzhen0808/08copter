#pragma once
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class PropellerConfigItem : public ConfigItem {
    float &activeThrottle;

public:
    PropellerConfigItem(float &activeThrottle) : activeThrottle(activeThrottle) {
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<long>("activeThrottle", activeThrottle);
    }

    void onAttached() override {
        ConfigItems::add(this, "activeThrottle", activeThrottle);
    }
};

} // namespace a8::fc