#pragma once
#include "a8/fc/Input.h"
#include "a8/util/DirectoryTree.h"

namespace a8::fc {
using namespace a8::util;
enum InputType {
    FLOAT,
    LONG,
    BOOL,
    DOUBLE,
    SELECT

};
static String &operator<<(String &str, InputType type) {
    switch (type) {
    case FLOAT:
        str << "Float";
        break;
    case LONG:
        str << "Long";
        break;
    case BOOL:
        str << "Bool";
        break;
    case DOUBLE:
        str << "Double";
        break;
    case SELECT:
        str << "Select";
        break;
    default:
        str << "Unknown";
        break;
    }
    return str;
}
using configCallback = int (*)(void *, void *, Result &);

class ConfigItem {
    bool isDirty = true;

public:
    void setDirty(bool dirty) {
        this->isDirty = dirty;
    }

    int config(Logger *logger, Result &res) {
        if (!this->isDirty) {
            return 1;
        }

        int ret = this->update(logger, res);
        if (ret > 0) {
            isDirty = false;
        }
        return ret;
    }
    virtual int update(Logger *logger, Result &res) = 0;
};

class InputCallbackConfigItem : public ConfigItem {
protected:
    InputType type;
    String title;
    Input *input;
    void *userObject;
    configCallback configCallback_;
    LineReader *reader;
    void (*releaseUserObject)(void *);

public:
    InputCallbackConfigItem(LineReader *reader, Output *out, InputType type, String title, void *default_, void *userObject, void (*releaseUserObject)(void *), configCallback configCallbackF) {
        this->userObject = userObject;
        this->releaseUserObject = releaseUserObject;
        this->reader = reader;
        this->type = type;
        this->title = title;
        this->configCallback_ = configCallbackF;
        String defaultStr;
        switch (this->type) {
        case FLOAT: {

            float defaultV = *static_cast<float *>(default_);
            this->input = new NumberInput<float>(this->reader, out, title, defaultV);
            defaultStr << defaultV;
        } break;
        case BOOL: {
            bool defaultV = *static_cast<bool *>(default_);
            this->input = new BoolInput(this->reader, out, title, defaultV);
            defaultStr << (defaultV ? "true" : "false");
        } break;
        case LONG: {
            long defaultV = *static_cast<long *>(default_);
            this->input = new NumberInput<long>(this->reader, out, title, defaultV);
            defaultStr << defaultV;
        } break;
        case DOUBLE: {
            double defaultV = *static_cast<double *>(default_);
            this->input = new NumberInput<double>(this->reader, out, title, defaultV);
            defaultStr << defaultV;
        }

        break;
        case SELECT: {
            int defaultV = *static_cast<int *>(default_);
            this->input = new SelectInput(this->reader, out, title, defaultV);
            defaultStr << defaultV;
        } break;
        default: {
            this->input = new UnknownInput(this->reader, out, title);
            defaultStr << "";
        } break;
        }
        String prompt = String() << "Please input argument(name:" << this->title << ",type:" << this->type
                                 << ",default: " << defaultStr << ")";
        this->input->setPrompt(prompt);
    }

    ~InputCallbackConfigItem() {
        delete this->input;
        this->releaseUserObject(this->userObject);
    }

    Input *getInput() {
        return this->input;
    }

    int update(Logger *logger, Result &res) override {
        logger->debug(String() << ">>update,type:" << type << ",input:" << input->getName());
        struct Params {
            Result &res;
            int ret;
            InputCallbackConfigItem *configItem;
            Params(Result &res) : res(res) {
            }

        } p(res);
        p.configItem = this;
        logger->debug(".update.1");
        this->input->readValue(logger, &p, [](void *pp, void *value) {
            Params *ppp = static_cast<Params *>(pp);
            ppp->ret = ppp->configItem->doUpdate(value, ppp->res);
        });
        logger->debug("<<update.");
        return p.ret;
    }
    int doUpdate(void *value, Result &res) {
        return this->configCallback_(userObject, value, res);
    }
};

class ConfigTree : public DirectoryTree, public FlyWeight {
protected:
    LineReader *reader;
    Output *out;

public:
    ConfigTree(LineReader *reader, Output *out, ConfigTree *parent, String name, LoggerFactory *logFac) : DirectoryTree(parent, name), FlyWeight(logFac) {
        this->reader = reader;
        this->out = out;
    }

    ~ConfigTree() {
        // release menu item.
        this->setConfigItem(0);
    }

    virtual DirectoryTree *newDirectoryTree(DirectoryTree *parent, String name) override {
        return new ConfigTree(this->reader, out, static_cast<ConfigTree *>(parent), name, this->loggerFactory);
    }

    ConfigTree *addConfig(String name) {
        log(String() << ">>addConfig." << name);
        ConfigTree *child = static_cast<ConfigTree *>(DirectoryTree::addChild(name));
        log(String() << "<<addConfig." << name);
        return child;
    }

    template <typename T>
    ConfigTree *addConfig(String name, InputType type, T &variable) {
        log(String() << ">>addConfig." << name);
        ConfigTree *child = this->addConfig(name);
        struct UserObject {
            T &variable;
            UserObject(T &v) : variable(v) {
            }
        };
        UserObject *uo = new UserObject(variable);
        child->setConfigItem<T>(
            name, type, variable, uo, [](void *uo) { delete static_cast<UserObject *>(uo); },
            [](void *uo, void *value, Result &res) {
                UserObject *uoP = static_cast<UserObject *>(uo);
                uoP->variable = *static_cast<T *>(value);
                return 1;
            });
        log(String() << "<<addConfig." << name);
        return child;
    }

    template <typename T>
    void setConfigItem(String name, InputType type, T def, void *userObject, void (*releaseUO)(void *), configCallback callback) {
        setConfigItem(new InputCallbackConfigItem(this->reader, out, type, name, &def, userObject, releaseUO, callback));
    }

    void setConfigItem(ConfigItem *configItem) {
        log(String() << ">>setConfigItem.");
        if (this->element != 0) {
            ConfigItem *ci = static_cast<ConfigItem *>(this->element);
            if (ci != 0) {
                delete ci;
            }
            this->element = 0;
        }
        this->element = configItem;
        log(String() << "<<setConfigItem.");
    }

    template <typename T>
    ConfigTree *addConfig(String name, InputType type, T def, void *context, configCallback callback) {
        ConfigTree *child = addConfig(name);
        child->setConfigItem<T>(name, def, context, callback);
        return child;
    }

    ConfigItem *getConfigItem() {
        if (this->element == 0) {
            return 0;
        }
        return static_cast<ConfigItem *>(element);
    }
    int config(Logger *logger, Result &res) {
        log(String() << ">>config,name:" << this->name);
        int ret = this->doConfig(logger, res);
        log(String() << "<<config,name:" << this->name);
        return ret;
    }
    int doConfig(Logger *logger, Result &res) {

        for (int i = 0; i < this->children.len(); i++) {
            ConfigTree *ct = static_cast<ConfigTree *>(children.get(i));
            int ret = ct->config(logger, res);
            if (ret < 0) {
                return ret;
            }
        }
        ConfigItem *ci = this->getConfigItem();
        if (ci != 0) {
            return ci->config(logger, res);
        }
        return 1;
    }
};
} // namespace a8::fc