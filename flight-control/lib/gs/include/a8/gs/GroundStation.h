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
class ChannelRunner : public Component {
public:
    static void run(void *runner) {
        ChannelRunner *cr = static_cast<ChannelRunner *>(runner);
        //cr->run();
    }
    Channel *channel;
    FcStub *fcStub;
    GsNetImp *skeleton;
    ChannelRunner(Channel *ch, FcStub *fcStub) : Component("channelRunnerX") {
        this->channel = ch;
        this->fcStub = fcStub;        
    }

    // ~ChannelRunner() {
    //     log(String()<<">>~ChannelRunner() ");
    //     delete channel;
    //     log(String()<<" after delete channel, ~ChannelRunner() ");
    //     delete fcStub;        
    //     log(String()<<"<<~ChannelRunner() ");
    // }

    // void run() {
    //     Result rst;
    //     while (true) {
    //         int ret = channel->receive(1, rst);
    //         if (ret < 0) {
    //             break;
    //         }
    //         if (ret == 0) {
    //             break;
    //         }
    //         fcStub->ping("hello, ack.");
    //     }
    //     log("Channel Runner Exited");
    // }
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
            GsNetImp *skeleton = new GsNetImp(this->dashboard);
            
            int ret = links->acceptGs(ch, fcStub, skeleton, rst);
            if (ret < 0) {
                log(rst.errorMessage);
                break;
            }
            log("A new GS client connected in, we treat it as a FcStub.");
            Component * cm1 = new Component("tmp");

            ChannelRunner *cr = new ChannelRunner(ch, fcStub);
            //cr->addChild(ticking->getStaging(), skeleton);
            cr->addChild(ticking->getStaging(), cm1);
            cr->stageTo(Boot, ticking->getStaging());
            
            // TODO manage all the channels.
            // TODO select chanel and avoid multiple runner thread.
            //this->addChild(ticking->getStaging(), cr);

            // while (true) {
            //     int ret = cr->channel->receive(rst);
            //     if (ret < 0) {
            //         //
            //         log(String() << "exit the receive loop of the client connection, ret:" << ret);
                    
            //         break;
            //     }
            // }
            //this->deleteChild(cr);
            //delete cr;
        }
        log("Warning: GS net accepter exited.");
    }

    void shutdown(StagingContext *context) override {
        Component::shutdown(context);
    }
};

} // namespace a8::gs