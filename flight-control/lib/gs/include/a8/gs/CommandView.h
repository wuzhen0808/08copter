#pragma once
#include "a8/gs/Background.h"
#include "a8/gs/DialogView.h"
#include "a8/gs/View.h"
#include "a8/util.h"
#include "a8/util/curses.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::curses;
namespace a8::gs {

class CommandView : public View {

    static void onBridgeCreate(Bridge<GsSkeleton> *bridge, CommandView *context) {
        context->bridge = bridge;
    }

    Entry *entry;

    Bridge<GsSkeleton> *bridge;

public:
    CommandView(Cdk *cdk, EventCenter *ec, LoggerFactory *logFac) : View(cdk, ec, logFac) {
        this->eventCenter = ec;
        this->entry = 0;
    }

    ~CommandView() {

        delete entry;
    }

    void draw() override {
        if (this->entry == 0) {
            this->entry = cdk->newEntry(CENTER, BOTTOM, "gs# ");
        }
        this->entry->draw();
    }

    bool ifExit() {
        DialogView dialog(cdk, this->eventCenter, loggerFactory);
        dialog << "<C></U>Dialog"
               << " "
               << "<C>Going to exit the </B/32>GS<!B!32>?";
        int *idx;

        dialog.addButton("</B/24>OK");
        dialog.addButton("</B/16>Cancel");
        dialog.draw();
        EExitType type;
        int ret = dialog.activate(type);
        if (ret == 0) {
            return true;
        }
        return false;
    }

    int cmdTakeoff(Result &rst) {
        //
        Bridge<GsSkeleton> *bridge = this->bridge;
        int ret = 0;
        if (bridge == 0) {
            //
            log("bridge is broken or not established yet.");
            ret = -1;
        } else {
            FcApi *fcApi = bridge->stub<FcApi>();
            ret = fcApi->ping("take off.", rst);
        }
        return ret;
    }
    void cmdHelp() {
    }
    int run(Result &rst) {

        int ret = 0;
        for (; ret >= 0;) {
            entry->draw();
            String cmd;
            ret = entry->activate(cmd);
            entry->clean();

            if (ret < 0) {
                break;
            }
            log(cmd);
            if (entry->exitType() == vESCAPE_HIT || cmd.equalsIgnoreCase("q") || cmd.equalsIgnoreCase("quit") || cmd.equalsIgnoreCase("exit")) {
                if (ifExit()) {
                    break;
                }
            } else if (cmd.equalsIgnoreCase("help")) {
                cmdHelp();
            } else if (cmd.equalsIgnoreCase("takeoff")) {
                ret = cmdTakeoff(rst);
            } else if (cmd.equalsIgnoreCase("refresh")) {
                cdk->refresh();
            } else {
                log("no such command" << cmd);
            }

            if (ret < 0) {
                log(rst.errorMessage);
                ret = rst.reset();
            }

        } //
        this->eventCenter->notifyEvent(EventTypes::BEFORE_QUIT, 0);

        return ret;
    }
};
} // namespace a8::gs