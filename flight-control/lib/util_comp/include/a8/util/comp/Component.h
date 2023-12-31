#pragma once
#include "a8/util.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/comp/TickingContext.h"
#include "a8/util/comp/defines.h"
#include "a8/util/sched.h"

namespace a8::util::comp {
using namespace a8::util;
using namespace a8::util::sched;

/**
 *
 * Component is the middle-size building block.
 *
 * It is a object managed by the parent and the root component.
 *
 * It encapsulates the business which require common service from environment.
 *
 * The services provided by the caller of a component are below.
 *
 *  1. Scheduler service to start a thread and running a Runnable object.
 *  2. LoggerFactory service which provide loggers with names.
 *  3. Network service to communicate with other component. Network service is to be implemented.
 *
 * What should not be a component?
 *
 *  1. The functions or logics under coding do not require external services above.
 *  2. It is a object and created with a higher freq, not a singleton.
 *
 * How to use the component?
 *
 *  1. Provide a context before create any component.
 *  2. Implement and override the methods driving by stages.All the stages are below.  *
 *      - Zero            - stage 0
 *      - Boot            - stage 1
 *      - Populate        - stage 2
 *      - PostPopulate    - stage 3
 *      - Setup           - stage 4
 *      - PostSetup       - stage 5
 *      - Shutdown        - stage 6
 *      - PostShutdown    - stage 7
 *
 *  3. Implement more component and add them by addChild(Component*) method.
 *      Note the stage in which you add a child.
 *      The child component will be automatically driven to the same stage as the parent component.
 *
 *  4. Optionally create a root component, such as an Application component.
 *
 *  5. Calling stageTo(aStage);
 *      This method will execute the methods mentioned above one by one. Until there is some error
 *      required the staging procedure stop.
 *      The stageTo method is executed recursively into each child and finally get the whole components
 *      tree into the destination stage.
 *
 *  6. If your component to be running in a thread.
 *      - Implement the run() method.
 *      - Set the rate, frequency in HZ, to a non zero value.
 *      - Make sure the root component is an Application component.
 *
 *  7. Note the root component.
 *      For example if the Application component is as the root component. At the postStart stage,
 *      the Application component will:
 *
 *      - Collect all the runnable components, assign thread(by the rate specified) for them and start the scheduler.
 *
 */
class Component {
public:
    struct TickEntry {
        Rate rate;
        void *component;
        void *handle;
        void (*tickHandle)(TickingContext *tc, void *component, void *handle);
    };

private:
    Buffer<TickEntry *> ticks; // in mHz = 0.001Hz
protected:
    Logger *logger;
    LoggerFactory *loggerFactory;
    Buffer<Component *> *children;
    Stage stage;
    String path; //
    String name;

