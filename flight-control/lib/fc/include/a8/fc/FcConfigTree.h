#pragma once
#include "a8/fc/ConfigTree.h"
namespace a8::fc {
class RpyStableCheckConfigItem : public ConfigItem {
    Rpy *rpy;

public:
    RpyStableCheckConfigItem(Rpy *rpy) {
        this->rpy = rpy;
    }

    int update(Logger *logger, Result &res) override {
        return rpy->checkIfStable(res);
    }
};

class FcConfigTree : public ConfigTree {

public:
    int select(String prompt, String option0, String option1, int def) {
        FcConfigTree tree(this->reader, out, 0, prompt, this->loggerFactory);
        struct Params {
            int value;
        };
        Params *uo = new Params();
        tree.setConfigItem<int>(
            prompt, InputType::SELECT, def, uo, [](void *uo) { delete static_cast<Params *>(uo); },
            [](void *pp, void *value, Result &res) {
                Params *ppp = static_cast<Params *>(pp);
                ppp->value = *static_cast<int *>(value);
                return 1;
            });

        InputCallbackConfigItem *configItem = static_cast<InputCallbackConfigItem *>(tree.getConfigItem());
        SelectInput *sInput = static_cast<SelectInput *>(configItem->getInput());
        sInput->addOption(option0);
        sInput->addOption(option1);
        Result res;
        int ret = tree.config(tree.logger, res);
        if (ret < 0) {
            return def;
        }
        return uo->value;
    }

    FcConfigTree(LineReader *reader, Output *out, Config &config, LoggerFactory *logFac) : ConfigTree(reader, out, 0, "Root", logFac) {

        ConfigTree *ct = this;
        ct->addConfig<long>("delayBeforeStartSec", InputType::LONG, config.delayBeforeStartSec);
        ct = ct->addConfig(String() << "Pid-arguments:");
        {

            ct->addConfig<double>(String() << "Kp", InputType::DOUBLE, config.pidKp);
            ct->addConfig<double>(String() << "Ki", InputType::DOUBLE, config.pidKi);
            ct->addConfig<double>(String() << "Kd", InputType::DOUBLE, config.pidKd);
            ct->addConfig<double>(String() << "maxBalancePidOutput", InputType::DOUBLE, config.maxBalancePidOutput);
            ct->addConfig<double>(String() << "maxBalancePidIntegralOutput", InputType::DOUBLE, config.maxBalancePidIntegralOutput);
        }
        ct = this;
        ct->addConfig<long>(String() << "pwmElevation", InputType::LONG, config.pwmElevation);
        ct->addConfig<long>(String() << "flyingTimeLimit(sec)", InputType::LONG, config.flyingTimeLimitSec);
        ct->addConfig<long>(String() << "delayBeforeStart(sec)", InputType::LONG, config.delayBeforeStartSec);
        ct->addConfig<int>(String() << "enablePropeller", InputType::SELECT, config.enablePropeller);
    }
    FcConfigTree(LineReader *reader, Output *out, FcConfigTree *parent, String name, LoggerFactory *logFac) : ConfigTree(reader, out, parent, name, logFac) {
    }

    DirectoryTree *newDirectoryTree(DirectoryTree *parent, String name) override {
        return new FcConfigTree(this->reader, out, static_cast<FcConfigTree *>(parent), name, this->loggerFactory);
    }
};
} // namespace a8::fc