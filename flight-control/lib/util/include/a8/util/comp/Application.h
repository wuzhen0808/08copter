#pragma once
#include "a8/util.h"
#include "a8/util/comp/Component.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/comp/TickRunner.h"
#include "a8/util/thread.h"
using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {

/**
 * Application is the typical implementation of a root component.
 *
 * - As the entry point of an app.
 * - Driving components tree to the desired stage.
 * - Provide schedule service after the start stage.
 *
 */
class Application : public Component {
private:
protected:
    Buffer<TickRunner *> *rateRunners_;

    Buffer<TickRunner *> *buildTickRunners(StagingContext *context, Buffer<TickRunner *> *runners) {
        Buffer<Component *> *allComponents = this->collect(new Buffer<Component *>());

        for (int i = 0; i < allComponents->length(); i++) {
            Component *component = allComponents->get(i);
            Rate rate = component->getRate();

            // find a runner for the component.
            TickRunner *runner = this->findTickRunner(runners, rate);
            if (runner == 0) {
                runner = new TickRunner(rate, context->sys);
                runners->append(runner);
            }
            runner->add(component);
        }
        return runners;
    }
    TickRunner *findTickRunner(Buffer<TickRunner *> *runners, const Rate &rate_) {
        for (int i = 0; i < runners->length(); i++) {
            TickRunner *runner = runners->get(i);
            long mHz1 = runner->rate.mHz();
            long mHz2 = rate_.mHz();
            if (mHz1 == mHz2) {
                return runner;
            }
        }
        return 0;
    }

public:
    /**
     * This is a utility method. This method implement following functions.
     *
     * - Building application from a child component.
     * - Getting the application to PostStart stage.
     *
     */
    static Application *start(String name, StagingContext *context, Component *child) {
        Application *app = new Application(name.text());
        app->addChild(child)->stageTo(PostStart, context);

        StringWriter strW;
        app->print(&strW);
        app->log(strW.toString());
        if (context->isStop()) {
            app->log(context->getMessage());
        }
        return app;
    }

    Application(const char *name) : Component(name) {
    }

    void boot(StagingContext *context) override {
        Component::boot(context);
        // Properties only valid during the start up scope, these properties are regarded as local(during application start up) variables.
        // Properties is as the interface between component and it's environment.
        // So properties normally assigned values from outside and read values by component in boot stage.

        Logger *logger = context->loggerFactory->getLogger("");
        Buffer<String> names = context->properties->getNames();

        logger->info("====== Start of properties === ");
        for (int i = 0; i < names.length(); i++) {
            String name = names.get(i);
            String line = context->properties->getLine(name);
            logger->info(line);
        }
        logger->info("====== End of properties =====");
    }

    void postPopulate(StagingContext *context) override {
        Component::postPopulate(context);
    }
    void start(StagingContext *context) override {
        Component::start(context);

        // ticker
        this->rateRunners_ = buildTickRunners(context, new Buffer<TickRunner *>());
        for (int i = 0; i < rateRunners_->length(); i++) {
            TickRunner *runner = rateRunners_->get(i);
            runner->run(context);
        }
    }
    //
    void postStart(StagingContext *context) override {
        Component::postStart(context);
    }
};

} // namespace a8::util::comp