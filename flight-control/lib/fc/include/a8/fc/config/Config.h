#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/ImuMonitor.h"
#include "a8/fc/config/FlightConfigItem.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/fc/GlobalVars.h"
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
    Imu *rpy;
    GlobalVars& vars;
public:
    Config(GlobalVars &vars, Reader *reader, Output *out, PowerManage *pm, Imu *rpy, Scheduler *sch):vars(vars) {
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
            this->startConfigItem = ci;
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
                this->flightConfigItem = new FlightConfigItem(vars, reader, out, pm, rpy, sch);
                ci2 = ConfigItems::addReturn(ci, "Flight-Mission", this->flightConfigItem);
                ci2->onEnter = [](ConfigContext &cc) {                    
                    Config *this_ = cc.navigator->get()->getElement()->getRoot<Config>();
                    this_->missionSelect = MissionType::FLIGHT;
                    cc.navigator->stop(1);
                };
                ci2 = ConfigItems::addReturn(ci, "EscCalibrate-Mission");
                ci2->onEnter = [](ConfigContext &cc) {
                    Config *this_ = cc.navigator->get()->getElement()->getRoot<Config>();
                    this_->missionSelect = MissionType::ESC_CALIBRATE;
                    cc.navigator->stop(1);
                };
            }
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