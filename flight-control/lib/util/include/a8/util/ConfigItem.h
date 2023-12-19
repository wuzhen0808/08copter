#pragma once
#include "a8/util/Directory.h"
#include "a8/util/HashTable.h"
#include "a8/util/Input.h"
namespace a8::util {

class ConfigItem;

class ConfigContext : public InputContext {
public:
    Result &res;
    DirectoryNavigator<ConfigContext &, ConfigItem *> *navigator = 0;
    ConfigContext(Reader *reader, Output *out, Logger *logger, Result &res) : res(res), InputContext(reader, out, logger) {
    }
};

class ConfigItem {
public:
    const static int TAG_IS_VALID = 11;

public:
    class TitleBuilder : HashTable<String, String> {
    public:
        ConfigItem *configItem;
        TitleBuilder(ConfigItem *configItem) {
            this->configItem = configItem;
        }
        void tag(int key, char value) {
            configItem->getDirectory()->tag(key, value);
        }

        template <typename T>
        void set(String key, T value) {
            HashTable<String, String>::set(key, String() << value);
        }

        template <typename T>
        T *getConfigItem() {
            return static_cast<T *>(this->configItem);
        }

        String build() {
            String title;
            Directory<ConfigItem *> *dir = configItem->getDirectory();
            title << "[";
            title << (configItem->isValid() ? '_' : '*');
            title << "]";
            title << configItem->getName();

            title << "(";
            this->forEach<String &>(title, [](String &title, String key, String value) {
                title << key << ":" << value << ",";
            });
            title << ")";
            return title;
        }
    };

    using onEnterF = void (*)(ConfigContext &);
    using onBuildTitleF = void (*)(TitleBuilder &);

protected:
    Directory<ConfigItem *> *dir;

public:
    // event handler.
    onEnterF onEnter = 0;
    onBuildTitleF onBuildTitle = 0;

public:
    ConfigItem() {
        this->dir = 0;
    }

    void setAttribute(void *value, void (*release)(void *)) {
        this->setAttribute(0, value, release);
    }

    void setAttribute(int key, void *value, void (*release)(void *)) {
        this->dir->setAttribute(key, value, release);
    }

    template <typename T>
    T getAttribute(int key, T def) {
        return dir->getAttribute<T>(key, def);
    }
    template <typename T>
    T getAttribute(T def) {
        return dir->getAttribute<T>(def);
    }
    Directory<ConfigItem *> *getDirectory() {
        return this->dir;
    }
    template <typename T>
    T *getParent() {
        if (this->dir == 0) {
            return 0;
        }
        Directory<ConfigItem *> *p = this->dir->getParent();
        if (p == 0) {
            return 0;
        }
        return (T *)p->getElement();
    }

    virtual void onLeftFailure(ConfigContext &cc) {
    }

    virtual void onRightFailure(ConfigContext &cc) {
    }

