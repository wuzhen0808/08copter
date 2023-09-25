#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Component.h"
#include "a8/util/Context.h"
#include "a8/util/Properties.h"
#include "a8/util/Scheduler.h"
#include "a8/util/Timer.h"

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
private:
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
    Application(const char *name) : Component(name) {
    }
    Application(String &name) : Component(name) {
    }

    virtual Scheduler *createScheduler() = 0;
    virtual LoggerFactory *createLoggerFactory() = 0;
    virtual void start(Context &context) {
        Component::start(context);
    }

    virtual void prepare(Properties &properties) {
    }

    virtual void start() {
        Scheduler *scheduler = this->createScheduler();
        LoggerFactory *loggerFac = this->createLoggerFactory();
        // Properties only valid during the start up scope, these properties are regarded as local variables.
        // Properties is as the interface between component and it's environment.
        // So properties normally assigned values from outside and read values by component in boot stage.
        Properties properties;
        Context *context = new Context(properties, scheduler, loggerFac);
        this->stageTo(PostStart, *context);
        if (context->isStop()) {
            log(context->getMessage());
            return;
        }

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