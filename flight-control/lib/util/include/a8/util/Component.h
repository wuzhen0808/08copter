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

    void print(Writer *writer, bool recursive, int indent) {
        for (int i = 0; i < indent; i++) {
            *writer << ' ';
        }
        *writer << name << ",stage:" << this->stage << '\n';
        if (!recursive) {
            return;
        }
        for (int i = 0; i < children->len(); i++) {
            children->get(i)->print(writer, indent + 1, recursive);
        }
    }

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

    virtual Component *addChild(Component *com) {
        this->children->append(com);
        return this;
    }

    Component *addChild(Context *context, Component *com) {
        com->stageTo(this->stage, context);
        this->addChild(com);
        return this;
    }

    void beforeStage(Context *context) {
        Buffer<String> *path = context->getPath();
        path->append(this->name);
        String msg(">>");
        for (int i = 0; i < path->len(); i++) {
            msg << "/" << path->get(i);
        }
        msg << ":" << this->stage;
        log(msg);
    }

    void afterStage(Context *context) {
        Buffer<String> *path = context->getPath();
        String msg("<<");
        for (int i = 0; i < path->len(); i++) {
            msg << "/" << path->get(i);
        }
        msg << ":" << this->stage;
        log(msg);
        path->removeLast();
    }

    virtual void boot(Context *context) {
        stageChildrenTo(Boot, context);
    }
    virtual void populate(Context *context) {
        stageChildrenTo(Populate, context);
    }

    virtual void postPopulate(Context *context) {

        this->stageChildrenTo(PostPopulate, context);
    }
    virtual void setup(Context *context) {
        this->stageChildrenTo(Setup, context);
    }
    virtual void postSetup(Context *context) {
        this->stageChildrenTo(PostSetup, context);
    }

    virtual void start(Context *context) {
        this->stageChildrenTo(Start, context);
    }

    virtual void postStart(Context *context) {
        this->stageChildrenTo(PostStart, context);
    }
    virtual void shutdown(Context *context) {
        this->stageChildrenTo(Shutdown, context);
    }

    virtual void postShutdown(Context *context) {
        this->stageChildrenTo(PostShutdown, context);
    }

    bool isStage(Stage stage) {
        return this->stage == stage;
    }
    virtual void stageChildrenTo(Stage stage2, Context *context) {
        if (this->children != 0) {
            int count = this->children->getLength();
            for (int i = 0; i < count; i++) {
                Component *com = children->get(i);
                com->stageTo(stage2, context);
                if (context->isStop()) {
                    break;
                }
            }
        }
    }

    virtual Component *stageTo(Stage stage2, Context *context) {

        switch (this->stage) {
        case Zero:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->loggerFactory = context->loggerFactory;
            this->stage = Boot;
            beforeStage(context);
            this->boot(context);
            afterStage(context);

        case Boot:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = Populate;
            beforeStage(context);
            this->populate(context);
            afterStage(context);

        case Populate:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = PostPopulate;
            beforeStage(context);
            this->postPopulate(context);
            afterStage(context);

        case PostPopulate:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = Setup;
            beforeStage(context);
            this->setup(context);
            afterStage(context);

        case Setup:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = PostSetup;
            beforeStage(context);
            this->postSetup(context);
            afterStage(context);

        case PostSetup:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = Start;
            beforeStage(context);
            this->start(context);
            afterStage(context);

        case Start:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = PostStart;
            beforeStage(context);
            this->postStart(context);
            afterStage(context);
        case PostStart:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = Shutdown;
            beforeStage(context);
            this->postStart(context);
            afterStage(context);

        case Shutdown:
            if (isStage(stage2) || context->isStop()) {
                break;
            }
            this->stage = PostShutdown;
            beforeStage(context);
            this->shutdown(context);
            afterStage(context);

        default:
            // Unknown stage.
            break;
        }
        if (context->isStop()) {
            this->log(context->getMessage());
        }
        return this;
    }
    virtual void stop() {
    }
    void print(Writer *writer) {
        print(writer, true, 0);
    }
    void print(Writer *writer, bool recursive) {
        print(writer, recursive, 0);
    }
};
} // namespace a8::util
