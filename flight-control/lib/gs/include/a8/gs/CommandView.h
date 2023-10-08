#pragma once
#include "a8/gs/Background.h"
#include "a8/util.h"
#include "a8/util/curses.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::curses;
namespace a8::gs {

class CommandView : public FlyWeight {
    Entry *entry;
    Background *bg;
    Bridge *bridge;
    Cdk *cdk;

public:
    CommandView(Cdk *cdk, Background *bg, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->cdk = cdk;
        this->entry = cdk->newEntry(CENTER, BOTTOM, "gs# ");
        this->bg = bg;
    }

    ~CommandView() {
        delete entry;
    }

    void onBridge(Bridge *bridge, int isCreate) {
        if (isCreate) {
            this->bridge = bridge;
        } else {
            Result rst;
            int ret = this->bridge->isRunning(rst);
            if (ret < 0) {
            }
            this->bridge = 0;
        }
    }
    bool askIfRealExit() {
        Buffer<String> message;
        int i = 0;
        message.append("<C></U>Dialog");
        message.append(" ");
        message.append("<C>Going to exit the </B/32>GS<!B!32>?");
        Buffer<String> buttons;
        buttons.append("</B/24>OK");
        buttons.append("</B16>Cancel");
        Dialog *dialog = cdk->newDialog(CENTER, CENTER, message, buttons);
        int selection = 0;
        EExitType ret = dialog->activate(selection);
        delete dialog;

        log(String() << "you select:" << selection << ",exitType:" << ret);
        if (ret == vNORMAL) {
            if (selection == 0) {
                return true;
            }
        }
        return false;
    }
    int run(Result &rst) {

        bg->add(this, [](Bridge *bridge, int isCreate, void *this_) {
            static_cast<CommandView *>(this_)->onBridge(bridge, isCreate);
        });
        int ret = -1;
        for (;;) {
            entry->draw();
            String cmd;
            log("before entry active.");
            ret = entry->activate(cmd);

            log("after entry active.");

            if (ret < 0) {
                break;
            }
            log(cmd);
            if (entry->exitType() == vESCAPE_HIT || cmd.equalsIgnoreCase("q") || cmd.equalsIgnoreCase("quit") || cmd.equalsIgnoreCase("exit")) {
                if (askIfRealExit()) {
                    break;
                } else {
                    // ignore
                }
            } else if (cmd.equalsIgnoreCase("help")) {

            } else if (cmd.equalsIgnoreCase("takeoff")) {
                //
                Bridge *bridge = this->bridge;
                if (bridge == 0) {
                    //
                    log("bridge is broken or not established yet.");
                } else {
                    FcApi *fcApi = bridge->stub<FcApi>();
                    fcApi->ping("take off.");
                }
            } else {
                log("no such command" << cmd);
            }
        }//
        bg->close();
        
        return ret;
    }
};
} // namespace a8::gs