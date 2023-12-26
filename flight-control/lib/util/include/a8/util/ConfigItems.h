#pragma once
#include "a8/util/ConfigItem.h"
namespace a8::util {
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
            void reset() {
                res.reset();
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
            c2->reset();
            c2->ret = c2->action(c2->c, c2->res);
        };
        return ci;
    }

    static ConfigItem *addSelectInput(ConfigItem *ci, String name, int &var, Buffer<String> options) {
        struct Context {
            Buffer<String> options;
        };
        Context *c = new Context();
        c->options = options;
        return add<int>(
            ci, name, new SelectInput<Context *>(
                          String() << "Please select " << name << ":", //
                          c,                                           //
                          Lang::delete_<Context>,
                          options.len(), //
                          [](Context *c, int idx) {
                              return c->options.get(idx, "No-Such-Option");
                          },           //
                          var),        //
            Lang::delete_<Input<int>>, //
            var                        //
        );
    }

    template <typename C>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, int &var, C c, int len, String (*options)(C, int)) {
        return add<int>(
            ci, name, new SelectInput<C>(String() << "Please select " << name << ":", c, len, options, var), //
            Lang::delete_<Input<int>>,                                                                       //
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