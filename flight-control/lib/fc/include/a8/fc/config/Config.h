#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/RpyMonitor.h"
#include "a8/fc/config/FlightConfigItem.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

/**
 * Test report:
 * pwmElevation,P,      I,  D,  pidLimit(actual),  pidLimitI(actual), actualMaxPwm
 * 200,         6.0,    0,  0,  150(-158),          75(0),
 *
 */
class Config : public ConfigItem {
public:
    enum MissionType {
        FLIGHT,
        ESC_CALIBRATE
    };

public:
    FlightConfigItem *flightConfigItem;
    int missionSelect;
    int propeller0;
    int propeller1;
    int propeller2;
    int propeller3;

private:
    Reader *reader;
    Output *out;
    Scheduler *sch;
    ConfigItem *startConfigItem;
    PowerManage *pm;
    Rpy *rpy;

public:
    Config(Reader *reader, Output *out, PowerManage *pm, Rpy *rpy, Scheduler *sch) {
        this->reader = reader;
        this->out = out;
        this->sch = sch;
        this->pm = pm;
        this->rpy = rpy;
        this->missionSelect = MissionType::FLIGHT;
    }
    ~Config() {
    }
    void onAttached() override {
        ConfigItem *ci = this;
        {
            ci = ConfigItems::addReturn(ci, "Start");
            ci->onEnter = [](ConfigContext &cc) {
                cc.navigator->stop(1);
            };
            ci->onBuildTitle = [](TitleBuilder &title) {
                Config *this_ = title.configItem->getRoot<Config>();

                if (this_->missionSelect == MissionType::FLIGHT) {
                    title.set<String>("mission", "Flight");
                } else if (this_->missionSelect == MissionType::ESC_CALIBRATE) {
                    title.set<String>("mission", "EscCalibrate");
                } else {
                    title.set<String>("mission", "<Please-select>");
                }
            };
            {

                ConfigItem *ci2 = 0;
                ci2 = ConfigItems::addReturn(ci, "Flight-Mission");
                ci2->onEnter = [](ConfigContext &cc) {                    
                    Config *this_ = cc.navigator->get()->getElement()->getRoot<Config>();
                    this_->missionSelect = MissionType::FLIGHT;
                };
                ci2 = ConfigItems::addReturn(ci, "EscCalibrate-Mission");
                ci2->onEnter = [](ConfigContext &cc) {
                    Config *this_ = cc.navigator->get()->getElement()->getRoot<Config>();
                    this_->missionSelect = MissionType::ESC_CALIBRATE;
                };
            }
        }

        ci = this;
        {
            this->flightConfigItem = new FlightConfigItem(reader, out, pm, rpy, sch);
            ConfigItems::addReturn(ci, "FlightConfig", this->flightConfigItem);
        }
    }

    bool isValid() override {
        if (this->missionSelect == MissionType::FLIGHT) {
            return this->flightConfigItem->isValid();
        } else if (this->missionSelect == MissionType::ESC_CALIBRATE) {
            return true;
        } else {
            // no such mission type.
            return false;
        }
    }
    void onLeftFailure(ConfigContext &cc) override {
        cc.navigator->to(this->startConfigItem->getDirectory());
    }

    void enter(ConfigContext &cc) override {
        ConfigItems::runNav(this->dir, cc);
    }
};
} // namespace a8::fc