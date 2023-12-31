#pragma once

#include "a8/fc/config/PropellerConfigItem.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class PropellersConfigItem : public ConfigItem {
    bool &lockPropellers;

public:
    Buffer<PropellerConfigItem *> propellers;
    PropellersConfigItem(bool &lockPropellers, float &activeThrottle0, float &activeThrottle1, float &activeThrottle2, float &activeThrottle3) : lockPropellers(lockPropellers) {
        this->propellers.add(new PropellerConfigItem(activeThrottle0));
        this->propellers.add(new PropellerConfigItem(activeThrottle1));
        this->propellers.add(new PropellerConfigItem(activeThrottle2));
        this->propellers.add(new PropellerConfigItem(activeThrottle3));
    }
    ~PropellersConfigItem() {
        propellers.clear(Lang::delete_<PropellerConfigItem>);
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
    }

    void onAttached() override {
        for (int i = 0; i < this->propellers.len(); i++) {
            ConfigItems::add(this, String() << "Propeller" << i, BufferUtil::get<PropellerConfigItem>(&propellers, i));
        }
        ConfigItems::add(this, "lockPropellers", lockPropellers);
    }
};

} // namespace a8::fc