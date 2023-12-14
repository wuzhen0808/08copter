#pragma once
#include "a8/util/Directory.h"
#include "a8/util/Input.h"

namespace a8::util {

class ConfigContext : public InputContext {
public:
    Result &res;
    ConfigContext(Reader *reader, Output *out, Logger *logger, Result &res) : res(res), InputContext(reader, out, logger) {
    }
};

class ConfigItem {
protected:
    Directory<ConfigItem *> *tree;

public:
    void (*onEnter)(ConfigContext &, ConfigItem *) = [](ConfigContext &cc, ConfigItem *ci) { ci->config(cc); };

public:
    ConfigItem() {
        this->tree = 0;
    }

    template <typename T>
    T *getParent() {
        if (this->tree == 0) {
            return 0;
        }
        Directory<ConfigItem *> *p = this->tree->getParent();
        if (p == 0) {
            return 0;
        }
        return (T *)p->getElement();
    }

    virtual bool isValid() {
        bool ret = true;
        if (this->tree != 0) {
            Buffer<Directory<ConfigItem *> *> buf = this->tree->getChildren();
            for (int i = 0; i < buf.len(); i++) {
                ConfigItem *ci = buf.get(i)->getElement();
                if (!ci->isValid()) {
                    ret = false;
                    break;
                }
            }
        }
        return ret;
    }

    virtual String getMemo() {
        return "";
    }

    ConfigItem *add(String name) {
        addReturn(name);
        return this;
    }
    ConfigItem *add(String name, ConfigItem *ci) {
        addReturn(name, ci);
        return this;
    }
    ConfigItem *addReturn(String name) {
        ConfigItem *ci = new ConfigItem();
        return addReturn(name, ci);
    }
    ConfigItem *addReturn(String name, ConfigItem *ci) {
        Directory<ConfigItem *> *childTree = tree->addChild(name, 0);
        ci->attach(childTree);
        return ci;
    }
    String getName() {
        if (this->tree == 0) {
            return "Unknown";
        }
        return this->tree->getName();
    }

    virtual void config(ConfigContext &cc) {
    }

    virtual bool attach(Directory<ConfigItem *> *tree) {
        if (this->tree != 0) {
            return false;
        }
        if (tree->getElement() != 0) {
            return false;
        }
        this->tree = tree;
        this->tree->setElement(this);
        this->tree->memo = [](ConfigItem *ci) {
            return ci->getMemo();
        };
        this->onAttached();
        return true;
    }

    virtual void onAttached() {
    }

    template <typename C>
    void forEach(C c, void (*consumer)(C, ConfigItem *), bool recursive) {
        if (this->tree == 0) {
            // not attached.
            return;
        }
        Buffer<Directory<ConfigItem *> *> list = this->tree->getChildren();
        for (int i = 0; i < list.len(); i++) {
            Directory<ConfigItem *> *childTree = list.get(i);
            ConfigItem *child = childTree->getElement();
            consumer(c, child);
            if (recursive) {
                child->forEach<C>(c, consumer, true);
            }
        }
    }
};

template <typename T>
class BindingInputConfigItem : public ConfigItem {
    using releaseInput = void (*)(Input<T> *);
    Input<T> *input;
    releaseInput releaseInput_ = [](Input<T> *) {};
    T &bind;

public:
    BindingInputConfigItem(Input<T> &input, T &variable) : bind(variable) {
        this->input = &input;
    }

    BindingInputConfigItem(Input<T> *input, releaseInput releaseInputF, T &variable) : bind(variable) {
        this->input = input;
        this->releaseInput_ = releaseInputF;
    }

    ~BindingInputConfigItem() {
        this->releaseInput_(this->input);
    }
    String getMemo() override {
        return String() << "value:" << bind;
    }
    void config(ConfigContext &cc) override {
        ConfigItem::config(cc);
        bind = this->input->readValue(&cc);
        cc.logger->debug(String() << "<<config,bind:" << bind);
    }
};

class ConfigItems {
public:
    static int select(Reader *reader, Output *out, String prompt, Buffer<String> options, int def, Logger *logger) {
        SelectInput input(prompt, options, def);
        int selected = read<int>(reader, out, input, logger);
        logger->debug(String() << "ConfigItems::selected:" << selected);
        return selected;
    }

    static bool confirm(Reader *reader, Output *out, String prompt, bool def, Logger *logger) {
        BoolInput input(prompt, def);
        return read<bool>(reader, out, input, logger);
    }

    template <typename T>
    static ConfigItem *addReturn(ConfigItem *ci, String name, Input<T> *input, void (*releaseInput)(Input<T> *), T &var) {
        BindingInputConfigItem<T> *ci2 = new BindingInputConfigItem<T>(input, releaseInput, var);
        return ci->addReturn(name, ci2);
    }

    static ConfigItem *add(ConfigItem *ci, String name, long &var) {
        return addNumberInput<long>(ci, name, var);
    }
    static ConfigItem *add(ConfigItem *ci, String name, float &var) {
        return addNumberInput<float>(ci, name, var);
    }
    static ConfigItem *add(ConfigItem *ci, String name, double &var) {
        return addNumberInput<double>(ci, name, var);
    }
    static ConfigItem *add(ConfigItem *ci, String name, int &var) {
        return addNumberInput<int>(ci, name, var);
    }
    static ConfigItem *add(ConfigItem *ci, String name, bool &var) {
        return addNumberInput<bool>(ci, name, var);
    }

    static ConfigItem *add(ConfigItem *ci, String name) {
        return ci->add(name);
    }

    static ConfigItem *add(ConfigItem *ci, String name, ConfigItem *ci2) {
        return ci->add(name, ci2);
    }

    static ConfigItem *addReturn(ConfigItem *ci, String name) {
        return ci->addReturn(name);
    }

    template <typename T>
    static ConfigItem *addNumberInput(ConfigItem *ci, String name, T &var) {

        return add<T>(
            ci, name, new NumberInput<T>(String() << "Please input " << name << ":", var), [](Input<T> *input) { delete input; }, var);
    }

    template <typename T>
    static ConfigItem *add(ConfigItem *ci, String name, Input<T> *input, void (*releaseInput)(Input<T> *), T &var) {
        addReturn(ci, name, input, releaseInput, var);
        return ci;
    }

    template <typename T>
    static T read(Reader *reader, Output *out, Input<T> &input, Logger *logger) {
        logger->debug(">>read.");
        T bind;
        BindingInputConfigItem<T> ci(input, bind);
        Result res;
        ConfigContext cc(reader, out, logger, res);
        ci.config(cc);
        logger->debug(String() << "<<read,bind:" << bind);
        return bind;
    }
};

} // namespace a8::util