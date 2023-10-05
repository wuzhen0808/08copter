#pragma once
#include "a8/gs/Dashboard.h"
#include "a8/gs/GsNetImp.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
namespace a8::gs {
class ChannelRunner : public FlyWeight {
public:
    static void run(void *runner) {
        ChannelRunner *cr = static_cast<ChannelRunner *>(runner);
        cr->run();
    }
    Channel *channel;
    FcStub *fcStub;
    GsNetImp *skeleton;
    ChannelRunner(Channel *ch, GsNetImp *skeleton, FcStub *fcStub, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->channel = ch;
        this->fcStub = fcStub;
        this->skeleton = skeleton;
    }

    ~ChannelRunner() {
        log(String() << ">>~ChannelRunner() ");
        delete channel;
        delete fcStub;
        delete skeleton;
        log(String() << "<<~ChannelRunner() ");
    }

    void run() {
        Result rst;
        while (true) {
            int ret = channel->receive(1, rst);
            if (ret < 0) {
                break;
            }
            if (ret == 0) {
                break;
            }
            fcStub->ping("hello, ack.");
        }
        log("Channel Runner Exited");
    }
};
class GroundStation : public Component {

    Dashboard *dashboard;
    Links *links;
    FcApi *fcApi;

public:
    GroundStation(int argc, char **argv, Links *links) : Component("gs") {
        this->links = links;
        this->rate = Rate::ForEver;
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
    }

    void setup(StagingContext *context) override {
        Component::setup(context);

        // open dashboard
        this->dashboard = new Dashboard();
        this->addChild(context, this->dashboard);
        // waiting fcs to connect
        Result rst;
        int ret = links->bindGs(rst);
        if (ret < 0) {
            return context->stop(rst.errorMessage);
        }

        ret = links->listenGs(rst);
        if (ret < 0) {
            return context->stop(rst.errorMessage);
        }
        log("listening connect in on port of gs.");
    }

    void run(TickingContext *ticking) override {
        log("GS net accepter running...");
        while (true) {
            Channel *ch = 0;
            FcStub *fcStub = 0;
            Result rst;
            GsNetImp *skeleton = new GsNetImp(this->dashboard, this->loggerFactory);

            int ret = links->acceptGs(ch, fcStub, skeleton, rst);
            if (ret < 0) {
                delete skeleton;
                log(rst.errorMessage);
                break;
            }

            log("A new GS client connected in, we treat it as a FcStub.");

            ChannelRunner *cr = new ChannelRunner(ch, skeleton, fcStub, this->loggerFactory);

            // TODO manage all the channels.
            // TODO select chanel and avoid multiple runner thread.

            while (true) {
                int ret = cr->channel->receive(rst);
                if (ret < 0) {
                    //
                    log(String() << "exit the receive loop of the client connection, ret:" << ret);

                    break;
                }
            }
            delete cr;
        }
        log("Warning: GS net accepter exited.");
    }

    void shutdown(StagingContext *context) override {
        Component::shutdown(context);
    }
};

} // namespace a8::gs