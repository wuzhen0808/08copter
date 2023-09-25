#pragma once
#include "a8/util/Attributes.h"
#include "a8/util/Buffer.h"
#include "a8/util/Context.h"
#include "a8/util/ForEach.h"
#include "a8/util/Logger.h"
#include "a8/util/LoggerFactory.h"
#include "a8/util/Rate.h"
#include "a8/util/Runnable.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {
enum Stage {
    Zero = 0,
    Boot,
    Populate,
    PostPopulate,
    Setup,
    PostSetup,
    Start,
    PostStart,
    Shutdown,
    PostShutdown
};

class Component : public Runnable {

protected:
    Attributes *attributes;
    Logger *logger;
    LoggerFactory *loggerFactory;
    Buffer<Component *> *children;
    Stage stage;
    String name;
    Rate rate; // in mHz = 0.001Hz

public:
    Component(const char *name) {
        String name_;
        name_ = name;
        this->init(name_);
    }

    Component(const String &name) {
        this->init(name);
    }

    ~Component() {
        if (this->attributes != 0) {
            delete[] attributes;
            attributes = 0;
        }
    }
    void init(const String &name) {
        this->children = new Buffer<Component *>();
        this->attributes = 0;
        this->logger = 0;
        this->stage = Zero;
        this->name = name;
        this->rate = 0;
    }
    Rate getRate() {
        return rate;
    }
    Logger *getLogger() {
        if (this->logger == 0) {
            this->logger = loggerFactory->getLogger("default"); // TODO name?
        }
        return this->logger;
    }
    String getName() {
        return name;
    }
    bool isName(const String &name) {
        return this->name == name;
    }

    Component *findComponent(String &path) {
        Buffer<String> names = path.split('/');
        return findComponent(names, 0);
    }

    Component *findChild(const String &name) {
        for (int i = 0; i < this->children->getLength(); i++) {
            Component *child = children->get(i);
            if (child->isName(name)) {
                return child;
            }
        }
        return 0;
    }

    Component *findComponent(Buffer<String> &path, int from) {
        String name = path.get(from);
        Component *child = this->findChild(name);
        if (child == 0 || from >= path.getLength()) {
            return 0;
        }

        return child->findComponent(path, from + 1);
    }
    Buffer<Component *> *collect(Buffer<Component *> *buffer) {
        buffer->append(*this->children);
        for (int i = 0; i < children->getLength(); i++) {
            children->get(i)->collect(buffer);
        }
        return buffer;
    }
    /**
     * Tick method must not blocked and should be quickly returned.
     */
    virtual void tick(const long tickTime) {
    }
    /*
     * Run method is in a separate thread. It's a loop never end until system shutdown.
     */
    virtual void run() override {
    }

    void log(const String &msg) {
        getLogger()->info(msg);
    }

    void addChild(Component *com, Context &context) {
        com->stageTo(this->stage, context);
        this->children->append(com);
    }
    virtual void boot(Context &context) {
        this->loggerFactory = context.loggerFactory;
        stageChildrenTo(Boot, context);
    }
    virtual void populate(Context &context) {
        stageChildrenTo(Populate, context);
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
        if (context.isStop()) {
            log("Context stoped, cannot stage forward.");
            return;
        }
        switch (this->stage) {
        case Zero:
            this->stage = Boot;
            this->boot(context);
            if (isStage(stage2)) {
                break;
            }
        case Boot:
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
