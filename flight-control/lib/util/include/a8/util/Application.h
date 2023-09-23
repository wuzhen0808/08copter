#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Component.h"
#include "a8/util/Context.h"
#include "a8/util/Scheduler.h"
namespace a8::util {

class Application : public Component {
private:
protected:
public:
    Application() {
    }

    virtual Scheduler *createScheduler() = 0;
    virtual LoggerFactory *createLoggerFactory() = 0;
    virtual void start(Context &context) {
        Component::start(context);
    }
    virtual void start() {
        Scheduler *scheduler = this->createScheduler();
        LoggerFactory *loggerFac = this->createLoggerFactory();
        Context *context = new Context(scheduler, loggerFac);

        this->stageTo(PostStart, *context);

        context->scheduler->startSchedule();
    }
};

} // namespace a8::util