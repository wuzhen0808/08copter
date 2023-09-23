#pragma once
#include "a8/util/Attributes.h"
#include "a8/util/Buffer.h"
#include "a8/util/Context.h"
#include "a8/util/Logger.h"
#include "a8/util/LoggerFactory.h"
#include "a8/util/Runnable.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {
enum Stage {
    Boot = 1,
    Populate,
    PostPopulate,
    Setup,
    PostSetup,
    Start,
    PostStart,    
    Shutdown,
    PostShutdown
};

class Component {

protected:
    Attributes *attributes;
    Logger *logger;
    LoggerFactory *loggerFactory;
    Buffer<Component *> *children;
    Stage stage = Boot;

public:
    Component() {
        this->attributes = 0;
        this->logger = 0;
        this->children = 0;
    }

    ~Component() {
        if (this->attributes != 0) {
            delete[] attributes;
            attributes = 0;
        }
    }

    Logger *getLogger() {
        if (this->logger == 0) {
            this->logger = loggerFactory->getLogger("default"); // TODO name?
        }
        return this->logger;
    }

    void log(const String &msg) {
        getLogger()->info(msg);
    }

    void addChild(Component *com, Context &context) {
        if (this->children == 0) {
            this->children = new Buffer<Component *>();
        }
        com->stageTo(this->stage, context);
        this->children->append(com);
    }

    virtual void populate(Context &context) {
        stageChildrenTo(Populate, context);
        this->loggerFactory = context.loggerFactory;
    }

    virtual void postPopulate(Context &context) {
        this->stageChildrenTo(PostPopulate, context);
    }
    virtual void setup(Context &context) {
        this->stageChildrenTo(Setup, context);
    }
    virtual void postSetup(Context &context) {
        this->stageChildrenTo(PostSetup, context);
    }

    virtual void start(Context &context) {
        this->stageChildrenTo(Start, context);
    }

    virtual void postStart(Context &context) {
        this->stageChildrenTo(PostStart, context);
    }

    bool isStage(Stage stage) {
        return this->stage == stage;
    }
    virtual void stageChildrenTo(Stage stage2, Context &context) {
        if (this->children != 0) {
            int count = this->children->getLength();
            for (int i = 0; i < count; i++) {
                Component *com = children->get(i);
                com->stageTo(stage2, context);
            }
        }
    }

    virtual void stageTo(Stage stage2, Context &context) {

        switch (this->stage) {
        case Boot:
            // DO Nothing.
            this->stage = Populate;
            this->populate(context);
            if (isStage(stage2)) {
                break;
            }

        case Populate:
            this->stage = PostPopulate;
            this->postPopulate(context);
            if (isStage(stage2)) {
                break;
            }
        case PostPopulate:
            this->stage = Setup;
            this->setup(context);
            if (isStage(stage2)) {
                break;
            }
        case Setup:
            this->stage = PostSetup;
            this->postSetup(context);
            if (isStage(stage2)) {
                break;
            }
        case PostSetup:
            this->stage = Start;
            this->start(context);
            if (isStage(stage2)) {
                break;
            }
        case Start:
            this->stage = PostStart;
            this->postStart(context);
            if (isStage(stage2)) {
                break;
            }
        case PostStart:
            break;
        default:
            // Unknown stage.
            break;
        }
    }
    virtual void stop() {
    }
};
} // namespace a8::util
