#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Component.h"
#include "a8/util/Context.h"
#include "a8/util/Properties.h"
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

        context->scheduler->startSchedule();
    }
};

} // namespace a8::util