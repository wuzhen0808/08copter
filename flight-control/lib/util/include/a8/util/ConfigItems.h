#pragma once

#include "a8/util/ConfigItem.h"
#include "a8/util/EnumType.h"
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
    static ConfigItem *add(ConfigItem *ci, String name, unsigned long &var) {
        return addNumberInput<unsigned long>(ci, name, var);
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
    static ConfigItem *add(ConfigItem *ci1, String name, C c, void (*action)(C, ConfigContext &)) {
        ConfigItem *ci2 = ConfigItems::addReturn(ci1, name);
        struct ActionAttribute {
            C c;
            void (*action)(C, ConfigContext &);
            ActionAttribute(C c) : c(c) {
            }
        } *aa = new ActionAttribute(c);
        aa->action = action;
        ci2->setAttribute(0, aa, Lang::staticCastDelete<ActionAttribute>);
        ci2->onEnter = [](ConfigContext &cc) {
            ConfigItem *ci3 = cc.getConfigItem();
            ActionAttribute *aa = ci3->getAttribute<ActionAttribute *>(0, 0);
            aa->action(aa->c, cc);
        };
        return ci1;
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

    template <typename T>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, T &var, EnumType<T> *options) {
        return addSelectInput<T>(ci, name, var, options->options);
    }

    template <typename T>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, T &var, Options<T> &options) {
        return addSelectInput<T>(ci, name, var, &options);
    }
    template <typename T>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, T &var, Options<T> *options) {
        Buffer<T> values;
        Buffer<String> names;
        options->getValueAndNames(values, names);
        return addSelectInput<T>(ci, name, var, values, names);
    }

    template <typename T>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, T &var, Buffer<T> values, Buffer<String> names) {
        struct Params {
            T &def;
            Buffer<T> values;
            Buffer<String> names;
            Params(T &def) : def(def) {
            }
        };
        Params *p = new Params(var);
        p->values = values;
        p->names = names;
        return addSelectInput<Params *, T>(
            ci, name, var,                                                //
            p, Lang::delete_<Params>,                                     //
            p->values.len(),                                              //
            [](Params *p, int i) { return p->values.get(i, p->def); },    //
            [](Params *p, int i) { return p->names.get(i, "<unknown>"); } //
        );
    }

    template <typename C, typename T>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, T &var, //
                                      C c, void (*rcf)(C),                 //
                                      int len,                             //
                                      T (*values)(C, int),                 //
                                      String (*names)(C, int)) {
        return add<T>(
            ci, name, new SelectInput<C, T>(String() << "Please select " << name << ":", //
                                            c,                                           //
                                            rcf,                                         //
                                            len,                                         //
                                            values,                                      //
                                            names,                                       //
                                            var),                                        //
            Lang::delete_<Input<T>>,                                                     //
            var                                                                          //
        );
    }

    static ConfigItem *addSelectInput(ConfigItem *ci, String name, int &var, Buffer<String> names) {
        struct Context {
            Buffer<String> names;
        };
        Context *c = new Context();
        c->names = names;
        return add<int>(
            ci, name, new SelectInput<Context *, int>(
                          String() << "Please select " << name << ":", //
                          c,                                           //
                          Lang::delete_<Context>,
                          names.len(), //
                          [](Context *c, int idx) {
                              return idx;
                          },
                          [](Context *c, int idx) {
                              return c->names.get(idx, "No-Such-Option");
                          },           //
                          var),        //
            Lang::delete_<Input<int>>, //
            var                        //
        );
    }

    template <typename C>
    static ConfigItem *addSelectInput(ConfigItem *ci, String name, int &var, C c, int len, String (*names)(C, int)) {
        return add<int>(
            ci, name, new SelectInput<C, int>(
                          String() << "Please select " << name << ":", c, len, [](C c, int i) { return i; }, names, var), //
            Lang::delete_<Input<int>>,                                                                                    //
            var                                                                                                           //
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

    static int runNav(Directory<ConfigItem *> *dir, ConfigContext &cc) {

        cc.logger->debug(">>Config::config.");
        DirectoryNavigator<ConfigContext &, ConfigItem *> nav(cc.reader, cc.out, dir);
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
            ci->afterEnter(cc);
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

        cc.logger->debug(String() << "run nav...");

        int ret = nav.run(cc); // blocked here until stop nav.

        cc.logger->debug(String() << "done of nav." << ret);
        return ret;
    }
};

} // namespace a8::util