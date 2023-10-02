#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Component.h"
#include "a8/util/Context.h"
#include "a8/util/Properties.h"
#include "a8/util/Scheduler.h"
#include "a8/util/Timer.h"
#include "a8/util/StringWriter.h"

namespace a8::util {

class RateRunner : public Callback {
public:
    Rate rate;
    Timer *timer;
    Buffer<Component *> *components;
    RateRunner(Rate &rate) {
        this->rate = rate;
        this->components = new Buffer<Component *>();
    }
    void add(Component *component) {
        components->append(component);
    }

    virtual void call() override {
        long tickTime = 0; // TODO
        for (int i = 0; i < components->getLength(); i++) {
            Component *comp = components->get(i);
            comp->tick(tickTime);
        }
    }
};

class Application : public Component {

protected:
    Buffer<RateRunner *> *rateRunners_;

    Buffer<RateRunner *> *buildRateRunners(Buffer<RateRunner *> *runners) {
        Buffer<Component *> *allComponents = this->collect(new Buffer<Component *>());

        for (int i = 0; i < allComponents->getLength(); i++) {
            Component *component = allComponents->get(i);
            Rate rate = component->getRate();
            // find a runner for the component.
            RateRunner *runner = this->findRateRunner(runners, rate);
            if (runner == 0) {
                runner = new RateRunner(rate);
                runners->append(runner);
            }
            runner->add(component);
        }
        return runners;
    }
    RateRunner *findRateRunner(Buffer<RateRunner *> *runners, const Rate &rate_) {
        for (int i = 0; i < runners->getLength(); i++) {
            RateRunner *runner = runners->get(i);
            long mHz1 = runner->rate.mHz();
            long mHz2 = rate_.mHz();
            if (mHz1 == mHz2) {
                return runner;
            }
        }
        return 0;
    }

public:
    static Application *start(Context *context, Component *child) {
        Application *app = new Application("app");
        app->addChild(child)->stageTo(PostStart, context);
        return app;
    }

    Application(const char *name) : Component(name) {
    }

    void boot(Context *context) override {
        Component::boot(context);
        // Properties only valid during the start up scope, these properties are regarded as local(during application start up) variables.
        // Properties is as the interface between component and it's environment.
        // So properties normally assigned values from outside and read values by component in boot stage.

        Logger *logger = context->loggerFactory->getLogger("");
        Buffer<String> names = context->properties->getNames();

        logger->info("====== Start of properties === ");
        for (int i = 0; i < names.getLength(); i++) {
            String name = names.get(i);
            String line = context->properties->getLine(name);
            logger->info(line);
        }
        logger->info("====== End of properties =====");
    }

    void postPopulate(Context *context) override {
        Component::postPopulate(context);
    }

    void postStart(Context *context) override {
        Component::postStart(context);
        StringWriter strW;
        this->print(&strW);
        log(strW.toString());
        // ticker
        this->rateRunners_ = buildRateRunners(new Buffer<RateRunner *>());
        for (int i = 0; i < rateRunners_->getLength(); i++) {
            RateRunner *runner = rateRunners_->get(i);
            if (runner->rate.isZero()) {
                continue;
            }
            if (runner->rate.isForEver()) {
                for (int j = 0; j < runner->components->getLength(); j++) {
                    Component *com = runner->components->get(j);
                    Thread *thread = context->scheduler->schedule(com);
                    // todo storage of thread?
                }

                continue;
            }
            Timer *timer = context->scheduler->scheduleTimer(runner, runner->rate);
            runner->timer = timer;
        }
        context->scheduler->startSchedule();
    }
};

} // namespace a8::util