#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Component.h"
#include "a8/util/Context.h"
#include "a8/util/Scheduler.h"
namespace a8::util {

class Application {
private:
protected:
    Buffer<Component *> *components;

    virtual void populate(Context *context) {
        int totalComponents = components->getLength();
        for (int i = 0; i < totalComponents; i++) {
            components->get(i)->populate(context);
        }
    }

    virtual void setup(Context *context) {

        int totalComponents = components->getLength();

        for (int i = 0; i < totalComponents; i++) {
            components->get(i)->setup(context);
        }
    }
    virtual void start(Context *context) {
        int totalComponents = components->getLength();

        for (int i = 0; i < totalComponents; i++) {
            components->get(i)->start(context);
        }
        context->scheduler->startSchedule();
    }

public:
    Application() {
        this->components = new Buffer<Component *>();
    }

    void add(Component *component) {
        this->components->append(component);
    }
    virtual Scheduler *createScheduler() = 0;
    virtual void log(String message) = 0;
    virtual void start() {
        Scheduler *scheduler = this->createScheduler();
        Context *context = new Context(scheduler);
        this->populate(context);

        if (context->isStop()) {
            log(String::format("Stoped with error:%s", context->getMessage().getText()));
            return;
        }
        this->setup(context);
        if (context->isStop()) {
            log(String::format("Stoped with error:%s", context->getMessage().getText()));
            return;
        }
        this->start(context);
    }
};

} // namespace a8::util