    virtual bool isValid() {
        bool ret = true;
        if (this->dir != 0) {
            Buffer<Directory<ConfigItem *> *> buf = this->dir->getChildren();
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

    virtual void buildTitle(TitleBuilder &title) {
        if (onBuildTitle != 0) {
            this->onBuildTitle(title);
        }
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
        Directory<ConfigItem *> *childTree = dir->addChild(name, 0);
        ci->attach(childTree);
        return ci;
    }
    String getName() {
        if (this->dir == 0) {
            return "Unknown";
        }
        return this->dir->getName();
    }

    virtual void enter(ConfigContext &cc) {
        if (this->onEnter != 0) {
            this->onEnter(cc);
        }
    }

    virtual bool attach(Directory<ConfigItem *> *dir) {
        if (this->dir != 0) {
            return false;
        }
        if (dir->getElement() != 0) {
            return false;
        }
        this->dir = dir;
        this->dir->setElement(this);
        this->dir->title = [](Directory<ConfigItem *> *dir) {
            String name = dir->getName();
            TitleBuilder title(dir->getElement());
            dir->getElement()->buildTitle(title);
            return title.build();
        };
        this->onAttached();
        return true;
    }

    virtual void onAttached() {
    }

    template <typename C>
    void forEach(C c, void (*consumer)(C, ConfigItem *), bool recursive) {
        if (this->dir == 0) {
            // not attached.
            return;
        }
        Buffer<Directory<ConfigItem *> *> list = this->dir->getChildren();
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
class InputConfigItem : public ConfigItem {
    using releaseInput = void (*)(Input<T> *);

protected:
    Input<T> *input;
    releaseInput releaseInput_ = [](Input<T> *) {};

public:
    InputConfigItem(Input<T> &input) {
        this->input = &input;
    }

    InputConfigItem(Input<T> *input, releaseInput releaseInputF) {
        this->input = input;
        this->releaseInput_ = releaseInputF;
    }

    ~InputConfigItem() {
        this->releaseInput_(this->input);
    }

    virtual void buildTitle(TitleBuilder &title) = 0;

    virtual void enter(ConfigContext &cc) = 0;
};

template <typename T>
class BindingInputConfigItem : public InputConfigItem<T> {
    using releaseInput = void (*)(Input<T> *);
    T &bind;

public:
    BindingInputConfigItem(Input<T> &input, T &variable) : InputConfigItem<T>(input), bind(variable) {
    }

    BindingInputConfigItem(Input<T> *input, releaseInput releaseInputF, T &variable) : InputConfigItem<T>(input, releaseInputF), bind(variable) {
    }

    ~BindingInputConfigItem() {
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        if (this->onBuildTitle == 0) {
            title.set<T>("value", bind);
        } else {
            this->onBuildTitle(title);
        }
    }

    T getValue() {
        return this->bind;
    }

    void enter(ConfigContext &cc) override {
        ConfigItem::enter(cc);
        bind = this->input->readValue(&cc);
        cc.logger->debug(String() << "<<config,bind:" << bind);
    }
};

template <typename C, typename T>
class MethodInputConfigItem : public InputConfigItem<T> {
    using releaseInput = void (*)(Input<T> *);
    using getMethod = T (*)(C);
    using setMethod = void (*)(C, T);

    C context;
    getMethod getMethod_;
    setMethod setMethod_;

public:
    MethodInputConfigItem(Input<T> &input, C c, getMethod getMethod, setMethod setMethod) : InputConfigItem<T>(input) {
        this->context = c;
        this->getMethod_ = getMethod;
        this->setMethod_ = setMethod;
    }

    MethodInputConfigItem(Input<T> *input, releaseInput releaseInputF, C c, getMethod getMethod, setMethod setMethod) : InputConfigItem<T>(input, releaseInputF) {
        this->context = c;
        this->getMethod_ = getMethod;
        this->setMethod_ = setMethod;
    }

    ~MethodInputConfigItem() {
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        T value = this->getMethod_(this->context);
        title.set<T>("value", value);
    }

    void enter(ConfigContext &cc) override {
        ConfigItem::enter(cc);
        T value = this->input->readValue(&cc);
        this->setMethod_(this->context, value);
    }
};

class ConfigItems {
public:
    static bool confirm(ConfigContext &cc, String prompt, bool def) {
        return confirm(cc.lines, cc.out, prompt, def, cc.logger);
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
    template <typename C>
    static ConfigItem *add(ConfigItem *ci, String name, int &var, C c, int len, String (*options)(C, int)) {
        return addSelectInput<C>(ci, name, var, c, len, options);
    }

    static ConfigItem *add(ConfigItem *ci, String name, bool &var) {
        ConfigItem *ci2 = addReturn<bool>(
            ci, name, new BoolInput(name, var),       //
            [](Input<bool> *input) { delete input; }, //
            var                                       //
        );
        ci2->onBuildTitle = [](ConfigItem::TitleBuilder &title) {
            BindingInputConfigItem<bool> *ci = title.getConfigItem<BindingInputConfigItem<bool>>();
            title.set<String>("value", ci->getValue() ? "Yes" : "No");
        };
        return ci;
    }

    static ConfigItem *add(ConfigItem *ci, String name) {
        return ci->add(name);
    }

    static ConfigItem *add(ConfigItem *ci, String name, ConfigItem *ci2) {
        return ci->add(name, ci2);
    }

    static ConfigItem *addReturn(ConfigItem *ci, String name, ConfigItem *ci2) {
        return ci->addReturn(name, ci2);
    }

    static ConfigItem *addReturn(ConfigItem *ci, String name) {
        return ci->addReturn(name);
    }

    template <typename C>
    static ConfigItem *addReturn(ConfigItem *ci, String name, C c, int (*action)(C, Result &)) {
        struct Context {
            C c;
            int (*action)(C, Result &);
            Result res;
            int ret = 0;
            String getTitle() {
                return String() << ret << ":" << res.errorMessage;
            }
        } *c2 = new Context();
        c2->c = c;
        c2->action = action;

        ci = ConfigItems::addReturn(ci, name);
        ci->setAttribute(c2, Lang::release<Context *>);
        ci->onBuildTitle = [](ConfigItem::TitleBuilder &title) {
            Context *context = title.configItem->getAttribute<Context *>(0);
            title.set<String>("status", context->getTitle());
        };
        ci->onEnter = [](ConfigContext &cc) {
            ConfigItem *ci = cc.navigator->get()->getElement();
            Context *c2 = ci->getAttribute<Context *>(0);
            Result res;
            int ret = c2->action(c2->c, res);
            c2->res = res;
            c2->ret = ret;
        };
        return ci;
    }

    template <typename C>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, int &var, C c, int len, String (*options)(C, int)) {
        return add<int>(
            ci, name, new SelectInput<C>(String() << "Please select " << name << ":", c, len, options, var), //
            [](Input<int> *input) { delete input; },                                                         //
            var                                                                                              //
        );
    }
    template <typename T>
    static ConfigItem *addNumberInput(ConfigItem *ci, String name, T &var) {
        return add<T>(
            ci, name, new NumberInput<T>(String() << "Please input " << name << ":", var), //
            [](Input<T> *input) { delete input; },                                         //
            var                                                                            //
        );
    }

    template <typename C, typename T>
    static ConfigItem *add(ConfigItem *ci, String name, C c, T (*get)(C), void (*set)(C, T)) {
        Input<T> *input = new NumberInput<T>(String() << "Please input " << name << ":", 0);
        MethodInputConfigItem<C, T> *ci2 = new MethodInputConfigItem<C, T>(
            input, [](Input<T> *input) { delete input; }, c, get, set);
        return add(ci, name, ci2);
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
        ci.enter(cc);
        logger->debug(String() << "<<read,bind:" << bind);
        return bind;
    }
};

} // namespace a8::util