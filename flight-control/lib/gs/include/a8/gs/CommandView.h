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
    Entry *entry;
    Background *bg;
    Bridge *bridge;

public:
    CommandView(Cdk *cdk, Background *bg, LoggerFactory *logFac) : View(cdk, logFac) {
        this->bg = bg;
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

    void onBridge(Bridge *bridge, int type) {
        switch (type) {
        case 0:
            this->bridge = 0;
            break;
        case 1:
            this->bridge = bridge;
            break;
        }
    }

    bool ifExit() {
        DialogView dialog(cdk, loggerFactory);
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

    void cmdTakeoff() {
        //
        Bridge *bridge = this->bridge;
        if (bridge == 0) {
            //
            log("bridge is broken or not established yet.");
        } else {
            FcApi *fcApi = bridge->stub<FcApi>();
            fcApi->ping("take off.");
        }
    }
    void cmdHelp() {
    }
    int run(Result &rst) {

        bg->add(this, [](Bridge *bridge, int type, void *this_) {
            static_cast<CommandView *>(this_)->onBridge(bridge, type);
        });
        int ret = -1;
        for (;;) {
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
                cmdTakeoff();
            } else if (cmd.equalsIgnoreCase("refresh")) {
                cdk->refresh();
            } else {
                log("no such command" << cmd);
            }

        } //
        bg->close();

        return ret;
    }
};
} // namespace a8::gs