    void print(Writer *writer, bool recursive, int indent) {
        for (int i = 0; i < indent; i++) {
            *writer << ' ';
        }
        *writer << name << ",stage:" << this->stage << '\n';
        if (!recursive) {
            return;
        }
        for (int i = 0; i < children->len(); i++) {
            children->get(i, 0)->print(writer, indent + 1, recursive);
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

    virtual ~Component() {
        //
        Buffer<Component *> tmp;
        tmp.append(this->children);

        for (int i = tmp.length() - 1; i >= 0; i--) {
            Component *com = tmp.get(i, 0);
            deleteChild(com);
        }
        delete this->children;
    }

    template <typename T>
    void schedule(void (*handleF)(TickingContext *tc, T *this_)) {
        this->schedule(Rate::RUN, handleF);
    }
    template <typename T>
    void scheduleHz1(void (*handleF)(TickingContext *tc, T *this_)) {
        this->schedule(1.0f, handleF);
    }

    template <typename T>
    void scheduleHz100(void (*handleF)(TickingContext *tc, T *this_)) {
        this->schedule(100.0f, handleF);
    }

    template <typename T>
    void scheduleHz500(void (*handleF)(TickingContext *tc, T *this_)) {
        this->schedule(500.0f, handleF);
    }

    template <typename T>
    void scheduleHz10(void (*handleF)(TickingContext *tc, T *this_)) {
        this->schedule(10.0f, handleF);
    }

    template <typename T>
    void scheduleHz20(void (*handleF)(TickingContext *tc, T *this_)) {
        this->schedule(20.0f, handleF);
    }

    template <typename T>
    void schedule(Rate rate, void (*handleF)(TickingContext *tc, T *this_)) {
        using handleT = void (*)(TickingContext *tc, T *this_);
        TickEntry *entry = new TickEntry();
        entry->rate = rate;
        entry->component = this;
        entry->handle = reinterpret_cast<void *>(handleF);
        entry->tickHandle = [](TickingContext *tc, void *this_, void *handleF2) {
            handleT handleF3 = reinterpret_cast<handleT>(handleF2);
            handleF3(tc, Lang::cast<T *>(this_));
        };
        this->ticks.append(entry);
    }

    void deleteChild(Component *child) {
        int ret = this->removeChild(child);
        if (ret < 0) {
            Assert::illegalArgument("what's wrong?");
        }
        delete child;
    }

    void init(const String &name) {
        A8_DEBUG_PRINT("Component::init,name:");
        A8_DEBUG_PRINT(name.text());
        A8_DEBUG_PRINTLN();
        this->children = new Buffer<Component *>();
        this->logger = 0;
        this->stage = Zero;
        this->name = name;
    }

    Buffer<TickEntry *> getTicks() {
        return ticks;
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

        Buffer<String> names = StringUtil::split(path, '/');
        return findComponent(names, 0);
    }

    Component *findChild(const String &name) {
        for (int i = 0; i < this->children->length(); i++) {
            Component *child = children->get(i, 0);
            if (child->isName(name)) {
                return child;
            }
        }
        return 0;
    }

    Component *findComponent(Buffer<String> &path, int from) {
        String name = path.get(from, "");
        Component *child = this->findChild(name);
        if (child == 0 || from >= path.length()) {
            return 0;
        }

        return child->findComponent(path, from + 1);
    }
    Buffer<Component *> *collect(Buffer<Component *> *buffer) {
        buffer->append(*this->children);
        for (int i = 0; i < children->length(); i++) {
            children->get(i, 0)->collect(buffer);
        }
        return buffer;
    }

    void log(const Result rst) {
        log(rst.errorMessage);
    }

    void log(const String &msg) {
        getLogger()->info(msg);
    }

    int removeChild(Component *com) {
        int ret = this->children->removeEle(com);
        return ret;
    }

    Component *addChild(StagingContext *context, Component *com) {
        com->stageTo(this->stage, context);
        this->children->append(com);

        com->path = StringUtil::buildStr(*context->getPath(), '/');

        return this;
    }

    void beforeStage(StagingContext *context) {
        A8_DEBUG_PRINT("Component::beforeStage,name:");
        A8_DEBUG_PRINT(name.text());
        A8_DEBUG_PRINTLN();
        Buffer<String> *path = context->getPath();
        path->append(this->name);
        String msg(">>");
        for (int i = 0; i < path->len(); i++) {
            msg << "/" << path->get(i, "");
        }
        msg << ":" << this->stage;
        log(msg);
    }

    void afterStage(StagingContext *context) {
        Buffer<String> *path = context->getPath();
        String msg("<<");
        for (int i = 0; i < path->len(); i++) {
            msg << "/" << path->get(i,"");
        }
        msg << ":" << this->stage;
        log(msg);
        path->removeLast();
    }

    virtual void boot(StagingContext *context) {
        stageChildrenTo(Boot, context);
    }
    virtual void populate(StagingContext *context) {
        stageChildrenTo(Populate, context);
    }

    virtual void postPopulate(StagingContext *context) {

        this->stageChildrenTo(PostPopulate, context);
    }
    virtual void setup(StagingContext *context) {
        this->stageChildrenTo(Setup, context);
    }
    virtual void postSetup(StagingContext *context) {
        this->stageChildrenTo(PostSetup, context);
    }

    virtual void start(StagingContext *context) {
        this->stageChildrenTo(Start, context);
    }

    virtual void postStart(StagingContext *context) {
        this->stageChildrenTo(PostStart, context);
    }
    virtual void shutdown(StagingContext *context) {
        this->stageChildrenTo(Shutdown, context);
    }

    virtual void postShutdown(StagingContext *context) {
        this->stageChildrenTo(PostShutdown, context);
    }

    bool isStage(Stage stage) {
        return this->stage == stage;
    }
    virtual void stageChildrenTo(Stage stage2, StagingContext *context) {
        if (this->children != 0) {
            int count = this->children->length();
            for (int i = 0; i < count; i++) {
                Component *com = children->get(i, 0);
                com->stageTo(stage2, context);
                if (context->isStop()) {
                    break;
                }
            }
        }
    }

    virtual Component *stageTo(Stage stage2, StagingContext *context) {

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

    friend String &operator<<(String &str, Component *comp) {
        return operator<<(str, *comp);
    }
    friend String &operator<<(String &str, Component &comp) {
        return str << comp.name << "," << comp.stage;
    }

    void print(Writer *writer) {
        print(writer, true, 0);
    }
    void print(Writer *writer, bool recursive) {
        print(writer, recursive, 0);
    }
};
} // namespace a8::util::comp
