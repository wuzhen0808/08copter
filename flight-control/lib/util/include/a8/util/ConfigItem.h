#pragma once
#include "a8/util/Directory.h"
#include "a8/util/DirectoryNavigator.h"
#include "a8/util/HashTable.h"
#include "a8/util/Input.h"
namespace a8::util {

class ConfigItem;

class ConfigContext : public InputContext {
private:
    ConfigContext(const ConfigContext &cc);
    ConfigContext &operator=(const ConfigContext &cc);

public:
    Result &res;
    DirectoryNavigator<ConfigContext &, ConfigItem *> *navigator = 0;
    ConfigContext(Reader *reader, Output *out, Logger *logger, Result &res) : res(res), InputContext(reader, out, logger) {
    }
    ~ConfigContext() {
        A8_DEBUG("~ConfigContext");
    }

    ConfigItem *getConfigItem() {
        return this->navigator->get()->getElement();
    }
};

class ConfigItem {
public:
    const static int TAG_IS_VALID = 11;

public:
    class TitleBuilder : HashTable<String, String> {
    public:
        ConfigItem *configItem;
        Buffer<String> sortedKeyBuffer;
        TitleBuilder(ConfigItem *configItem) {
            this->configItem = configItem;
        }
        void tag(int key, char value) {
            configItem->getDirectory()->tag(key, value);
        }

        template <typename T>
        void set(String key, T value) {
            HashTable<String, String>::set(key, String() << value);
            sortedKeyBuffer.addIfNotExists(key);
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
            for (int i = 0; i < sortedKeyBuffer.len(); i++) {
                String key = sortedKeyBuffer.get(i, "");
                String value = this->get(key, "");
                title << key << ":" << value << ",";
            }
            title << ")";
            return title;
        }
    };

    using onEnterF = void (*)(ConfigContext &);
    using onAfterEnterF = void (*)(ConfigContext &);
    using onBuildTitleF = void (*)(TitleBuilder &);

protected:
    Directory<ConfigItem *> *dir;

public:
    // event handler.
    onEnterF onEnter = 0;
    onAfterEnterF onAfterEnter = 0;
    onBuildTitleF onBuildTitle = 0;

public:
    ConfigItem() {
        this->dir = 0;
    }
    Directory<ConfigItem *> *getDir() {
        return this->dir;
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
    T *getRoot() {
        ConfigItem *ci = this;
        while (true) {
            ConfigItem *p = ci->getParent<ConfigItem>();
            if (p == 0) {
                break;
            }
            ci = p;
        }
        return (T *)ci;
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
                ConfigItem *ci = buf.get(i, 0)->getElement();
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

    virtual void afterEnter(ConfigContext &cc) {
        if (this->onAfterEnter != 0) {
            this->onAfterEnter(cc);
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
            Directory<ConfigItem *> *childTree = list.get(i, 0);
            ConfigItem *child = childTree->getElement();
            consumer(c, child);
            if (recursive) {
                child->forEach<C>(c, consumer, true);
            }
        }
    }
    ConfigItem *getLastChild() {
        Directory<ConfigItem *> *child = this->dir->getLastChild();
        if (child == 0) {
            return 0;
        }
        return child->getElement();
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

} // namespace a8::util