#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/RpyMonitor.h"
#include "a8/fc/config/FlightConfigItem.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/fc/config/StartConfigItem.h"
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
    bool enableStart = true;
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
        this->attach(new Directory<ConfigItem *>("Root", 0));
    }
    ~Config() {
    }
    void onAttached() override {
        ConfigItem *ci = this;
        {
            Buffer<String> missions;
            missions.add("FlightMission");
            missions.add("EscCalibrateMission");
            ConfigItems::addSelectInput(ci, String() << "SelectMission", this->missionSelect, missions);
        }
        {
            this->startConfigItem = ConfigItems::addReturn(ci, "Start", new StartConfigItem(enableStart));
        }
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

        cc.logger->debug(">>Config::config.");
        DirectoryNavigator<ConfigContext &, ConfigItem *> nav(cc.reader, cc.out, this->dir);
        cc.navigator = &nav;
        nav.setEnterHandler([](ConfigContext &cc, DirectoryNavigator<ConfigContext &, ConfigItem *> *nav) {
            cc.logger->debug("dir enter.");
            Directory<ConfigItem *> *dir = nav->get();
            if (dir->isRoot()) {
                // ignore root,avoid recursive config.
                return;
            }
            ConfigItem *ci = dir->getElement();
            ci->enter(cc);
        });
        nav.setLeftHandler([](ConfigContext &cc, DirectoryNavigator<ConfigContext &, ConfigItem *> *nav) {
            bool changed = nav->left();
            if (!changed) {
                // go to start config item if at root node & press left key.
                Directory<ConfigItem *> *dir = nav->get();
                ConfigItem *ci = dir->getElement();
                ci->onLeftFailure(cc);
            }
        });

        cc.logger->debug(String() << "run...");

        int ret = nav.run(cc); // blocked here until stop nav.

        cc.logger->debug(String() << "<<Config::config,ret:" << ret);
    }
};
} // namespace a8::fc